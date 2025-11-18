#pragma once
#include "ModelARX.h"
#include "RegulatorPID.h"
#include "GeneratorWartosciZadanej.h"

class Symulacja
{
private:
    std::shared_ptr<ModelARX> m_model;
    std::shared_ptr<RegulatorPID> m_regulator;
    std::shared_ptr<GeneratorWartosciZadanej> m_generator;

    bool m_czyDziala;
    double m_czas;

    double m_wartoscZadana;
    double m_wartoscWyjscie;
    double m_sterowanie;
    double m_uchyb;
public:
    Symulacja();

    void setModel(std::shared_ptr<ModelARX> i_model);
    void setRegulator(std::shared_ptr<RegulatorPID> i_regulator);
    void setGenerator(std::shared_ptr<GeneratorWartosciZadanej> i_generator);

    void uruchom();
    void zatrzymaj();
    void resetuj();

    double getWartoscZadana() const { return m_wartoscZadana; }
    double getWartoscWyjscie() const { return m_wartoscWyjscie; }
    double getSterowanie() const { return m_sterowanie; }
    double getUchyb() const { return m_uchyb; }
    double getCzas() const { return m_czas; }
    int getInterwalMs() const;

    // Przyk³adowe konfiguracje
    void tylkoGenerator();
    void generatorIModel();
    void pelnyUAR();
    void wykonajKrok();
};