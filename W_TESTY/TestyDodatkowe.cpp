#include "W_TESTY/TestyDodatkowe.h"
#include "W_TESTY/Testy.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

TestyDodatkowe::TestyDodatkowe(QObject *parent)
    : QObject{parent}
{}

namespace TESTY_Dodatkowe {

// --- TE FUNKCJE ZOSTAJĄ BEZ ZMIAN (używają cerr) ---
//funkcje przeniesione do "W_TESTY/Testy.h"

// --- TUTAJ ZMIENIAMY cout NA cerr ---

void wykonaj_wszystkie_testy() {
    std::cerr << "\n=== DODATKOWE TESTY DLA ROZNYCH KLAS ===" << std::endl; // Zmiana na cerr

    test_generator_sygnal_staly();
    test_generator_sygnal_sinusoidalny();
    test_generator_sygnal_prostokatny();
    test_generator_zmiana_parametrow();
    test_generator_zerowa_amplituda();

    test_regulator_reset_calki();
    test_regulator_zmiana_trybu_calkowania();
    test_regulator_skladowa_D();
    test_regulator_brak_akcji();

    test_symulacja_tylko_generator();
    test_symulacja_generator_i_model();
    test_symulacja_pelny_uar();

    wynikiTestow();

    std::cerr << "=== KONIEC DODATKOWYCH TESTOW ===" << std::endl; // Zmiana na cerr
}

void test_generator_sygnal_staly() {
    //std::cerr << "Generator -> test sygnalu stalego: "; // Zmiana na cerr
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

        myAssert("Generator - test sygnalu stalego", expected, actual);
    }
    catch (...) { std::cerr << "PRZERWANE!\n"; }
}

void test_generator_sygnal_sinusoidalny() {
    //std::cerr << "Generator -> test sygnalu sinusoidalnego: "; // Zmiana na cerr
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

        myAssert("Generator - test sygnalu sinusoidalnego",expected, actual);
    }
    catch (...) { std::cerr << "PRZERWANE!\n"; }
}

void test_generator_sygnal_prostokatny() {
    //std::cerr << "Generator -> test sygnalu prostokatnego: "; // Zmiana na cerr
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

        myAssert("Generator - test sygnalu prostokatnego",expected, actual);
    }
    catch (...) { std::cerr << "PRZERWANE!\n"; }
}

void test_generator_zmiana_parametrow() {
    //std::cerr << "Generator -> test zmiany parametrow: "; // Zmiana na cerr
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
        myAssert("Generator - test zmiany parametrow",expected, actual);
    }
    catch (...) { std::cerr << "PRZERWANE!\n"; }
}

void test_generator_zerowa_amplituda() {
    //std::cerr << "Generator -> test zerowej amplitudy: "; // Zmiana na cerr
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

        myAssert("Generator - test zerowej amplitudy",expected, actual);
    }
    catch (...) { std::cerr << "PRZERWANE!\n"; }
}

void test_regulator_reset_calki() {
    //std::cerr << "RegulatorPID -> test resetu calki: "; // Zmiana na cerr
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

        std::vector<double> expected = { 1.5, 2.5, 3.5, 1.5 };
        myAssert("RegulatorPID - test resetu calki",expected, outputs);
    }
    catch (...) { std::cerr << "PRZERWANE!\n"; }
}

void test_regulator_zmiana_trybu_calkowania() {
    //std::cerr << "RegulatorPID -> test zmiany trybu calkowania: "; // Zmiana na cerr
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

        std::vector<double> expected = { 1.5, 2.0, 2.5, 3.0 };
        myAssert("RegulatorPID - test zmiany trybu calkowania",expected, outputs);
    }
    catch (...) { std::cerr << "PRZERWANE!\n"; }
}

void test_regulator_skladowa_D() {
    //std::cerr << "RegulatorPID -> test skladowa D: "; // Zmiana na cerr
    try {
        RegulatorPID regulator(1.0, 0.0, 1.0);
        regulator.setLiczCalk(RegulatorPID::LiczCalk::Zew);

        std::vector<double> inputs = { 0.0, 1.0, 2.0, 3.0 };
        std::vector<double> outputs;

        for (double e : inputs) {
            outputs.push_back(regulator.symuluj(e));
        }

        std::vector<double> expected = { 0.0, 2.0, 3.0, 4.0 };
        myAssert("RegulatorPID - test skladowa D",expected, outputs);
    }
    catch (...) { std::cerr << "PRZERWANE!\n"; }
}

void test_regulator_brak_akcji() {
    //std::cerr << "RegulatorPID -> test braku akcji: "; // Zmiana na cerr
    try {
        RegulatorPID regulator(0.0, 0.0, 0.0);
        regulator.setLiczCalk(RegulatorPID::LiczCalk::Zew);

        std::vector<double> inputs = { 1.0, 2.0, 3.0 };
        std::vector<double> outputs;

        for (double e : inputs) {
            outputs.push_back(regulator.symuluj(e));
        }

        std::vector<double> expected = { 0.0, 0.0, 0.0 };
        myAssert("RegulatorPID - test braku akcji",expected, outputs);
    }
    catch (...) { std::cerr << "PRZERWANE!\n"; }
}

void test_symulacja_tylko_generator() {
    try {
        Symulacja simulation;

        // POPRAWKA: Konfigurujemy symulację tak, aby pasowała do oczekiwań (5.0)
        // Wcześniej konfigurowałeś prostokąt 1.0, a oczekiwałeś 5.0 ze zmiennej lokalnej.
        simulation.konfigurujGenerator(0.0, 1.0, 100, GeneratorWartosciZadanej::SYGNAL_STALY, 5.0, 0.5);

        std::vector<double> setpointValues;
        for (int i = 0; i < 5; i++) {
            simulation.wykonajKrok();
            setpointValues.push_back(simulation.getWartoscZadana());
        }

        std::vector<double> expected(5, 5.0);
        myAssert("Symulacja - test tylko generator", expected, setpointValues);
    }
    catch (...) { std::cerr << "PRZERWANE!\n"; }
}

void test_symulacja_generator_i_model() {
    try {
        Symulacja simulation;

        // Generator: Skok jednostkowy (Prostokąt, Ampl=1.0, Offset=0.0 -> stan wysoki = 1.0)
        simulation.konfigurujGenerator(1.0, 10.0, 100, GeneratorWartosciZadanej::SYGNAL_PROSTOKATNY, 0.0, 0.5);

        // POPRAWKA: Musisz skonfigurować model w symulacji!
        // Parametry z Twojego testu: A=[-0.4], B=[0.6], k=1
        simulation.konfigurujModel({-0.4}, {0.6}, 1);

        std::vector<double> outputs;
        for (int i = 0; i < 3; i++) {
            simulation.wykonajKrok();
            outputs.push_back(simulation.getWartoscWyjscie());
        }

        // T0: u=1.0 (start), y=0 (opóźnienie k=1) -> wyjscie=0
        // T1: u=1.0, y(1) = 0.6*u(0) - (-0.4)*y(0) = 0.6*1.0 - 0 = 0.6
        // T2: u=1.0, y(2) = 0.6*u(1) - (-0.4)*y(1) = 0.6*1.0 + 0.4*0.6 = 0.6 + 0.24 = 0.84
        std::vector<double> expected = { 0.0, 0.6, 0.84 };
        myAssert("Symulacja - test generator i model", expected, outputs);
    }
    catch (...) { std::cerr << "PRZERWANE!\n"; }
}

void test_symulacja_pelny_uar() {
    try {
        Symulacja simulation;

        // Generator: Skok 1.0
        simulation.konfigurujGenerator(1.0, 10.0, 100, GeneratorWartosciZadanej::SYGNAL_PROSTOKATNY, 0.0, 0.5);

        // POPRAWKA: Konfiguracja Modelu (A=[-0.4], B=[0.6], k=1)
        simulation.konfigurujModel({-0.4}, {0.6}, 1);

        // POPRAWKA: Konfiguracja Regulatora (Kp=0.5, Ti=1.0, Td=0.0)
        simulation.konfigurujRegulator(0.5, 1.0, 0.0);

        std::vector<double> outputs;
        for (int i = 0; i < 3; i++) {
            simulation.wykonajKrok();
            outputs.push_back(simulation.getWartoscWyjscie());
        }

        // Obliczenia ręczne (dla weryfikacji):
        // Krok 1: e=1, Up=0.5, Ui=1.0 -> u=1.5. Model(k=1) -> y=0.0.
        // Krok 2: e=1, Up=0.5, Ui+=1.0(2.0) -> u=2.5. Model: y = 0.6*1.5 + 0.4*0 = 0.9.
        // Krok 3: e=0.1, Up=0.05, Ui+=0.1(2.1) -> u=2.15. Model: y = 0.6*2.5 + 0.4*0.9 = 1.5 + 0.36 = 1.86.
        std::vector<double> expected = { 0.0, 0.9, 1.86 };
        myAssert("Symulacja - test pelny UAR", expected, outputs);
    }
    catch (...) { std::cerr << "PRZERWANE!\n"; }
}

}
