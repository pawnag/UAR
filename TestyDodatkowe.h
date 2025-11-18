#pragma once
#include "GeneratorWartosciZadanej.h"
#include "RegulatorPID.h"
#include "Symulacja.h"
#include <vector>
#include <string>

namespace TESTY_Dodatkowe {
    void wykonaj_wszystkie_testy();

    // Testy dla GeneratorWartosciZadanej
    void test_generator_sygnal_staly();
    void test_generator_sygnal_sinusoidalny();
    void test_generator_sygnal_prostokatny();
    void test_generator_zmiana_parametrow();
    void test_generator_zerowa_amplituda();

    // Testy dla RegulatorPID
    void test_regulator_reset_calki();
    void test_regulator_zmiana_trybu_calkowania();
    void test_regulator_skladowa_D();
    void test_regulator_brak_akcji();

    // Testy dla Symulacja
    void test_symulacja_tylko_generator();
    void test_symulacja_generator_i_model();
    void test_symulacja_pelny_uar();

    // Funkcje pomocnicze
    bool porownanie_z_tolerancja(double a, double b, double tolerancja = 1e-3);
    void raport_pojedynczego_testu(const std::string& nazwa, bool wynik);
}

