#include "GeneratorWartosciZadanej.h"

GeneratorWartosciZadanej::GeneratorWartosciZadanej()
    : m_typ(SYGNAL_STALY),
    m_amplituda(1.0),
    m_okresDyskretny(5),
    m_skladowaStala(0.0),
    m_wypelnienie(0.5),
    m_licznikKrokow(0),
    m_okresRzeczywisty(1.0),
    m_interwal(0.2) // 200ms = 0.2s
{ }

void GeneratorWartosciZadanej::setOkresRzeczywisty(double T_RZ)
{
    m_okresRzeczywisty = T_RZ;
    przeliczOkresDyskretny();
}

void GeneratorWartosciZadanej::setInterwal(double T_T_ms)
{
    m_interwal = T_T_ms / 1000.0;
    przeliczOkresDyskretny();
}

void GeneratorWartosciZadanej::przeliczOkresDyskretny()
{
    if (m_interwal > 0 && m_okresRzeczywisty > 0)
    {
        double T = m_okresRzeczywisty / m_interwal;
        m_okresDyskretny = static_cast<int>(std::round(T));
        m_okresDyskretny = std::max(1, m_okresDyskretny);
    }
    else
    {
        m_okresDyskretny = 1;
    }
}

double GeneratorWartosciZadanej::generuj()
{
    switch (m_typ)
    {
    case SYGNAL_SINUSOIDALNY:
    {
        if (m_okresDyskretny <= 0) return m_skladowaStala;
        // Używamy wzoru z wymagań: w_i = A*sin((i mod T)/T * 2π) + S
        double faza = (m_licznikKrokow % m_okresDyskretny) / static_cast<double>(m_okresDyskretny) * 2.0 * M_PI;
        return m_amplituda * std::sin(faza) + m_skladowaStala;
    }

    case SYGNAL_PROSTOKATNY:
    {
        if (m_okresDyskretny <= 0) return m_skladowaStala;
        // Używamy wzoru z wymagań: w_i = A+S gdy (i mod T) < p*T, w przeciwnym razie S
        int pozycjaWOkresie = m_licznikKrokow % m_okresDyskretny;
        double prog = m_wypelnienie * m_okresDyskretny;
        return (pozycjaWOkresie < prog) ? (m_amplituda + m_skladowaStala) : m_skladowaStala;
    }

    case SYGNAL_STALY:
    default:
        return m_skladowaStala;
    }
}