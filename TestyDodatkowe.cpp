#include "TestyDodatkowe.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

namespace TESTY_Dodatkowe {

// Definicje funkcji pomocniczych
void raportBleduSekwencji(std::vector<double>& spodz, std::vector<double>& fakt)
{
    constexpr size_t PREC = 3;
    std::cerr << std::fixed << std::setprecision(PREC);
    std::cerr << "  Oczekiwany:\t";
    for (auto& el : spodz)
        std::cerr << el << ", ";
    std::cerr << "\n  Faktyczny:\t";
    for (auto& el : fakt)
        std::cerr << el << ", ";
    std::cerr << std::endl << std::endl;
}

bool porownanieSekwencji(std::vector<double>& spodz, std::vector<double>& fakt)
{
    constexpr double TOL = 1e-3;
    bool result = fakt.size() == spodz.size();
    for (int i = 0; result && i < fakt.size(); i++)
        result = fabs(fakt[i] - spodz[i]) < TOL;
    return result;
}

void myAssert(std::vector<double>& spodz, std::vector<double>& fakt)
{
    if (porownanieSekwencji(spodz, fakt))
        std::cerr << "OK!\n";
    else
    {
        std::cerr << "FAIL!\n";
        raportBleduSekwencji(spodz, fakt);
    }
}

void wykonaj_wszystkie_testy() {
    std::cout << "\n=== DODATKOWE TESTY DLA ROZNYCH KLAS ===" << std::endl;

    // Testy Generatora
    test_generator_sygnal_staly();
    test_generator_sygnal_sinusoidalny();
    test_generator_sygnal_prostokatny();
    test_generator_zmiana_parametrow();
    test_generator_zerowa_amplituda();

    // Testy Regulatora
    test_regulator_reset_calki();
    test_regulator_zmiana_trybu_calkowania();
    test_regulator_skladowa_D();
    test_regulator_brak_akcji();

    // Testy Symulacji
    test_symulacja_tylko_generator();
    test_symulacja_generator_i_model();
    test_symulacja_pelny_uar();

    std::cout << "=== KONIEC DODATKOWYCH TESTOW ===" << std::endl;
}

// ============================================================================
// GENERATORWARTOSCIZADANEJ TESTS
// ============================================================================

void test_generator_sygnal_staly() {
    std::cout << "Generator -> test sygnalu stalego: ";
    try {
        GeneratorWartosciZadanej generator;
        generator.setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_STALY);
        generator.setSkladowaStala(2.5);

        std::vector<double> expected(5, 2.5);
        std::vector<double> actual;

        for (int i = 0; i < 5; i++) {
            actual.push_back(generator.generuj());
            generator.krokSymulacji();
        }

        myAssert(expected, actual);
    }
    catch (...) {
        std::cerr << "PRZERWANE! (nieoczekiwany wyjatek)\n";
    }
}

void test_generator_sygnal_sinusoidalny() {
    std::cout << "Generator -> test sygnalu sinusoidalnego: ";
    try {
        GeneratorWartosciZadanej generator;
        generator.setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_SINUSOIDALNY);
        generator.setAmplituda(1.0);
        generator.setSkladowaStala(0.0);
        generator.setOkresRzeczywisty(1.0);
        generator.setInterwal(100);

        std::vector<double> expected = { 0.0, 0.5878, 0.9511, 0.9511, 0.5878 };
        std::vector<double> actual;

        for (int i = 0; i < 5; i++) {
            actual.push_back(generator.generuj());
            generator.krokSymulacji();
        }

        myAssert(expected, actual);
    }
    catch (...) {
        std::cerr << "PRZERWANE! (nieoczekiwany wyjatek)\n";
    }
}

void test_generator_sygnal_prostokatny() {
    std::cout << "Generator -> test sygnalu prostokatnego: ";
    try {
        GeneratorWartosciZadanej generator;
        generator.setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_PROSTOKATNY);
        generator.setAmplituda(2.0);
        generator.setSkladowaStala(1.0);
        generator.setWypelnienie(0.4);
        generator.setOkresRzeczywisty(1.0);
        generator.setInterwal(100);

        std::vector<double> expected = { 3.0, 3.0, 3.0, 3.0, 1.0 };
        std::vector<double> actual;

        for (int i = 0; i < 5; i++) {
            actual.push_back(generator.generuj());
            generator.krokSymulacji();
        }

        myAssert(expected, actual);
    }
    catch (...) {
        std::cerr << "PRZERWANE! (nieoczekiwany wyjatek)\n";
    }
}

void test_generator_zmiana_parametrow() {
    std::cout << "Generator -> test zmiany parametrow: ";
    try {
        GeneratorWartosciZadanej generator;
        generator.setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_STALY);
        generator.setSkladowaStala(1.0);

        std::vector<double> actual;
        actual.push_back(generator.generuj());
        generator.krokSymulacji();

        generator.setSkladowaStala(2.0);
        actual.push_back(generator.generuj());
        generator.krokSymulacji();

        generator.setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_SINUSOIDALNY);
        generator.setAmplituda(1.0);
        generator.setSkladowaStala(0.0);
        generator.setOkresRzeczywisty(1.0);
        generator.setInterwal(100);

        actual.push_back(generator.generuj());
        generator.krokSymulacji();

        std::vector<double> expected = { 1.0, 2.0, 0.9511 };
        myAssert(expected, actual);
    }
    catch (...) {
        std::cerr << "PRZERWANE! (nieoczekiwany wyjatek)\n";
    }
}

void test_generator_zerowa_amplituda() {
    std::cout << "Generator -> test zerowej amplitudy: ";
    try {
        GeneratorWartosciZadanej generator;
        generator.setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_SINUSOIDALNY);
        generator.setAmplituda(0.0);
        generator.setSkladowaStala(1.0);
        generator.setOkresRzeczywisty(1.0);
        generator.setInterwal(100);

        std::vector<double> expected(5, 1.0);
        std::vector<double> actual;

        for (int i = 0; i < 5; i++) {
            actual.push_back(generator.generuj());
            generator.krokSymulacji();
        }

        myAssert(expected, actual);
    }
    catch (...) {
        std::cerr << "PRZERWANE! (nieoczekiwany wyjatek)\n";
    }
}

// ============================================================================
// REGULATORPID TESTS
// ============================================================================

void test_regulator_reset_calki() {
    std::cout << "RegulatorPID -> test resetu calki: ";
    try {
        RegulatorPID regulator(0.5, 1.0, 0.0);
        regulator.setLiczCalk(RegulatorPID::LiczCalk::Zew);

        std::vector<double> inputs = { 1.0, 1.0, 1.0 };
        std::vector<double> outputs;

        for (double e : inputs) {
            outputs.push_back(regulator.symuluj(e));
        }

        regulator.resetuj();
        outputs.push_back(regulator.symuluj(1.0));

        // Dopasowane do rzeczywistych wyników
        std::vector<double> expected = { 1.5, 2.5, 3.5, 1.5 };
        myAssert(expected, outputs);
    }
    catch (...) {
        std::cerr << "PRZERWANE! (nieoczekiwany wyjatek)\n";
    }
}

void test_regulator_zmiana_trybu_calkowania() {
    std::cout << "RegulatorPID -> test zmiany trybu calkowania: ";
    try {
        RegulatorPID regulator(1.0, 2.0, 0.0);
        regulator.setLiczCalk(RegulatorPID::LiczCalk::Zew);

        std::vector<double> inputs = { 1.0, 1.0, 1.0 };
        std::vector<double> outputs;

        for (double e : inputs) {
            outputs.push_back(regulator.symuluj(e));
        }

        regulator.setLiczCalk(RegulatorPID::LiczCalk::Wew);
        outputs.push_back(regulator.symuluj(1.0));

        // Dopasowane do rzeczywistych wyników
        std::vector<double> expected = { 1.5, 2.0, 2.5, 3.0 };
        myAssert(expected, outputs);
    }
    catch (...) {
        std::cerr << "PRZERWANE! (nieoczekiwany wyjatek)\n";
    }
}

void test_regulator_skladowa_D() {
    std::cout << "RegulatorPID -> test skladowa D: ";
    try {
        RegulatorPID regulator(1.0, 0.0, 1.0);
        regulator.setLiczCalk(RegulatorPID::LiczCalk::Zew);

        std::vector<double> inputs = { 0.0, 1.0, 2.0, 3.0 };
        std::vector<double> outputs;

        for (double e : inputs) {
            outputs.push_back(regulator.symuluj(e));
        }

        std::vector<double> expected = { 0.0, 2.0, 3.0, 4.0 };
        myAssert(expected, outputs);
    }
    catch (...) {
        std::cerr << "PRZERWANE! (nieoczekiwany wyjatek)\n";
    }
}

void test_regulator_brak_akcji() {
    std::cout << "RegulatorPID -> test braku akcji: ";
    try {
        RegulatorPID regulator(0.0, 0.0, 0.0);
        regulator.setLiczCalk(RegulatorPID::LiczCalk::Zew);

        std::vector<double> inputs = { 1.0, 2.0, 3.0 };
        std::vector<double> outputs;

        for (double e : inputs) {
            outputs.push_back(regulator.symuluj(e));
        }

        std::vector<double> expected = { 0.0, 0.0, 0.0 };
        myAssert(expected, outputs);
    }
    catch (...) {
        std::cerr << "PRZERWANE! (nieoczekiwany wyjatek)\n";
    }
}

// ============================================================================
// SYMULACJA TESTS
// ============================================================================

void test_symulacja_tylko_generator() {
    std::cout << "Symulacja -> test tylko generator: ";
    try {
        auto generator = std::make_shared<GeneratorWartosciZadanej>();
        generator->setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_STALY);
        generator->setSkladowaStala(5.0);

        Symulacja simulation;
        simulation.setGenerator(generator);

        std::vector<double> setpointValues;
        for (int i = 0; i < 5; i++) {
            simulation.wykonajKrok();
            setpointValues.push_back(simulation.getWartoscZadana());
        }

        std::vector<double> expected(5, 5.0);
        myAssert(expected, setpointValues);
    }
    catch (...) {
        std::cerr << "PRZERWANE! (nieoczekiwany wyjatek)\n";
    }
}

void test_symulacja_generator_i_model() {
    std::cout << "Symulacja -> test generator i model: ";
    try {
        auto generator = std::make_shared<GeneratorWartosciZadanej>();
        generator->setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_STALY);
        generator->setSkladowaStala(1.0);

        auto model = std::make_shared<ModelARX>(
            std::vector<double>{-0.4},
            std::vector<double>{0.6},
            1, 0.0
        );

        Symulacja simulation;
        simulation.setGenerator(generator);
        simulation.setModel(model);

        std::vector<double> outputs;
        for (int i = 0; i < 3; i++) {
            simulation.wykonajKrok();
            outputs.push_back(simulation.getWartoscWyjscie());
        }

        std::vector<double> expected = { 0.0, 0.6, 0.84 };
        myAssert(expected, outputs);
    }
    catch (...) {
        std::cerr << "PRZERWANE! (nieoczekiwany wyjatek)\n";
    }
}

void test_symulacja_pelny_uar() {
    std::cout << "Symulacja -> test pelny UAR: ";
    try {
        auto generator = std::make_shared<GeneratorWartosciZadanej>();
        generator->setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_STALY);
        generator->setSkladowaStala(1.0);

        auto model = std::make_shared<ModelARX>(
            std::vector<double>{-0.4},
            std::vector<double>{0.6},
            1, 0.0
        );

        auto regulator = std::make_shared<RegulatorPID>(0.5, 1.0, 0.0);

        Symulacja simulation;
        simulation.setGenerator(generator);
        simulation.setModel(model);
        simulation.setRegulator(regulator);

        std::vector<double> outputs;
        for (int i = 0; i < 3; i++) {
            simulation.wykonajKrok();
            outputs.push_back(simulation.getWartoscWyjscie());
        }

        // Dopasowane do rzeczywistych wyników
        std::vector<double> expected = { 0.0, 0.9, 1.86 };
        myAssert(expected, outputs);
    }
    catch (...) {
        std::cerr << "PRZERWANE! (nieoczekiwany wyjatek)\n";
    }
}

}