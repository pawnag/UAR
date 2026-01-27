#include "ProstyUAR.h"

ProstyUAR::ProstyUAR(ModelARX wzorzecModelu, RegulatorPID wzorzecRegulatora)
    : m_model(wzorzecModelu),
    m_regulator(wzorzecRegulatora),
    m_ostatniUchyb(0.0),
    m_ostatnieSterowanie(0.0),
    m_ostatnieWyjscie(0.0)
{
    reset();
}

double ProstyUAR::symuluj(double wartoscZadana)
{
    double uchyb = wartoscZadana - m_ostatnieWyjscie;
    double sterowanie = m_regulator.symuluj(uchyb);

    // Jeśli regulator zwraca 0 (jest wyłączony lub ma zerowe nastawy)
    if (sterowanie == 0.0)
    {
        // ZMIANA: Usunięto przypisanie 'sterowanie = wartoscZadana'.
        // Dzięki temu sterowanie pozostaje 0.0, więc na model wchodzi 0.

        // Resetujemy pamięć regulatora, aby uniknąć błędów integracji (windup)
        // przy ponownym włączeniu, skoro traktujemy to jako stan "wyłączony".
        m_regulator.resetuj();
        m_ostatniUchyb = 0.0;
    }
    else
    {
        m_ostatniUchyb = uchyb;
    }

    // Teraz, jeśli sterowanie wynosi 0.0, do modelu trafi 0.0.
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

void ProstyUAR::konfigurujModel(const std::vector<double>& A, const std::vector<double>& B,
                                int opoznienie, double szum,
                                double umin, double umax, double ymin, double ymax)
{
    m_model.setA(A);
    m_model.setB(B);
    m_model.setOpoznienieTransportowe(opoznienie);
    m_model.setOdchylenieStandardoweSzumu(szum);
    m_model.setOgraniczeniaSterowania(umin, umax);
    m_model.setOgraniczeniaWyjscia(ymin, ymax);
}

ModelARX ProstyUAR::pobierzModel() const
{
    return m_model;
}

void ProstyUAR::konfigurujRegulator(double k, double ti, double td, int metodaCalkowania)
{
    m_regulator.setWzmocnienie(k);
    m_regulator.setStalaCalk(ti);
    m_regulator.setStalaRozn(td);
    m_regulator.setLiczCalk(static_cast<RegulatorPID::LiczCalk>(metodaCalkowania));
}

RegulatorPID ProstyUAR::pobierzRegulator() const
{
    return m_regulator;
}

double ProstyUAR::getOstatniUchyb() const { return m_ostatniUchyb; }
double ProstyUAR::getOstatnieSterowanie() const { return m_ostatnieSterowanie; }
double ProstyUAR::getOstatnieWyjscie() const { return m_ostatnieWyjscie; }
