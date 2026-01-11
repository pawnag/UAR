#include "W_USLUG/Symulacja.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>


Symulacja::Symulacja(QObject *parent)
    : QObject(parent),
    m_trybPracy(TrybPracy::IDLE),
    m_czyDziala(false),
    m_czas(0.0),
    m_maModel(false),
    m_maRegulator(false),
    m_maGenerator(true), // Generator jest domyślnie aktywny (np. stała 0)
    m_wartoscZadana(0.0),
    m_wartoscWyjscie(0.0),
    m_sterowanie(0.0),
    m_uchyb(0.0)
{
    // Domyślna aktualizacja trybu
    aktualizujTrybPracy();
}

// ==========================================
// METODY KONFIGURACYJNE
// ==========================================

void Symulacja::konfigurujModel(const std::vector<double>& A, const std::vector<double>& B, int opoznienie)
{
    m_model.setA(A);
    m_model.setB(B);
    m_model.setOpoznienieTransportowe(opoznienie);
    //m_model.resetuj();

    m_maModel = true;
    aktualizujTrybPracy();
}

void Symulacja::konfigurujRegulator(double k, double ti, double td)
{
    m_regulator.setWzmocnienie(k);
    m_regulator.setStalaCalk(ti);
    m_regulator.setStalaRozn(td);
    m_regulator.resetuj();

    m_maRegulator = true;
    aktualizujTrybPracy();
}

void Symulacja::konfigurujGenerator(double ampl, double okres, int interwal,
                                    GeneratorWartosciZadanej::TypSygnalu typ,
                                    double skladowa, double wypelnienie)
{
    m_generator.setAmplituda(ampl);
    m_generator.setOkresRzeczywisty(okres);
    m_generator.setInterwal(interwal);
    m_generator.setTypSygnalu(typ);
    m_generator.setSkladowaStala(skladowa);
    m_generator.setWypelnienie(wypelnienie);
    m_generator.przeliczOkresDyskretny();

    m_maGenerator = true;
    aktualizujTrybPracy();
}

void Symulacja::aktualizujTrybPracy()
{
    // Logika decyzyjna: Co uruchamiamy?

    if (!m_maGenerator) {
        m_trybPracy = TrybPracy::IDLE;
        return;
    }

    if (m_maRegulator && m_maModel) {
        m_trybPracy = TrybPracy::ZAMKNIETA_UAR;
    }
    else if (m_maModel && !m_maRegulator) {
        m_trybPracy = TrybPracy::OTWARTA_MODEL;
    }
    else if (m_maRegulator && !m_maModel) {
        m_trybPracy = TrybPracy::OTWARTA_PID;
    }
    else {
        m_trybPracy = TrybPracy::TYLKO_GENERATOR;
    }
}

// ==========================================
// LOGIKA STEROWANIA
// ==========================================

void Symulacja::uruchom()
{
    m_czyDziala = true;
}

void Symulacja::zatrzymaj()
{
    m_czyDziala = false;
}

void Symulacja::resetuj()
{
    zatrzymaj();
    m_czas = 0.0;
    m_wartoscZadana = 0.0;
    m_wartoscWyjscie = 0.0;
    m_sterowanie = 0.0;
    m_uchyb = 0.0;

    // Reset stanów wewnętrznych obiektów
    m_model.resetuj();
    m_regulator.resetuj();
    m_generator.reset();
}

void Symulacja::wykonajKrok()
{
    // 1. Zawsze pobieramy wartość zadaną z generatora
    if (m_maGenerator) {
        m_wartoscZadana = m_generator.generuj();
        m_generator.krokSymulacji();
    } else {
        m_wartoscZadana = 0.0;
    }

    // 2. MASZYNA STANÓW - SWITCH CASE
    switch (m_trybPracy)
    {
    case TrybPracy::ZAMKNIETA_UAR:
        // Pełna pętla: e = w - y; u = PID(e); y = ARX(u)
        m_uchyb = m_wartoscZadana - m_wartoscWyjscie;
        m_sterowanie = m_regulator.symuluj(m_uchyb);
        m_wartoscWyjscie = m_model.symuluj(m_sterowanie);
        break;

    case TrybPracy::OTWARTA_MODEL:
        // Sterowanie ręczne: u = w; y = ARX(u)
        m_uchyb = 0.0; // Brak regulacji
        m_sterowanie = m_wartoscZadana;
        m_wartoscWyjscie = m_model.symuluj(m_sterowanie);
        break;

    case TrybPracy::OTWARTA_PID:
        // Test PID: e = w; u = PID(e); y = u (by widzieć wyjście na wykresie)
        m_uchyb = m_wartoscZadana;
        m_sterowanie = m_regulator.symuluj(m_uchyb);
        m_wartoscWyjscie = m_sterowanie;
        break;

    case TrybPracy::TYLKO_GENERATOR:
        // Test sygnału: y = w
        m_uchyb = 0.0;
        m_sterowanie = 0.0;
        m_wartoscWyjscie = m_wartoscZadana;
        break;

    case TrybPracy::IDLE:
    default:
        m_wartoscZadana = 0.0;
        m_uchyb = 0.0;
        m_sterowanie = 0.0;
        m_wartoscWyjscie = 0.0;
        break;
    }

    // 3. Aktualizacja czasu
    double dt_sec = m_generator.getInterwal() / 1000.0;
    m_czas += dt_sec;
}

QJsonObject Symulacja::toJson() const
{
    QJsonObject obj;

    // Konfiguracje podsystemów
    obj["model"]     = m_model.toJson();
    obj["regulator"] = m_regulator.toJson();
    obj["generator"] = m_generator.toJson();

    // Flagi dostępności
    obj["maModel"]     = m_maModel;
    obj["maRegulator"] = m_maRegulator;
    obj["maGenerator"] = m_maGenerator;

    return obj;
}

void Symulacja::fromJson(const QJsonObject& obj)
{
    // Wczytanie konfiguracji podsystemów
    if (obj.contains("model"))
        m_model.fromJson(obj["model"].toObject());

    if (obj.contains("regulator"))
        m_regulator.fromJson(obj["regulator"].toObject());

    if (obj.contains("generator"))
        m_generator.fromJson(obj["generator"].toObject());

    // Flagi dostępności
    m_maModel     = obj["maModel"].toBool();
    m_maRegulator = obj["maRegulator"].toBool();
    m_maGenerator = obj["maGenerator"].toBool();

    // Stany dynamiczne NIE są wczytywane
    m_trybPracy = TrybPracy::IDLE;
    m_czyDziala = false;
    m_czas = 0.0;

    m_wartoscZadana = 0.0;
    m_wartoscWyjscie = 0.0;
    m_sterowanie = 0.0;
    m_uchyb = 0.0;

    // Aktualizacja trybu pracy po wczytaniu flag
    aktualizujTrybPracy();
}

