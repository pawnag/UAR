#ifndef PROSTYUAR_H
#define PROSTYUAR_H

#include "ModelARX.h"
#include "RegulatorPID.h"

class ProstyUAR
{
private:
    ModelARX m_model;
    RegulatorPID m_regulator;

    bool m_trybOtwarty;
    double m_ostatniUchyb;
    double m_ostatnieSterowanie;
    double m_ostatnieWyjscie;
public:
    ProstyUAR(ModelARX wzorzecModelu = ModelARX(), RegulatorPID wzorzecRegulatora = RegulatorPID());

    double symuluj(double wartoscZadana);
    void reset();

    void aktualizujModel(const std::vector<double>& A, const std::vector<double>& B, int opoznienie, double szum);
    void setTrybOtwarty(bool tak);

    bool czyTrybOtwarty() const;
    ModelARX& getModel();
    RegulatorPID& getRegulator();
    const ModelARX& getModel() const;
    const RegulatorPID& getRegulator() const;
    double getOstatniUchyb() const;
    double getOstatnieSterowanie() const;
    double getOstatnieWyjscie() const;
};

#endif // PROSTYUAR_H
