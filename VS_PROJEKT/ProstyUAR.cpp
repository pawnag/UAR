#include "ProstyUAR.h"

ProstyUAR::ProstyUAR(ModelARX& arx, RegulatorPID& regulator)
    : m_arx(arx), m_regulator(regulator),
    m_poprzedniaWartoscRegulowana(0.0),
    m_ostatniUchyb(0.0), m_ostatnieSterowanie(0.0)
{
}

double ProstyUAR::symuluj(double wartoscZadana)
{
    // Obliczenie uchybu: wartoœæ zadana - poprzednia wartoœæ regulowana
    m_ostatniUchyb = wartoscZadana - m_poprzedniaWartoscRegulowana;

    // Obliczenie sterowania przez regulator PID
    m_ostatnieSterowanie = m_regulator.symuluj(m_ostatniUchyb);

    // Symulacja obiektu ARX z obliczonym sterowaniem
    double wartoscRegulowana = m_arx.symuluj(m_ostatnieSterowanie);

    // Zapamiêtanie wartoœci regulowanej dla nastêpnego kroku
    m_poprzedniaWartoscRegulowana = wartoscRegulowana;

    return wartoscRegulowana;
}

void ProstyUAR::reset()
{
    m_poprzedniaWartoscRegulowana = 0.0;
    m_ostatniUchyb = 0.0;
    m_ostatnieSterowanie = 0.0;
}