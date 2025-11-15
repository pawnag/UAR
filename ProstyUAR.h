#pragma once
#include "ModelARX.h"
#include "RegulatorPID.h"

class ProstyUAR
{
private:
    ModelARX& m_arx;
    RegulatorPID& m_regulator;
    double m_poprzedniaWartoscRegulowana;
    double m_ostatniUchyb;
    double m_ostatnieSterowanie;

public:
    // Konstruktor przyjmuj¹cy referencje do obiektów ARX i regulatora PID
    ProstyUAR(ModelARX& arx, RegulatorPID& regulator);

    // Metoda symuluj¹ca jeden krok uk³adu regulacji
    double symuluj(double wartoscZadana);

    // Metoda resetuj¹ca stan uk³adu (dla testów)
    void reset();

    // Akcesory do odczytu stanu - DODANE!
    double ostatniUchyb() const { return m_ostatniUchyb; }
    double ostatnieSterowanie() const { return m_ostatnieSterowanie; }
    double poprzedniaWartoscRegulowana() const { return m_poprzedniaWartoscRegulowana; }
};