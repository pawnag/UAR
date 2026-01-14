#include "ProstyUAR.h" // Sprawdź ścieżkę

ProstyUAR::ProstyUAR(ModelARX wzorzecModelu, RegulatorPID wzorzecRegulatora)
    : m_model(wzorzecModelu),       // Kopiuje model
    m_regulator(wzorzecRegulatora), // Kopiuje regulator
    m_ostatniUchyb(0.0),
    m_ostatnieSterowanie(0.0),
    m_ostatnieWyjscie(0.0)
{
    reset(); // Resetuje historię po skopiowaniu
}

double ProstyUAR::symuluj(double wartoscZadana)
{
    double uchyb = wartoscZadana - m_ostatnieWyjscie;
    double sterowanie = m_regulator.symuluj(uchyb);
    if (sterowanie == 0.0)
    {
        sterowanie = wartoscZadana;
        m_regulator.resetuj();
        m_ostatniUchyb = 0.0;
    }
    else
    {
        m_ostatniUchyb = uchyb;
    }
    double noweWyjscie = m_model.symuluj(sterowanie);
    m_ostatnieSterowanie = sterowanie;
    m_ostatnieWyjscie = noweWyjscie;
    return noweWyjscie;
}

void ProstyUAR::reset()
{
    m_model.resetuj();
    m_regulator.resetuj();
    m_ostatniUchyb = 0.0;
    m_ostatnieSterowanie = 0.0;
    m_ostatnieWyjscie = 0.0;
}

void ProstyUAR::aktualizujModel(const std::vector<double>& A, const std::vector<double>& B, int opoznienie, double szum)
{
    m_model.aktualizuj(A, B, opoznienie, szum);
}

ModelARX& ProstyUAR::getModel() { return m_model; }
RegulatorPID& ProstyUAR::getRegulator() { return m_regulator; }
const ModelARX& ProstyUAR::getModel() const { return m_model; }
const RegulatorPID& ProstyUAR::getRegulator() const { return m_regulator; }

double ProstyUAR::getOstatniUchyb() const { return m_ostatniUchyb; }
double ProstyUAR::getOstatnieSterowanie() const { return m_ostatnieSterowanie; }
double ProstyUAR::getOstatnieWyjscie() const { return m_ostatnieWyjscie; }
