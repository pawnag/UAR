#ifndef SYMULACJA_H
#define SYMULACJA_H

#include <QObject>
#include "../W_DANYCH/GeneratorWartosciZadanej.h"
#include "ProstyUAR.h"

class Symulacja : public QObject
{
    Q_OBJECT

public:
    explicit Symulacja(QObject *parent = nullptr);

    // --- Konfiguracja ---
    void konfigurujModel(const std::vector<double>& A, const std::vector<double>& B, int opoznienie);
    void konfigurujRegulator(double k, double ti, double td);
    void konfigurujGenerator(double ampl, double okres, int interwal,
                             GeneratorWartosciZadanej::TypSygnalu typ,
                             double skladowa, double wypelnienie);

    // --- Sterowanie ---
    void uruchom();
    void zatrzymaj();
    void resetuj();
    void wykonajKrok();

    // --- Gettery ---
    // Te metody teraz muszą "sięgać" głębiej do ProstyUAR
    std::vector<double> getModelA() const { return m_prostyUAR.getModel().getA(); }
    std::vector<double> getModelB() const { return m_prostyUAR.getModel().getB(); }
    int getModelOpoznienie() const { return m_prostyUAR.getModel().getOpoznienieTransportowe(); }
    double getModelSzum() const { return m_prostyUAR.getModel().getOdchylenieStandardoweSzumu(); }

    // Dane procesowe
    double getWartoscZadana() const { return m_wartoscZadana; }
    double getWartoscWyjscie() const { return m_wartoscWyjscie; }
    double getSterowanie() const { return m_sterowanie; }
    double getUchyb() const { return m_uchyb; }

    double getCzas() const { return m_czas; }
    bool czyDziala() const { return m_czyDziala; }
    int getInterwalMs() const { return m_generator.getInterwal(); }

    //
    GeneratorWartosciZadanej& getGenerator() { return m_generator; }
    const GeneratorWartosciZadanej& getGenerator() const { return m_generator; }

    ModelARX& getModel() { return m_prostyUAR.getModel(); }
    const ModelARX& getModel() const { return m_prostyUAR.getModel(); }

    RegulatorPID& getRegulator() { return m_prostyUAR.getRegulator(); }
    const RegulatorPID& getRegulator() const { return m_prostyUAR.getRegulator(); }

    void resetUAR();

private:
    GeneratorWartosciZadanej m_generator;
    ProstyUAR m_prostyUAR;

    // Stan
    bool m_czyDziala;
    double m_czas;

    // Bufory do wizualizacji
    double m_wartoscZadana;
    double m_wartoscWyjscie;
    double m_sterowanie;
    double m_uchyb;
};

#endif // SYMULACJA_H
