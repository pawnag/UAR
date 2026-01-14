#include "ProstyUAR.h" // Sprawdź ścieżkę

// 1. IMPLEMENTACJA KONSTRUKTORA DOMYŚLNEGO (Musi być!)
ProstyUAR::ProstyUAR()
    : m_model(),        // Tworzy domyślny model
    m_regulator(),    // Tworzy domyślny regulator
    m_trybOtwarty(true),
    m_ostatniUchyb(0.0),
    m_ostatnieSterowanie(0.0),
    m_ostatnieWyjscie(0.0)
{
}

// 2. IMPLEMENTACJA KONSTRUKTORA PARAMETRYCZNEGO
ProstyUAR::ProstyUAR(ModelARX wzorzecModelu, RegulatorPID wzorzecRegulatora)
    : m_model(wzorzecModelu),       // Kopiuje model
    m_regulator(wzorzecRegulatora), // Kopiuje regulator
    m_trybOtwarty(false),
    m_ostatniUchyb(0.0),
    m_ostatnieSterowanie(0.0),
    m_ostatnieWyjscie(0.0)
{
    reset(); // Resetuje historię po skopiowaniu
}

// ... Reszta metod (symuluj, reset itp.) bez zmian ...
double ProstyUAR::symuluj(double wartoscZadana)
{
    double uchyb = wartoscZadana - m_ostatnieWyjscie;
    double sterowanie;

    if (m_trybOtwarty) {
        sterowanie = wartoscZadana;
        m_regulator.resetuj();
    } else {
        sterowanie = m_regulator.symuluj(uchyb);
    }

    double noweWyjscie = m_model.symuluj(sterowanie);

    m_ostatniUchyb = uchyb;
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
    m_trybOtwarty = false;
}

void ProstyUAR::aktualizujModel(const std::vector<double>& A, const std::vector<double>& B, int opoznienie, double szum)
{
    m_model.aktualizuj(A, B, opoznienie, szum);
}

void ProstyUAR::setTrybOtwarty(bool tak) { m_trybOtwarty = tak; if(tak) m_regulator.resetuj(); }
bool ProstyUAR::czyTrybOtwarty() const { return m_trybOtwarty; }

ModelARX& ProstyUAR::getModel() { return m_model; }
RegulatorPID& ProstyUAR::getRegulator() { return m_regulator; }
const ModelARX& ProstyUAR::getModel() const { return m_model; }
const RegulatorPID& ProstyUAR::getRegulator() const { return m_regulator; }

double ProstyUAR::getOstatniUchyb() const { return m_ostatniUchyb; }
double ProstyUAR::getOstatnieSterowanie() const { return m_ostatnieSterowanie; }
double ProstyUAR::getOstatnieWyjscie() const { return m_ostatnieWyjscie; }
