#include "Symulacja.h"

Symulacja::Symulacja(QObject *parent)
    : QObject(parent),
    m_generator(),
    m_prostyUAR(this), // ProstyUAR tworzy sobie ARX i PID w środku
    m_czyDziala(false),
    m_czas(0.0),
    m_wartoscZadana(0.0),
    m_wartoscWyjscie(0.0),
    m_sterowanie(0.0),
    m_uchyb(0.0)
{
}

void Symulacja::konfigurujModel(const std::vector<double>& A, const std::vector<double>& B, int opoznienie)
{
    // Sięgamy do wnętrza ProstyUAR, aby skonfigurować JEGO model
    m_prostyUAR.getModel().setA(A);
    m_prostyUAR.getModel().setB(B);
    m_prostyUAR.getModel().setOpoznienieTransportowe(opoznienie);
}

void Symulacja::konfigurujRegulator(double k, double ti, double td)
{
    // Sięgamy do wnętrza ProstyUAR, aby skonfigurować JEGO regulator
    m_prostyUAR.getRegulator().setWzmocnienie(k);
    m_prostyUAR.getRegulator().setStalaCalk(ti);
    m_prostyUAR.getRegulator().setStalaRozn(td);
    m_prostyUAR.setTrybOtwarty(false);
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
}

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

    m_generator.reset();

    // Resetujemy stan wewnętrzny obiektów, ale NIE zmieniamy trybu pracy!
    m_prostyUAR.reset();

    // USUNIĘTO: m_prostyUAR.setTrybOtwarty(true);
    // Tryb powinien być zmieniany tylko przez konfigurujRegulator()
    // lub jawnie przez UI.
}

void Symulacja::wykonajKrok()
{
    // 1. Generator
    m_wartoscZadana = m_generator.generuj();
    m_generator.krokSymulacji();

    // 2. UAR (wszystko dzieje się w środku)
    m_wartoscWyjscie = m_prostyUAR.symuluj(m_wartoscZadana);

    // 3. Pobranie danych do wykresów
    m_uchyb = m_prostyUAR.getOstatniUchyb();
    m_sterowanie = m_prostyUAR.getOstatnieSterowanie();


    // --- DEBUGOWANIE ---
    // Pokaż co 10-tą próbkę lub zawsze, jeśli krok jest wolny
    // static int licznik = 0;
    // if (licznik++ % 10 == 0) {
    qDebug() << "T:" << m_czas
             << " Zad:" << m_wartoscZadana
             << " Ster:" << m_sterowanie
             << " Wyj:" << m_wartoscWyjscie
             << " TrybOtwarty:" << m_prostyUAR.czyTrybOtwarty();
    // }
    // -------------------

    // 4. Czas
    double dt_sec = m_generator.getInterwal() / 1000.0;
    m_czas += dt_sec;
}

/*
QJsonObject Symulacja::toJson() const
{
    QJsonObject obj;
    // Pobieramy config z obiektów wewnątrz ProstyUAR
    obj["model"]     = m_prostyUAR.getModel().toJson();
    obj["regulator"] = m_prostyUAR.getRegulator().toJson();
    obj["generator"] = m_generator.toJson();
    return obj;
}

void Symulacja::fromJson(const QJsonObject& obj)
{
    if (obj.contains("model"))
        m_prostyUAR.getModel().fromJson(obj["model"].toObject());

    if (obj.contains("regulator"))
        m_prostyUAR.getRegulator().fromJson(obj["regulator"].toObject());

    if (obj.contains("generator"))
        m_generator.fromJson(obj["generator"].toObject());

    resetuj();
}
*/
