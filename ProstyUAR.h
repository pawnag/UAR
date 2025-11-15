#include "ModelARX.h"
#include "RegulatorPID.h"
#pragma once


class ProstyUAR
{
private:
    ModelARX& m_arx;
    RegulatorPID& m_regulator;
    double m_poprzedniaWartoscRegulowana;

public:
    // Konstruktor przyjmuj¹cy referencje do obiektów ARX i regulatora PID
    ProstyUAR(ModelARX& arx, RegulatorPID& regulator);

    // Metoda symuluj¹ca jeden krok uk³adu regulacji
    double symuluj(double wartoscZadana);

    // Metoda resetuj¹ca stan uk³adu (dla testów)
    void reset();
};