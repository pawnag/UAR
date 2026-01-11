#ifndef PROSTYUAR_H
#define PROSTYUAR_H

#include <QObject>
#include "../W_DANYCH/ModelARX.h"
#include "../W_DANYCH/RegulatorPID.h"

class ProstyUAR : public QObject
{
    Q_OBJECT

public:
    // Konstruktor uproszczony - nie potrzebuje już zewnętrznych obiektów
    explicit ProstyUAR(QObject *parent = nullptr);

    // DLA TESTÓW
    ProstyUAR(const ModelARX& arx, const RegulatorPID& regulator, QObject *parent = nullptr);

    // Główna pętla (bez zmian)
    double symuluj(double wartoscZadana);
    void reset();

    // --- DOSTĘP DO OBIEKTÓW WEWNĘTRZNYCH ---
    // Zwracamy referencje, aby Symulacja mogła zmieniać ich ustawienia (A, B, Kp, Ti...)
    ModelARX& getModel() { return m_arx; }
    const ModelARX& getModel() const { return m_arx; }

    RegulatorPID& getRegulator() { return m_regulator; }
    const RegulatorPID& getRegulator() const { return m_regulator; }

    // Gettery wyników (bez zmian)
    double getOstatniUchyb() const { return m_ostatniUchyb; }
    double getOstatnieSterowanie() const { return m_ostatnieSterowanie; }
    double getPoprzedniaWartoscRegulowana() const { return m_poprzedniaWartoscRegulowana; }

private:
    // TERAZ TO SĄ INSTANCJE (WŁASNOŚĆ), A NIE REFERENCJE
    ModelARX m_arx;
    RegulatorPID m_regulator;

    double m_poprzedniaWartoscRegulowana;
    double m_ostatniUchyb;
    double m_ostatnieSterowanie;
};

#endif // PROSTYUAR_H
