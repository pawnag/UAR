#include "Symulacja.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

Symulacja::Symulacja()
    : m_czyDziala(false),
    m_czas(0.0),
    m_wartoscZadana(0.0),
    m_wartoscWyjscie(0.0),
    m_sterowanie(0.0),
    m_uchyb(0.0)
{
}

void Symulacja::setModel(std::shared_ptr<ModelARX> i_model)
{
    m_model = i_model;
}

void Symulacja::setRegulator(std::shared_ptr<RegulatorPID> i_regulator)
{
    m_regulator = i_regulator;
}

void Symulacja::setGenerator(std::shared_ptr<GeneratorWartosciZadanej> i_generator)
{
    m_generator = i_generator;
}

int Symulacja::getInterwalMs() const
{
    if (m_generator) {
        // Pobierz interwał z generatora (konwersja z sekund na ms)
        return static_cast<int>(m_generator->getInterwal() * 1000.0);
    }
    return 200; // Domyślny interwał 200ms
}

void Symulacja::uruchom()
{
    m_czyDziala = true;
    int interwalMs = getInterwalMs();

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Czas[s]\tZadana\tWyjscie\tSterowanie\tUchyb\n";

    while (m_czyDziala)
    {
        wykonajKrok();

        std::cout << m_czas << "\t"
            << m_wartoscZadana << "\t"
            << m_wartoscWyjscie << "\t"
            << m_sterowanie << "\t\t"
            << m_uchyb << std::endl;

        if (m_generator) {
            m_generator->krokSymulacji();
        }

        // Aktualizuj czas na podstawie interwału z generatora
        interwalMs = getInterwalMs();
        m_czas += interwalMs / 1000.0;
        std::this_thread::sleep_for(std::chrono::milliseconds(interwalMs));
    }
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

    if (m_model) m_model->resetuj();
    if (m_regulator) m_regulator->resetuj();
    if (m_generator) m_generator->reset();
}

void Symulacja::wykonajKrok()
{
    // 1. Generuj wartość zadaną
    if (m_generator) {
        m_wartoscZadana = m_generator->generuj();
    }
    else {
        m_wartoscZadana = 0.0;
    }

    // 2. Określ konfigurację i oblicz wartości
    bool hasModel = (m_model != nullptr);
    bool hasRegulator = (m_regulator != nullptr);
    bool hasGenerator = (m_generator != nullptr);

    if (hasModel && hasRegulator) {
        // PEŁNY UAR: generator -> regulator -> model
        m_uchyb = m_wartoscZadana - m_wartoscWyjscie;
        m_sterowanie = m_regulator->symuluj(m_uchyb);
        m_wartoscWyjscie = m_model->symuluj(m_sterowanie);
    }
    else if (hasRegulator && hasGenerator) {
        // TYLKO PID: generator -> regulator -> wyjście = sterowanie
        m_uchyb = m_wartoscZadana;
        m_sterowanie = m_regulator->symuluj(m_uchyb);
        m_wartoscWyjscie = m_sterowanie; // Pokaż sterowanie jako wyjście
    }
    else if (hasModel && hasGenerator) {
        // UKŁAD OTWARTY: generator -> model
        m_sterowanie = m_wartoscZadana;
        m_uchyb = m_wartoscZadana;
        m_wartoscWyjscie = m_model->symuluj(m_sterowanie);
    }
    else if (hasModel) {
        // TYLKO MODEL: stałe sterowanie = 0
        m_sterowanie = 0.0;
        m_uchyb = 0.0;
        m_wartoscWyjscie = m_model->symuluj(m_sterowanie);
    }
    else if (hasGenerator) {
        // TYLKO GENERATOR: pokaż wartość zadaną jako wyjście
        m_sterowanie = m_wartoscZadana;
        m_uchyb = m_wartoscZadana;
        m_wartoscWyjscie = m_wartoscZadana;
    }
    else {
        // BRAK KOMPONENTÓW
        m_sterowanie = 0.0;
        m_uchyb = 0.0;
        m_wartoscWyjscie = 0.0;
    }
}
// Przykład 1: Tylko generator
void Symulacja::tylkoGenerator()
{
    auto generator = std::make_shared<GeneratorWartosciZadanej>();
    generator->setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_SINUSOIDALNY);
    generator->setAmplituda(1.0);
    generator->setOkresRzeczywisty(2.0);
    generator->setInterwal(200); // Ustawia interwał w ms

    Symulacja sym;
    sym.setGenerator(generator);
    sym.uruchom();
}

// Przykład 2: Generator + model
void Symulacja::generatorIModel()
{
    auto generator = std::make_shared<GeneratorWartosciZadanej>();
    generator->setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_PROSTOKATNY);
    generator->setAmplituda(2.0);
    generator->setOkresRzeczywisty(3.0);
    generator->setWypelnienie(0.4);
    generator->setInterwal(100); // 100ms

    auto model = std::make_shared<ModelARX>(
        std::vector<double>{-0.4},
        std::vector<double>{0.6},
        1, 0.0
    );

    Symulacja sym;
    sym.setGenerator(generator);
    sym.setModel(model);
    sym.uruchom();
}

// Przykład 3: Pełny układ regulacji
void Symulacja::pelnyUAR()
{
    auto generator = std::make_shared<GeneratorWartosciZadanej>();
    generator->setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_SINUSOIDALNY);
    generator->setAmplituda(1.0);
    generator->setOkresRzeczywisty(2.0);
    generator->setInterwal(50); // 50ms - szybsza symulacja

    auto model = std::make_shared<ModelARX>(
        std::vector<double>{-0.4},
        std::vector<double>{0.6},
        1, 0.0
    );

    auto regulator = std::make_shared<RegulatorPID>(0.5, 5.0, 0.2);

    Symulacja sym;
    sym.setGenerator(generator);
    sym.setModel(model);
    sym.setRegulator(regulator);
    sym.uruchom();
}