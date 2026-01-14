#include "Symulacja.h"
#include <QDebug>

Symulacja::Symulacja(GeneratorWartosciZadanej i_generator, ProstyUAR i_prostyUAR)
    : m_generator(i_generator),
    m_prostyUAR(i_prostyUAR),
    m_czyDziala(false),
    m_czas(0.0),
    m_wartoscZadana(0.0),
    m_wartoscWyjscie(0.0),
    m_sterowanie(0.0),
    m_uchyb(0.0)
{}

void Symulacja::konfigurujModel(const std::vector<double>& A, const std::vector<double>& B, int opoznienie, double szum)
{
    // Aby skorzystać z grupowej metody konfigurujModel w ProstyUAR,
    // musimy pobrać aktualne limity (żeby ich niechcący nie wyzerować)
    ModelARX temp = m_prostyUAR.pobierzModel();

    m_prostyUAR.konfigurujModel(A, B, opoznienie, szum,
                                temp.getUMIN(), temp.getUMAX(),
                                temp.getYMIN(), temp.getYMAX());
}

void Symulacja::konfigurujRegulator(double k, double ti, double td)
{
    // Podobnie dla regulatora - musimy zachować obecną metodę całkowania
    RegulatorPID temp = m_prostyUAR.pobierzRegulator();

    m_prostyUAR.konfigurujRegulator(k, ti, td, (int)temp.getLiczCalk());
}

void Symulacja::konfigurujMetodePID(int indeksMetody)
{
    RegulatorPID pid = m_prostyUAR.pobierzRegulator();

    m_prostyUAR.konfigurujRegulator(
        pid.getWzmocnienie(),
        pid.getStalaCalk(),
        pid.getStalaRozn(),
        indeksMetody // To jest jedyna zmieniona wartość
        );
}

void Symulacja::konfigurujGenerator(double ampl, double okres, int interwal,
                                    GeneratorWartosciZadanej::TypSygnalu typ,
                                    double skladowa, double wypelnienie)
{
    m_generator.setAmplituda(ampl);
    m_generator.setOkresRzeczywisty(okres);
    m_generator.setInterwal(interwal);
    m_generator.setTypSygnalu(typ);
    m_generator.setSkladowaStala(skladowa);
    m_generator.setWypelnienie(wypelnienie);
    m_generator.przeliczOkresDyskretny();
}

void Symulacja::uruchom()
{
    m_czyDziala = true;
}

void Symulacja::zatrzymaj()
{
    m_czyDziala = false;
}

void Symulacja::resetuj()
{
    zatrzymaj();
    m_czas = 0.0;
    m_wartoscZadana = 0.0;
    m_wartoscWyjscie = 0.0;
    m_sterowanie = 0.0;
    m_uchyb = 0.0;

    m_generator.reset();
    m_prostyUAR.reset();
}

void Symulacja::wykonajKrok()
{
    // 1. Generacja Wartości Zadanej
    m_wartoscZadana = m_generator.generuj();
    m_generator.krokSymulacji();

    // 2. Symulacja Pętli UAR
    m_wartoscWyjscie = m_prostyUAR.symuluj(m_wartoscZadana);

    // 3. Pobranie wyników
    m_uchyb = m_prostyUAR.getOstatniUchyb();
    m_sterowanie = m_prostyUAR.getOstatnieSterowanie();

    // 4. Aktualizacja Czasu
    double dt_sec = m_generator.getInterwal() / 1000.0;
    m_czas += dt_sec;
}

void Symulacja::resetUAR()
{
    m_prostyUAR.reset();
}

// --- Gettery Fasadowe (Pobierają kopię modelu i zwracają wartość) ---

std::vector<double> Symulacja::getModelA() const { return m_prostyUAR.pobierzModel().getA(); }
std::vector<double> Symulacja::getModelB() const { return m_prostyUAR.pobierzModel().getB(); }
int Symulacja::getModelOpoznienie() const { return m_prostyUAR.pobierzModel().getOpoznienieTransportowe(); }
double Symulacja::getModelSzum() const { return m_prostyUAR.pobierzModel().getOdchylenieStandardoweSzumu(); }

double Symulacja::getModelUMIN() const { return m_prostyUAR.pobierzModel().getUMIN(); }
double Symulacja::getModelUMAX() const { return m_prostyUAR.pobierzModel().getUMAX(); }
double Symulacja::getModelYMIN() const { return m_prostyUAR.pobierzModel().getYMIN(); }
double Symulacja::getModelYMAX() const { return m_prostyUAR.pobierzModel().getYMAX(); }

// --- Gettery Procesowe ---
double Symulacja::getWartoscZadana() const { return m_wartoscZadana; }
double Symulacja::getWartoscWyjscie() const { return m_wartoscWyjscie; }
double Symulacja::getSterowanie() const { return m_sterowanie; }
double Symulacja::getUchyb() const { return m_uchyb; }
double Symulacja::getCzas() const { return m_czas; }
bool Symulacja::czyDziala() const { return m_czyDziala; }
int Symulacja::getInterwalMs() const { return m_generator.getInterwal(); }

// --- Pobieranie Kopii Obiektów ---

GeneratorWartosciZadanej Symulacja::pobierzGenerator() const
{
    return m_generator;
}

ModelARX Symulacja::pobierzModel() const
{
    return m_prostyUAR.pobierzModel();
}

RegulatorPID Symulacja::pobierzRegulator() const
{
    return m_prostyUAR.pobierzRegulator();
}
