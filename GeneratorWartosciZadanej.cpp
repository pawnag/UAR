#include "GeneratorWartosciZadanej.h"

GeneratorWartosciZadanej::GeneratorWartosciZadanej()
    : m_typ(SYGNAL_STALY), m_amplituda(1.0), m_okres(1.0),
    m_skladowaStala(0.0), m_wypelnienie(0.5), m_czas(0.0), m_interwal(0.2)
{
}

double GeneratorWartosciZadanej::generuj()
{
    switch (m_typ) {
    case SYGNAL_SINUSOIDALNY: {
        if (m_okres <= 0.0) return m_skladowaStala;
        double faza = 2.0 * M_PI * m_czas / m_okres;
        return m_amplituda * std::sin(faza) + m_skladowaStala;
    }

    case SYGNAL_PROSTOKATNY: {
        if (m_okres <= 0.0) return m_skladowaStala;
        double faza = std::fmod(m_czas, m_okres) / m_okres;
        return (faza < m_wypelnienie) ? (m_amplituda + m_skladowaStala) : m_skladowaStala;
    }

    case SYGNAL_STALY:
    default:
        return m_skladowaStala;
    }
}