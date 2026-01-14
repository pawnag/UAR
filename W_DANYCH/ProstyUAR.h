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

    // Helper kopiujący ustawienia (przyjmuje referencje)
    // UWAGA: Skoro w .cpp usunęliśmy tę metodę na rzecz listy inicjalizacyjnej,
    // to tutaj też powinna zniknąć. Ale jeśli chcesz ją trzymać, musi mieć ciało.
    // Sugeruję usunąć, jeśli używasz mojego kodu z poprzedniego kroku.
    // void kopiujUstawienia(const ModelARX& model, const RegulatorPID& reg);

public:
   // 1. Konstruktor domyślny (ODKOMENTOWANY!)
    ProstyUAR();

    // 2. Konstruktor parametryczny
    ProstyUAR(ModelARX wzorzecModelu, RegulatorPID wzorzecRegulatora);

    double symuluj(double wartoscZadana);
    void reset();

    // Dostęp do podzespołów
    ModelARX& getModel();
    RegulatorPID& getRegulator();
    const ModelARX& getModel() const;
    const RegulatorPID& getRegulator() const;

    void aktualizujModel(const std::vector<double>& A, const std::vector<double>& B, int opoznienie, double szum);

    // Settery/Gettery
    void setTrybOtwarty(bool tak);
    bool czyTrybOtwarty() const;
    double getOstatniUchyb() const;
    double getOstatnieSterowanie() const;
    double getOstatnieWyjscie() const;
};

#endif // PROSTYUAR_H
