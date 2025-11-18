#pragma once
#include "KlasaUslugowa.h"
#include "Symulacja.h"

class KlasaUslugowa
{
private:
    Symulacja m_symulacja;

public:
    KlasaUslugowa() = default;

    // Konfiguracja komponentów
    void nowyGenerator(double amplituda, double okres, int interwalMs, GeneratorWartosciZadanej::TypSygnalu typ, double skladowa = 0.0, double wypelnienie = 0.5);

    void nowyModelARX(const std::vector<double>& A, const std::vector<double>& B, int opoznienie = 1, double szum = 0.0);

    void nowyRegulator(double k, double TI, double TD);

    // Sterowanie symulacj¹
    void start();
    void stop();
    void reset();

    // Dostêp do wyników
    double getCzas() const;
    double getWartoscZadana() const;
    double getWartoscWyjscie() const;
    double getSterowanie() const;
    double getUchyb() const;
};

