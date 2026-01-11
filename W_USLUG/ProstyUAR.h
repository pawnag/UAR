#ifndef PROSTYUAR_H
#define PROSTYUAR_H

#include <QObject>
#include "W_DANYCH/ModelARX.h"
#include "W_DANYCH/RegulatorPID.h"

class ProstyUAR : public QObject
{
    Q_OBJECT

public:
    explicit ProstyUAR(QObject *parent = nullptr);

    // Konstruktor dla testów (kopiujący parametry)
    ProstyUAR(const ModelARX& model, const RegulatorPID& reg, QObject *parent = nullptr);

    // --- Główna metoda symulacji ---
    double symuluj(double wartoscZadana);

    // --- Sterowanie stanem ---
    void reset();
    void setTrybOtwarty(bool tak);
    bool czyTrybOtwarty() const { return m_trybOtwarty; }

    // --- DOSTĘP DO PODZESPOŁÓW (POPRAWKA: Wersje zwykłe i const) ---

    // 1. Wersje do modyfikacji (np. zmiana nastaw w GUI)
    ModelARX& getModel() { return m_model; }
    RegulatorPID& getRegulator() { return m_regulator; }

    // 2. Wersje do odczytu (dla toJson, wykresów i obiektów const) - TO NAPRAWIA C2662
    const ModelARX& getModel() const { return m_model; }
    const RegulatorPID& getRegulator() const { return m_regulator; }

    // --- DOSTĘP DO WYNIKÓW POŚREDNICH (POPRAWKA: Brakujące gettery) ---
    double getOstatniUchyb() const { return m_ostatniUchyb; }
    double getOstatnieSterowanie() const { return m_ostatnieSterowanie; }
    double getOstatnieWyjscie() const { return m_ostatnieWyjscie; }

private:
    // Podzespoły
    ModelARX m_model;
    RegulatorPID m_regulator;

    // Stan wewnętrzny
    bool m_trybOtwarty;
    double m_ostatniUchyb;       // Przechowuje e(k)
    double m_ostatnieSterowanie; // Przechowuje u(k)
    double m_ostatnieWyjscie;    // Przechowuje y(k)
};

#endif // PROSTYUAR_H
