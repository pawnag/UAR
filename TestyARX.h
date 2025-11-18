#pragma once
#include "ModelARX.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>

namespace TESTY_ModelARX_Dodatkowe {
    void wykonaj_wszystkie_testy();

    // Nowe testy
    void test_ograniczenia_sterowania_dolne();
    void test_ograniczenia_sterowania_gorne();
    void test_ograniczenia_wyjscia_dolne();
    void test_ograniczenia_wyjscia_gorne();
    void test_ograniczenia_wylaczone();
    void test_zmiana_parametrow_A_w_trakcie_symulacji();
    void test_zmiana_parametrow_B_w_trakcie_symulacji();
    void test_zmiana_opoznienia_transportowego();
    void test_szum_zerowe_odchylenie();
    void test_szum_male_odchylenie();
    void test_brak_zaklocenia_dla_zerowego_odchylenia();
    void test_wielu_wspolczynnikow();

    // Funkcje pomocnicze
    bool porownanie_z_tolerancja(double a, double b, double tolerancja = 1e-3);
    void raport_pojedynczego_testu(const std::string& nazwa, bool wynik);
}
