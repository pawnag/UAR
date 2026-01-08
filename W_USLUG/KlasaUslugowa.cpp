#include "W_USLUG/KlasaUslugowa.h"

KlasaUslugowa::KlasaUslugowa(QWidget *parent)
    : QMainWindow{parent}
{}

void KlasaUslugowa::nowyGenerator(double amplituda,
                                  double okres,
                                  int interwalMs,
                                  GeneratorWartosciZadanej::TypSygnalu typ,
                                  double skladowa,
                                  double wypelnienie)
{
    auto gen = std::make_shared<GeneratorWartosciZadanej>();
    gen->setTypSygnalu(typ);
    gen->setAmplituda(amplituda);
    gen->setOkresRzeczywisty(okres);
    gen->setInterwal(interwalMs);
    gen->setSkladowaStala(skladowa);
    gen->setWypelnienie(wypelnienie);
    m_symulacja.setGenerator(gen);
}

void KlasaUslugowa::nowyModelARX(const std::vector<double>& A,
                                 const std::vector<double>& B,
                                 int opoznienie,
                                 double szum)
{
    auto model = std::make_shared<ModelARX>(A, B, opoznienie, szum);
    m_symulacja.setModel(model);
}

void KlasaUslugowa::nowyRegulator(double k, double TI, double TD)
{
    auto reg = std::make_shared<RegulatorPID>(k, TI, TD);
    m_symulacja.setRegulator(reg);
}

void KlasaUslugowa::start() { m_symulacja.uruchom(); }
void KlasaUslugowa::stop() { m_symulacja.zatrzymaj(); }
void KlasaUslugowa::reset() { m_symulacja.resetuj(); }

double KlasaUslugowa::getCzas() const { return m_symulacja.getCzas(); }
double KlasaUslugowa::getWartoscZadana() const { return m_symulacja.getWartoscZadana(); }
double KlasaUslugowa::getWartoscWyjscie() const { return m_symulacja.getWartoscWyjscie(); }
double KlasaUslugowa::getSterowanie() const { return m_symulacja.getSterowanie(); }
double KlasaUslugowa::getUchyb() const { return m_symulacja.getUchyb(); }
