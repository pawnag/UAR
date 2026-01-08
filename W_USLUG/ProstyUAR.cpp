#include "W_USLUG/ProstyUAR.h"
// Include ModelARX i RegulatorPID nie są tu konieczne, jeśli są w .h,
// ale mogą zostać dla pewności.

// <--- ZMIANA 4: Implementacja konstruktora
ProstyUAR::ProstyUAR(ModelARX& arx, RegulatorPID& regulator, QObject *parent)
    : QObject(parent),      // <--- ZMIANA 5: Wywołujemy konstruktor QObject
    m_arx(arx),
    m_regulator(regulator),
    m_poprzedniaWartoscRegulowana(0.0),
    m_ostatniUchyb(0.0),
    m_ostatnieSterowanie(0.0)
{
    // Usunęliśmy wszelkie odwołania do ui->setupUi, bo to nie jest okno.
}

double ProstyUAR::symuluj(double wartoscZadana)
{
    // Logika bez zmian...
    m_ostatniUchyb = wartoscZadana - m_poprzedniaWartoscRegulowana;
    m_ostatnieSterowanie = m_regulator.symuluj(m_ostatniUchyb);
    double wartoscRegulowana = m_arx.symuluj(m_ostatnieSterowanie);
    m_poprzedniaWartoscRegulowana = wartoscRegulowana;

    return wartoscRegulowana;
}

void ProstyUAR::reset()
{
    m_poprzedniaWartoscRegulowana = 0.0;
    m_ostatniUchyb = 0.0;
    m_ostatnieSterowanie = 0.0;
}
