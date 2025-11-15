#pragma once
#define _USE_MATH_DEFINES // dla M_PI
#include <cmath>

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
    double m_okres;  // w sekundach
    double m_skladowaStala;
    double m_wypelnienie;  // 0.0 - 1.0, tylko dla prostok¹ta
    double m_czas;         // aktualny czas symulacji w sekundach
    double m_interwal;     // interwa³ symulacji w sekundach
    //static constexpr double M_PI = 3.14159265358979323846;
public:
    GeneratorWartosciZadanej();

    void ustawTypSygnalu(TypSygnalu typ) { m_typ = typ; }
    void ustawAmplitude(double amplituda) { m_amplituda = amplituda; }
    void ustawOkres(double okres) { m_okres = okres; }
    void ustawSkladowaStala(double skladowa) { m_skladowaStala = skladowa; }
    void ustawWypelnienie(double wypelnienie) { m_wypelnienie = wypelnienie; }
    void ustawInterwal(double interwalMs) { m_interwal = interwalMs / 1000.0; } // konwersja ms na s

    double generuj();
    void reset() { m_czas = 0.0; }
    void aktualizujCzas() { m_czas += m_interwal; }

    // Akcesory
    TypSygnalu typSygnalu() const { return m_typ; }
    double amplituda() const { return m_amplituda; }
    double okres() const { return m_okres; }
    double skladowaStala() const { return m_skladowaStala; }
    double wypelnienie() const { return m_wypelnienie; }
};