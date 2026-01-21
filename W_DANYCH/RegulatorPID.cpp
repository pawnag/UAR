#include "RegulatorPID.h"
#include <stdexcept>

RegulatorPID::RegulatorPID(double k, double TI, double TD)
    :
    m_k(k),
    m_TI(TI),
    m_TD(TD),
    m_trybCalk(LiczCalk::Zew),
    m_calka(0.0),
    m_e_prev(0.0)
{}

double RegulatorPID::symuluj(double e)
{
    m_lastP = m_k * e;

    m_lastI = 0.0;
    if (m_TI > 0.0)
    {
        if (m_trybCalk == LiczCalk::Zew)
        {
            m_calka += e;
            m_lastI = (1.0 / m_TI) * m_calka;
        }
        else
        {
            m_calka += e / m_TI;
            m_lastI = m_calka;
        }
    }

    m_lastD = 0.0;
    if (m_TD > 0.0)
    {
        m_lastD = m_TD * (e - m_e_prev);
    }

    m_e_prev = e;

    return m_lastP + m_lastI + m_lastD;
}

void RegulatorPID::resetuj()
{
    m_calka = 0.0;
    m_e_prev = 0.0;

    m_lastI = 0.0;
}

void RegulatorPID::setLiczCalk(LiczCalk tryb)
{
    if (tryb == m_trybCalk) return;

    if (tryb == LiczCalk::Wew)
    {
        if (m_TI > 0.0)
        {
            m_calka /= m_TI;
        }
    }
    else
    {
        if (m_TI > 0.0)
        {
            m_calka *= m_TI;
        }
    }
    m_trybCalk = tryb;
}
void RegulatorPID::setWzmocnienie(double k) { m_k = std::max(0.0, k); }
void RegulatorPID::setStalaCalk(double TI)  { m_TI = std::max(0.0, TI); }
void RegulatorPID::setStalaRozn(double TD)  { m_TD = std::max(0.0, TD); }

RegulatorPID::LiczCalk RegulatorPID::getLiczCalk() const { return m_trybCalk; }
double RegulatorPID::getWzmocnienie() const { return m_k; }
double RegulatorPID::getStalaCalk() const { return m_TI; }
double RegulatorPID::getStalaRozn() const { return m_TD; }
double RegulatorPID::getLastP() const { return m_lastP; }
double RegulatorPID::getLastI() const { return m_lastI; }
double RegulatorPID::getLastD() const { return m_lastD; }
