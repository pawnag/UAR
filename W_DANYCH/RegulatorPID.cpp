#include "W_DANYCH/RegulatorPID.h" // Upewnij się, że ten include działa (ew. "W_DANYCH/RegulatorPID.h")
#include <stdexcept>

// ZMIANA: Implementacja jednego, scalonego konstruktora
RegulatorPID::RegulatorPID(double k, double TI, double TD, QObject *parent)
    : QObject(parent), // Inicjalizacja QObject
    m_k(k),
    m_TI(TI),
    m_TD(TD),
    m_trybCalk(LiczCalk::Zew),
    m_calka(0.0),
    m_e_prev(0.0)
{
    if (k < 0.0)
    {
        throw std::invalid_argument("Wzmocnienie musi być nieujemne");
    }
    if (TI < 0.0)
    {
        throw std::invalid_argument("Stała całkowania musi być nieujemna");
    }
    if (TD < 0.0)
    {
        throw std::invalid_argument("Stała różniczkowania musi być nieujemna");
    }
}

double RegulatorPID::symuluj(double e)
{
    // --- P ---
    double u_P = m_k * e;

    // --- I ---
    double u_I = 0.0;
    if (m_TI > 0.0)
    {
        if (m_trybCalk == LiczCalk::Zew)
        {
            m_calka += e;
            u_I = (1.0 / m_TI) * m_calka;
        }
        else
        {
            m_calka += e / m_TI;
            u_I = m_calka;
        }
    }

    // --- D ---
    double u_D = 0.0;
    if (m_TD > 0.0)
    {
        u_D = m_TD * (e - m_e_prev);
    }

    // Zapisujemy e do następnego kroku
    m_e_prev = e;

    // --- ZAPIS DO ZMIENNYCH KLASOWYCH (dla GUI) ---
    m_lastP = u_P;
    m_lastI = u_I;
    m_lastD = u_D;

    // --- Zwracamy sumę (dla testów i regulatora) ---
    return u_P + u_I + u_D;
}

void RegulatorPID::resetuj()
{
    m_calka = 0.0;
    m_e_prev = 0.0;
}

void RegulatorPID::setWzmocnienie(double k)
{
    if (k < 0.0)
    {
        throw std::invalid_argument("Wzmocnienie musi być nieujemne");
    }
    m_k = k;
}

void RegulatorPID::setStalaCalk(double TI)
{
    if (TI < 0.0)
    {
        throw std::invalid_argument("Stała całkowania musi być nieujemna");
    }
    m_TI = TI;
}

void RegulatorPID::setStalaRozn(double TD)
{
    if (TD < 0.0)
    {
        throw std::invalid_argument("Stała różniczkowania musi być nieujemna");
    }
    m_TD = TD;
}

void RegulatorPID::setLiczCalk(LiczCalk tryb)
{
    if (tryb == m_trybCalk) return;

    // Przeliczenie całki przy zmianie trybu
    if (tryb == LiczCalk::Wew)
    {
        // Zewnętrzny -> Wewnętrzny: całka = całka / TI
        if (m_TI > 0.0)
        {
            m_calka /= m_TI;
        }
    }
    else
    {
        // Wewnętrzny -> Zewnętrzny: całka = całka * TI
        if (m_TI > 0.0)
        {
            m_calka *= m_TI;
        }
    }
    m_trybCalk = tryb;
}
