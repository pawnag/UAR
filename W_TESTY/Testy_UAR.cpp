#include <QCoreApplication>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <memory>
#include <thread>

#include "W_TESTY/Testy.h"
#include "W_DANYCH/ModelARX.h"
#include "W_DANYCH/RegulatorPID.h"
#include "W_DANYCH/GeneratorWartosciZadanej.h"
#include "W_DANYCH/ProstyUAR.h"
#include "W_DANYCH/Symulacja.h"
#include "W_TESTY/TestyDodatkowe.h"

#define DEBUG  // Tryb testów jednostkowych
// #define MAIN   // Tryb symulacji konsolowej (stary main)

/*
void raportBleduSekwencji(const std::vector<double>& spodz, const std::vector<double>& fakt)
{
    constexpr size_t PREC = 3;
    std::cerr << std::fixed << std::setprecision(PREC);
    std::cerr << "  Spodziewany:\t";
    for (const auto& el : spodz) std::cerr << el << ", ";
    std::cerr << "\n  Faktyczny:\t";
    for (const auto& el : fakt) std::cerr << el << ", ";
    std::cerr << std::endl << std::endl;
}
bool porownanieSekwencji(const std::vector<double>& spodz, const std::vector<double>& fakt)
{
    constexpr double TOL = 1e-3;
    if (fakt.size() != spodz.size()) return false;
    for (size_t i = 0; i < fakt.size(); i++)
        if (std::fabs(fakt[i] - spodz[i]) >= TOL) return false;
    return true;
}
void myAssert(const std::vector<double>& spodz, const std::vector<double>& fakt)
{
    if (porownanieSekwencji(spodz, fakt))
        std::cerr << "OK!\n";
    else
    {
        std::cerr << "FAIL!\n";
        raportBleduSekwencji(spodz, fakt);
    }
}
*/

// ---------------------------------------------------------
// TESTY DLA MODELU ARX
// ---------------------------------------------------------
namespace TESTY_ModelARX
{
void wykonaj_testy();
void test_brakPobudzenia();
void test_skokJednostkowy_1();
void test_skokJednostkowy_2();
void test_skokJednostkowy_3();
}

void TESTY_ModelARX::wykonaj_testy()
{
    //std::cerr << "--- TESTY ARX ---\n";
    test_brakPobudzenia();
    test_skokJednostkowy_1();
    test_skokJednostkowy_2();
    test_skokJednostkowy_3();
    //std::cerr << "\n";
}

void TESTY_ModelARX::test_brakPobudzenia()
{
    //std::cerr << "ModelARX (-0.4 | 0.6 | 1 | 0 ) -> test zerowego pobudzenia: ";
    try {
        ModelARX instancjaTestowa({ -0.4 }, { 0.6 }, 1, 0);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER, 0.0);
        std::vector<double> spodzSygWy(LICZ_ITER, 0.0);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

        myAssert("ModelARX (-0.4 | 0.6 | 1 | 0 ) -> test zerowego pobudzenia:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

void TESTY_ModelARX::test_skokJednostkowy_1()
{
    //std::cerr << "ModelARX (-0.4 | 0.6 | 1 | 0 ) -> test skoku jednostkowego nr 1: ";
    try {
        ModelARX instancjaTestowa({ -0.4 }, { 0.6 }, 1, 0);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++) sygWe[i] = (i > 0 ? 1.0 : 0.0);

        std::vector<double> spodzSygWy = { 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362,
                                          0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999,
                                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

        myAssert("ModelARX (-0.4 | 0.6 | 1 | 0 ) -> test skoku jednostkowego nr 1:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

void TESTY_ModelARX::test_skokJednostkowy_2()
{
    //std::cerr << "ModelARX (-0.4 | 0.6 | 2 | 0 ) -> test skoku jednostkowego nr 2: ";
    try {
        ModelARX instancjaTestowa({ -0.4 }, { 0.6 }, 2, 0);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++) sygWe[i] = (i > 0 ? 1.0 : 0.0);

        std::vector<double> spodzSygWy = { 0, 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

        myAssert("ModelARX (-0.4 | 0.6 | 2 | 0 ) -> test skoku jednostkowego nr 2:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

void TESTY_ModelARX::test_skokJednostkowy_3()
{
    //std::cerr << "ModelARX (-0.4, 0.2 | 0.6, 0.3 | 2 | 0 ) -> test skoku jednostkowego nr 3: ";
    try {
        ModelARX instancjaTestowa({ -0.4, 0.2 }, { 0.6, 0.3 }, 2, 0);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++) sygWe[i] = (i > 0 ? 1.0 : 0.0);

        std::vector<double> spodzSygWy = { 0, 0, 0, 0.6, 1.14, 1.236, 1.1664, 1.11936, 1.11446, 1.12191, 1.12587, 1.12597, 1.12521, 1.12489, 1.12491, 1.12499, 1.12501, 1.12501, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125 };

        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

        myAssert("ModelARX (-0.4, 0.2 | 0.6, 0.3 | 2 | 0 ) -> test skoku jednostkowego nr 3:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

// ---------------------------------------------------------
// TESTY DLA REGULATORA PID
// ---------------------------------------------------------
namespace TESTY_RegulatorPID
{
void wykonaj_testy();
void test_P_brakPobudzenia();
void test_P_skokJednostkowy();
void test_PI_skokJednostkowy_1();
void test_PI_skokJednostkowy_2();
void test_PID_skokJednostkowy();
void test_PI_skokJednostkowy_3();
}

void TESTY_RegulatorPID::wykonaj_testy()
{
    //std::cerr << "--- TESTY PID ---\n";
    test_P_brakPobudzenia();
    test_P_skokJednostkowy();
    test_PI_skokJednostkowy_1();
    test_PI_skokJednostkowy_2();
    test_PI_skokJednostkowy_3();
    test_PID_skokJednostkowy();
    //std::cerr << "\n";
}

void TESTY_RegulatorPID::test_P_brakPobudzenia()
{
    //std::cerr << "RegP (k = 0.5) -> test zerowego pobudzenia: ";
    try {
        RegulatorPID instancjaTestowa(0.5);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER, 0.0);
        std::vector<double> spodzSygWy(LICZ_ITER, 0.0);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

        myAssert("RegP (k = 0.5) -> test zerowego pobudzenia:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

void TESTY_RegulatorPID::test_P_skokJednostkowy()
{
    //std::cerr << "RegP (k = 0.5) -> test skoku jednostkowego: ";
    try {
        RegulatorPID instancjaTestowa(0.5);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++) sygWe[i] = (i > 0 ? 1.0 : 0.0);

        std::vector<double> spodzSygWy(LICZ_ITER, 0.5);
        spodzSygWy[0] = 0.0; // Pierwsza probka to 0

        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

        myAssert("RegP (k = 0.5) -> test skoku jednostkowego:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

void TESTY_RegulatorPID::test_PI_skokJednostkowy_1()
{
    //std::cerr << "RegPI (k = 0.5, TI = 1.0) -> test skoku jednostkowego nr 1: ";
    try {
        RegulatorPID instancjaTestowa(0.5, 1.0);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++) sygWe[i] = (i > 0 ? 1.0 : 0.0);

        std::vector<double> spodzSygWy = { 0, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5, 11.5, 12.5,
                                          13.5, 14.5, 15.5, 16.5, 17.5, 18.5, 19.5, 20.5, 21.5, 22.5, 23.5,
                                          24.5, 25.5, 26.5, 27.5, 28.5, 29.5 };

        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

        myAssert("RegPI (k = 0.5, TI = 1.0) -> test skoku jednostkowego nr 1:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

void TESTY_RegulatorPID::test_PI_skokJednostkowy_2()
{
    //std::cerr << "RegPI (k = 0.5, TI = 10.0) -> test skoku jednostkowego nr 2: ";
    try {
        RegulatorPID instancjaTestowa(0.5, 10.0);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++) sygWe[i] = (i > 0 ? 1.0 : 0.0);

        std::vector<double> spodzSygWy = { 0, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,
                                          2, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3, 3.1, 3.2, 3.3, 3.4 };

        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

        myAssert("RegPI (k = 0.5, TI = 10.0) -> test skoku jednostkowego nr 2:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

void TESTY_RegulatorPID::test_PID_skokJednostkowy()
{
    //std::cerr << "RegPID (k = 0.5, TI = 10.0, TD = 0.2) -> test skoku jednostkowego: ";
    try {
        RegulatorPID instancjaTestowa(0.5, 10.0, 0.2);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++) sygWe[i] = (i > 0 ? 1.0 : 0.0);

        std::vector<double> spodzSygWy = { 0, 0.8, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.1,
                                          2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3, 3.1, 3.2, 3.3, 3.4 };

        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

        myAssert("RegPID (k = 0.5, TI = 10.0, TD = 0.2) -> test skoku jednostkowego:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

void TESTY_RegulatorPID::test_PI_skokJednostkowy_3()
{
    //std::cerr << "RegPI (k = 0.5, TI = 10.0 -> 5.0 -> 10.0) -> test skoku jednostkowego nr 3: ";
    try {
        RegulatorPID instancjaTestowa(0.5, 10.0);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++) sygWe[i] = (i > 0 ? 1.0 : 0.0);

        std::vector<double> spodzSygWy = { 0, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.8, 2, 2.2, 2.4,
                                          2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 2.35, 2.45, 2.55, 2.65, 2.75, 2.85 };

        for (size_t i = 0; i < LICZ_ITER; i++)
        {
            if (i == LICZ_ITER * 1 / 5) instancjaTestowa.setLiczCalk(RegulatorPID::LiczCalk::Wew);
            if (i == LICZ_ITER * 2 / 5) instancjaTestowa.setStalaCalk(5.0);
            if (i == LICZ_ITER * 3 / 5) instancjaTestowa.setLiczCalk(RegulatorPID::LiczCalk::Zew);
            if (i == LICZ_ITER * 4 / 5) instancjaTestowa.setStalaCalk(10.0);

            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);
        }
        myAssert("RegPI (k = 0.5, TI = 10.0 -> 5.0 -> 10.0) -> test skoku jednostkowego nr 3:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

// ---------------------------------------------------------
// TESTY DLA PROSTEGO UAR
// ---------------------------------------------------------
namespace TESTY_ProstyUAR
{
void wykonaj_testy();
void test_UAR_1_brakPobudzenia();
void test_UAR_1_skokJednostkowyPID();
void test_UAR_2_skokJednostkowyPID();
void test_UAR_3_skokJednostkowyPID();
}

void TESTY_ProstyUAR::wykonaj_testy()
{
    //std::cerr << "--- TESTY UAR ---\n";
    test_UAR_1_brakPobudzenia();
    test_UAR_1_skokJednostkowyPID();
    test_UAR_2_skokJednostkowyPID();
    test_UAR_3_skokJednostkowyPID();
    //std::cerr << "\n";
}

void TESTY_ProstyUAR::test_UAR_1_brakPobudzenia()
{
    //std::cerr << "UAR_1 -> test zerowego pobudzenia: ";
    try {
        RegulatorPID testPID(0.5, 5.0, 0.2);
        ModelARX testARX({ -0.4 }, { 0.6 });
        ProstyUAR instancjaTestowa(testARX, testPID); // Przekazujemy REFERENCJE do QObject
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER, 0.0);
        std::vector<double> spodzSygWy(LICZ_ITER, 0.0);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

        myAssert("UAR_1 -> test zerowego pobudzenia:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

void TESTY_ProstyUAR::test_UAR_1_skokJednostkowyPID()
{
    //std::cerr << "UAR_1 PID -> test skoku jednostkowego: ";
    try {
        RegulatorPID testPID(0.5, 5.0, 0.2);
        ModelARX testARX({ -0.4 }, { 0.6 });
        ProstyUAR instancjaTestowa(testARX, testPID);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++) sygWe[i] = (i > 0 ? 1.0 : 0.0);

        std::vector<double> spodzSygWy = { 0.0, 0.0, 0.54, 0.756, 0.6708, 0.64008, 0.729, 0.810437, 0.834499,
            0.843338, 0.8664, 0.8936, 0.911886, 0.923312, 0.93404, 0.944929,
            0.954065, 0.961042, 0.966815, 0.971965, 0.97642, 0.980096, 0.983143,
            0.985741, 0.987964, 0.989839, 0.991411, 0.992739, 0.993865, 0.994818
        };

        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

        myAssert("UAR_1 PID -> test skoku jednostkowego:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

void TESTY_ProstyUAR::test_UAR_2_skokJednostkowyPID()
{
    //std::cerr << "UAR_2 PID (k = 2) -> test skoku jednostkowego: ";
    try {
        RegulatorPID testPID(0.5, 5.0, 0.2);
        ModelARX testARX({ -0.4 }, { 0.6 }, 2);
        ProstyUAR instancjaTestowa(testARX, testPID);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++) sygWe[i] = (i > 0 ? 1.0 : 0.0);

        std::vector<double> spodzSygWy = { 0.0, 0.0, 0.0, 0.54, 0.756, 0.9624, 0.87336, 0.841104, 0.771946, 0.821644,
            0.863453, 0.93272, 0.952656, 0.965421, 0.954525, 0.955787, 0.957472,
            0.969711, 0.978075, 0.985968, 0.987821, 0.989149, 0.989053, 0.990645,
            0.992248, 0.994403, 0.995707, 0.996677, 0.997024, 0.997388
        };

        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

        myAssert("UAR_2 PID (k = 2) -> test skoku jednostkowego:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

void TESTY_ProstyUAR::test_UAR_3_skokJednostkowyPID()
{
    //std::cerr << "UAR_3 PID (kP=1.0,Ti=2.0) -> test skoku jednostkowego: ";
    try {
        RegulatorPID testPID(1.0, 2.0, 0.2);
        ModelARX testARX({ -0.4 }, { 0.6 }, 1);
        ProstyUAR instancjaTestowa(testARX, testPID);
        constexpr size_t LICZ_ITER = 30;
        std::vector<double> sygWe(LICZ_ITER);
        std::vector<double> faktSygWy(LICZ_ITER);

        for (size_t i = 0; i < LICZ_ITER; i++) sygWe[i] = (i > 0 ? 1.0 : 0.0);

        std::vector<double> spodzSygWy = { 0.0, 0.0, 1.02, 1.608, 1.1028, 0.41736, 0.546648, 1.20605, 1.43047,
            0.999176, 0.615056, 0.799121, 1.21304, 1.26025, 0.939289, 0.748507,
            0.927166, 1.17292, 1.14155, 0.921616, 0.843258, 0.990018, 1.12577,
            1.068, 0.927024, 0.908125, 1.01702, 1.08484, 1.02618, 0.941508
        };

        for (size_t i = 0; i < LICZ_ITER; i++)
            faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

        myAssert("UAR_3 PID (kP=1.0,Ti=2.0) -> test skoku jednostkowego:",spodzSygWy, faktSygWy);
    } catch (...) { std::cerr << "INTERUPTED!\n"; }
}

// ---------------------------------------------------------
// MAIN
// ---------------------------------------------------------

void uruchomWszystkieTesty()
{
    std::cerr << "=== ROZPOCZYNAM TESTY JEDNOSTKOWE ===\n\n";

    TESTY_ModelARX::wykonaj_testy();
    TESTY_RegulatorPID::wykonaj_testy();
    TESTY_ProstyUAR::wykonaj_testy();

    TESTY_Dodatkowe::wykonaj_wszystkie_testy();

    std::cerr << "=== KONIEC TESTOW ===\n";
}

// #ifdef DEBUG
// int main(int argc, char *argv[])
// {
//     // Wymagane, jeśli używamy QObjects, nawet w konsoli
//     QCoreApplication a(argc, argv);

//     TESTY_ModelARX::wykonaj_testy();
//     TESTY_RegulatorPID::wykonaj_testy();
//     TESTY_ProstyUAR::wykonaj_testy();

//     // TESTY_Dodatkowe::wykonaj_wszystkie_testy();

//     // return a.exec(); // Nie używamy exec(), bo to testy jednorazowe, a nie pętla zdarzeń
//     return 0;
// }
//#endif

/*
#ifdef MAIN
// UWAGA: Ta część wymaga klasy Symulacja
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto generator = std::make_shared<GeneratorWartosciZadanej>();
    generator->setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_SINUSOIDALNY);
    generator->setAmplituda(1.5);
    generator->setOkresRzeczywisty(3.0);
    generator->setInterwal(100);

    auto model = std::make_shared<ModelARX>(
        std::vector<double>{-0.4, -0.1, -0.2},
        std::vector<double>{0.6, 0.2, 0.3},
        1,
        0.01
    );

    auto regulator = std::make_shared<RegulatorPID>(0.8, 4.0, 0.1);

    // Aby to działało, musisz odkomentować include Symulacja.h
    // Symulacja symulacja;
    // symulacja.setGenerator(generator);
    // symulacja.setModel(model);
    // symulacja.setRegulator(regulator);

    // std::cout << "Uruchamianie symulacji..." << std::endl;
    // symulacja.uruchom(); // Uwaga: w metodzie uruchom() używasz pętli while, co blokuje QCoreApplication

    return 0;
}
#endif
*/
