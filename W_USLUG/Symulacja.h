#ifndef SYMULACJA_H
#define SYMULACJA_H

#include <QObject>
#include "W_DANYCH/ModelARX.h"
#include "W_DANYCH/RegulatorPID.h"
#include "W_DANYCH/GeneratorWartosciZadanej.h"

class Symulacja : public QObject
{
    Q_OBJECT

public:
    // Definicja trybów pracy symulatora
    enum class TrybPracy {
        IDLE,               // Brak symulacji / Stop
        TYLKO_GENERATOR,    // Wyjście = Zadana (test generatora)
        OTWARTA_MODEL,      // Sterowanie ręczne: Zadana -> Model -> Wyjście
        OTWARTA_PID,        // Test regulatora: Zadana -> PID -> Wyjście
        ZAMKNIETA_UAR       // Pełna pętla: PID + Model ze sprzężeniem
    };

private:
    // --- KOMPOZYCJA (Obiekty składowe) ---
    ModelARX m_model;
    RegulatorPID m_regulator;
    GeneratorWartosciZadanej m_generator;

    // --- STAN SYMULACJI ---
    TrybPracy m_trybPracy;
    bool m_czyDziala;
    double m_czas;

    // Flagi dostępności (czy użytkownik skonfigurował dany moduł)
    bool m_maModel;
    bool m_maRegulator;
    bool m_maGenerator;

    // --- ZMIENNE PROCESOWE ---
    double m_wartoscZadana;
    double m_wartoscWyjscie;
    double m_sterowanie;
    double m_uchyb;

    // Metoda pomocnicza ustalająca tryb na podstawie flag
    void aktualizujTrybPracy();

public:
    explicit Symulacja(QObject *parent = nullptr);

    // --- KONFIGURACJA (API) ---
    void konfigurujModel(const std::vector<double>& A, const std::vector<double>& B, int opoznienie);
    void konfigurujRegulator(double k, double ti, double td);
    void konfigurujGenerator(double ampl, double okres, int interwal,
                             GeneratorWartosciZadanej::TypSygnalu typ,
                             double skladowa, double wypelnienie);

    // --- STEROWANIE ---
    void uruchom();
    void zatrzymaj();
    void resetuj();
    void wykonajKrok();

    // --- GETTERY ---
    double getWartoscZadana() const { return m_wartoscZadana; }
    double getWartoscWyjscie() const { return m_wartoscWyjscie; }
    double getSterowanie() const { return m_sterowanie; }
    double getUchyb() const { return m_uchyb; }
    double getCzas() const { return m_czas; }

    // Interwał pobieramy zawsze z generatora (serce układu)
    int getInterwalMs() const { return m_generator.getInterwal(); }

    // Dostęp do obiektów (np. dla wykresów PID w MainWindow)
    const RegulatorPID& getRegulator() const { return m_regulator; }

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);


signals:
};

#endif // SYMULACJA_H
