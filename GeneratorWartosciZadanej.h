#pragma once
#define _USE_MATH_DEFINES // dla M_PI
#include <cmath>
#include <algorithm>

class GeneratorWartosciZadanej {
public:
    enum TypSygnalu {
        SYGNAL_PROSTOKATNY,
        SYGNAL_SINUSOIDALNY,
        SYGNAL_STALY
    };

private:
    TypSygnalu m_typ;
    double m_amplituda;
    int m_okresDyskretny; // T - liczba próbek na okres
    double m_skladowaStala;
    double m_wypelnienie;
    int m_licznikKrokow; // i - numer aktualnej próbki
    double m_okresRzeczywisty; // T_RZ - w sekundach
    double m_interwal; // T_T - w sekundach

public:
    GeneratorWartosciZadanej();

    void setTypSygnalu(TypSygnalu typ) { m_typ = typ; }
    void setAmplituda(double amplituda) { m_amplituda = amplituda; }
    void setSkladowaStala(double skladowa) { m_skladowaStala = skladowa; }
    void setWypelnienie(double wypelnienie)
    {
        if (wypelnienie < 0.0) m_wypelnienie = 0.0;
        else if (wypelnienie > 1.0) m_wypelnienie = 1.0;
        else m_wypelnienie = wypelnienie;
    }

    void setOkresRzeczywisty(double T_RZ);
    void setInterwal(double T_T_ms);
    void przeliczOkresDyskretny();

    double generuj();
    void krokSymulacji() { m_licznikKrokow++; }
    void reset() { m_licznikKrokow = 0; }

    TypSygnalu getTypSygnalu() const { return m_typ; }
    double getAmplituda() const { return m_amplituda; }
    double getOkresRzeczywisty() const { return m_okresRzeczywisty; }
    double getSkladowaStala() const { return m_skladowaStala; }
    double getWypelnienie() const { return m_wypelnienie; }
    int getOkresDyskretny() const { return m_okresDyskretny; }
    double getInterwal() const { return m_interwal; }
};