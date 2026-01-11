#include "W_USLUG/KlasaUslugowa.h"

KlasaUslugowa::KlasaUslugowa(QObject *parent)
    : QObject(parent)
{
}

void KlasaUslugowa::nowyGenerator(double amplituda,
                                  double okres,
                                  int interwalMs,
                                  GeneratorWartosciZadanej::TypSygnalu typ,
                                  double skladowa,
                                  double wypelnienie)
{
    m_symulacja.konfigurujGenerator(amplituda, okres, interwalMs, typ, skladowa, wypelnienie);
}

void KlasaUslugowa::nowyModelARX(const std::vector<double>& A,
                                 const std::vector<double>& B,
                                 int opoznienie,
                                 double szum)
{

    m_symulacja.konfigurujModel(A, B, opoznienie);
    // Jeśli dodasz obsługę szumu w Symulacji, odkomentuj:
    // m_symulacja.setSzum(szum);
}

void KlasaUslugowa::nowyRegulator(double k, double TI, double TD)
{
    m_symulacja.konfigurujRegulator(k, TI, TD);
}

void KlasaUslugowa::start() { m_symulacja.uruchom(); }
void KlasaUslugowa::stop()  { m_symulacja.zatrzymaj(); }
void KlasaUslugowa::reset() { m_symulacja.resetuj(); }

void KlasaUslugowa::wykonajKrokSymulacji()
{
    m_symulacja.wykonajKrok();
}

double KlasaUslugowa::getCzas() const { return m_symulacja.getCzas(); }
double KlasaUslugowa::getWartoscZadana() const { return m_symulacja.getWartoscZadana(); }
double KlasaUslugowa::getWartoscWyjscie() const { return m_symulacja.getWartoscWyjscie(); }
double KlasaUslugowa::getSterowanie() const { return m_symulacja.getSterowanie(); }
double KlasaUslugowa::getUchyb() const { return m_symulacja.getUchyb(); }

// KlasaUslugowa.cpp

std::vector<double> KlasaUslugowa::getWektorA() const
{
    return m_symulacja.getModelA();
}

std::vector<double> KlasaUslugowa::getWektorB() const
{
    return m_symulacja.getModelB();
}

int KlasaUslugowa::getOpoznienie() const
{
    return m_symulacja.getModelOpoznienie();
}

double KlasaUslugowa::getSzum() const
{
    return m_symulacja.getModelSzum();
}
