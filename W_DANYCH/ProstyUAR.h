#ifndef PROSTYUAR_H
#define PROSTYUAR_H

#include "ModelARX.h"
#include "RegulatorPID.h"
#include <vector>

class ProstyUAR
{
private:
    ModelARX m_model;
    RegulatorPID m_regulator;

    double m_ostatniUchyb = 0.0;
    double m_ostatnieSterowanie = 0.0;
    double m_ostatnieWyjscie = 0.0;

public:
    ProstyUAR(ModelARX wzorzecModelu = ModelARX(), RegulatorPID wzorzecRegulatora = RegulatorPID());

    double symuluj(double wartoscZadana);
    void reset();

    void konfigurujModel(const std::vector<double>& A, const std::vector<double>& B,
                         int opoznienie = 1, double szum = 0.0,
                         double umin = -10, double umax = 10, double ymin = -10, double ymax = 10);
    ModelARX pobierzModel() const;

    void konfigurujRegulator(double k, double ti, double td, int metodaCalkowania);
    RegulatorPID pobierzRegulator() const;

    double getOstatniUchyb() const;
    double getOstatnieSterowanie() const;
    double getOstatnieWyjscie() const;
};

#endif // PROSTYUAR_H
