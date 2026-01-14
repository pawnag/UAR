#ifndef GENERATORWARTOSCIZADANEJ_H
#define GENERATORWARTOSCIZADANEJ_H

class GeneratorWartosciZadanej
{
public:
    enum TypSygnalu {
        SYGNAL_STALY,
        SYGNAL_PROSTOKATNY,
        SYGNAL_SINUSOIDALNY
    };

private:
    TypSygnalu m_typ;
    double m_amplituda;
    double m_skladowaStala;
    double m_wypelnienie;
    double m_okresRzeczywisty; // T_RZ - okres rzeczywisty [s]
    int m_licznikKrokow;       // i - numer aktualnej próbki
    int m_interwal;            // T_T - taktowanie symulacji [ms]
    int m_okresDyskretny;      // T - liczba próbek na okres

public:
    GeneratorWartosciZadanej();

    double generuj();
    void krokSymulacji();
    void reset();

    void setTypSygnalu(TypSygnalu typ);
    void setAmplituda(double amplituda);
    void setSkladowaStala(double skladowa);
    void setWypelnienie(double wypelnienie);
    void setOkresRzeczywisty(double T_RZ);
    void setInterwal(int T_T_ms);
    void przeliczOkresDyskretny();

    TypSygnalu getTypSygnalu() const;
    double getAmplituda() const;
    double getOkresRzeczywisty() const;
    double getSkladowaStala() const;
    double getWypelnienie() const;
    int getOkresDyskretny() const;
    int getInterwal() const;

};

#endif // GENERATORWARTOSCIZADANEJ_H
