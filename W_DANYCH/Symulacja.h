#ifndef SYMULACJA_H
#define SYMULACJA_H

#include <vector>
#include "GeneratorWartosciZadanej.h"
#include "ProstyUAR.h"

class Symulacja
{
private:
    GeneratorWartosciZadanej m_generator;
    ProstyUAR m_prostyUAR;

    // Stan symulacji
    bool m_czyDziala;
    double m_czas;

    // Bufory chwilowe
    double m_wartoscZadana;
    double m_wartoscWyjscie;
    double m_sterowanie;
    double m_uchyb;

public:
    Symulacja(GeneratorWartosciZadanej i_generator = GeneratorWartosciZadanej(),
              ProstyUAR i_prostyUAR = ProstyUAR());

    // --- Konfiguracja (Delegacja do obiektów wewnętrznych) ---
    void konfigurujModel(const std::vector<double>& A, const std::vector<double>& B, int opoznienie, double szum = 0.0);

    // Konfiguracja PID
    void konfigurujRegulator(double k, double ti, double td);
    void konfigurujMetodePID(int indeksMetody);


    // Konfiguracja Generatora
    void konfigurujGenerator(double ampl, double okres, int interwal,
                             GeneratorWartosciZadanej::TypSygnalu typ,
                             double skladowa, double wypelnienie);

    // --- Sterowanie symulacją ---
    void uruchom();
    void zatrzymaj();
    void resetuj();       // Reset wszystkiego (czas, gener, uar)
    void wykonajKrok();
    void resetUAR();      // Reset tylko UAR (np. po zmianie nastaw)

    // --- Gettery Danych (Fasada dla ProstyUAR) ---
    std::vector<double> getModelA() const;
    std::vector<double> getModelB() const;
    int getModelOpoznienie() const;
    double getModelSzum() const;

    double getModelUMIN() const;
    double getModelUMAX() const;
    double getModelYMIN() const;
    double getModelYMAX() const;

    // Dane procesowe (Wyniki symulacji)
    double getWartoscZadana() const;
    double getWartoscWyjscie() const;
    double getSterowanie() const;
    double getUchyb() const;

    double getCzas() const;
    bool czyDziala() const;
    int getInterwalMs() const; // Z generatora

    // --- DOSTĘP DO OBIEKTÓW (Zwracamy KOPIE - bezpieczna kompozycja) ---
    // Zastępuje stare getModel() i getRegulator() zwracające referencje

    GeneratorWartosciZadanej pobierzGenerator() const;
    ModelARX pobierzModel() const;
    RegulatorPID pobierzRegulator() const;
};

#endif // SYMULACJA_H
