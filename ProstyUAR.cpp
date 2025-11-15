#include "ProstyUAR.h"

ProstyUAR::ProstyUAR(ModelARX& arx, RegulatorPID& regulator)
    : m_arx(arx), m_regulator(regulator), m_poprzedniaWartoscRegulowana(0.0)
{
}

double ProstyUAR::symuluj(double wartoscZadana)
{
    // Obliczenie uchybu: wartoœæ zadana - poprzednia wartoœæ regulowana
    double uchyb = wartoscZadana - m_poprzedniaWartoscRegulowana;

    // Obliczenie sterowania przez regulator PID
    double sterowanie = m_regulator.symuluj(uchyb);

    // Symulacja obiektu ARX z obliczonym sterowaniem
    double wartoscRegulowana = m_arx.symuluj(sterowanie);

    // Zapamiêtanie wartoœci regulowanej dla nastêpnego kroku
    m_poprzedniaWartoscRegulowana = wartoscRegulowana;

    return wartoscRegulowana;
}

void ProstyUAR::reset()
{
    m_poprzedniaWartoscRegulowana = 0.0;
    // Tutaj mo¿na dodaæ resetowanie stanu regulatora i modelu ARX jeœli potrzeba
}