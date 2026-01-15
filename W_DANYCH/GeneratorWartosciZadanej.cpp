#include "W_DANYCH/GeneratorWartosciZadanej.h"
#include "qmath.h"
#include <cmath>
#include <algorithm>

GeneratorWartosciZadanej::GeneratorWartosciZadanej() :
    m_typ(SYGNAL_STALY),
    m_amplituda(1.0),
    m_skladowaStala(0.0),
    m_wypelnienie(0.5),
    m_okresRzeczywisty(10.0),
    m_licznikKrokow(0),
    m_interwal(200),
    m_okresDyskretny(1)
{
    przeliczOkresDyskretny();
}

void GeneratorWartosciZadanej::przeliczOkresDyskretny() {
    if (m_interwal <= 0) m_interwal = 100;

    double interwalSekundy = m_interwal / 1000.0;
    m_okresDyskretny = static_cast<int>(std::round(m_okresRzeczywisty / interwalSekundy));
    if (m_okresDyskretny < 1) m_okresDyskretny = 1;
}

double GeneratorWartosciZadanej::generuj()
{
    if (m_typ == SYGNAL_STALY) return m_skladowaStala;

    int pozycja = m_licznikKrokow % m_okresDyskretny;

    if (m_typ == SYGNAL_SINUSOIDALNY) {
        double postep = static_cast<double>(pozycja) / m_okresDyskretny;
        return m_amplituda * std::sin(postep * 2.0 * M_PI) + m_skladowaStala;
    }

    if (m_typ == SYGNAL_PROSTOKATNY) {
        double prog = m_wypelnienie * m_okresDyskretny;
        return (pozycja < prog) ? (m_amplituda + m_skladowaStala) : m_skladowaStala;
    }

    return m_skladowaStala;
}

void GeneratorWartosciZadanej::krokSymulacji() { m_licznikKrokow++; }
void GeneratorWartosciZadanej::reset() { m_licznikKrokow = 0; }

void GeneratorWartosciZadanej::setTypSygnalu(TypSygnalu typ) { m_typ = typ; }
void GeneratorWartosciZadanej::setAmplituda(double a) { m_amplituda = a; }
void GeneratorWartosciZadanej::setSkladowaStala(double s) { m_skladowaStala = s; }
void GeneratorWartosciZadanej::setWypelnienie(double w) { m_wypelnienie = std::max(0.0, std::min(1.0, w)); }
void GeneratorWartosciZadanej::setOkresRzeczywisty(double t) { m_okresRzeczywisty = t; przeliczOkresDyskretny(); }
void GeneratorWartosciZadanej::setInterwal(int t_ms) { m_interwal = t_ms; przeliczOkresDyskretny(); }

GeneratorWartosciZadanej::TypSygnalu GeneratorWartosciZadanej::getTypSygnalu() const { return m_typ; }
double GeneratorWartosciZadanej::getAmplituda() const { return m_amplituda; }
double GeneratorWartosciZadanej::getOkresRzeczywisty() const { return m_okresRzeczywisty; }
double GeneratorWartosciZadanej::getSkladowaStala() const { return m_skladowaStala; }
double GeneratorWartosciZadanej::getWypelnienie() const { return m_wypelnienie; }
int GeneratorWartosciZadanej::getOkresDyskretny() const { return m_okresDyskretny; }
int GeneratorWartosciZadanej::getInterwal() const { return m_interwal; }
