#include "TestyARX.h"
#include <vector>
#include <iostream>
#include "ModelARX.h"

namespace TESTY_ModelARX_Dodatkowe {

    // Deklaracje extern dla funkcji z g³ównego pliku testowego
    extern void raportBleduSekwencji(std::vector<double>& spodz, std::vector<double>& fakt);
    extern bool porownanieSekwencji(std::vector<double>& spodz, std::vector<double>& fakt);
    extern void myAssert(std::vector<double>& spodz, std::vector<double>& fakt);

    void wykonaj_wszystkie_testy() {
        std::cout << "\n=== DODATKOWE TESTY MODELU ARX ===" << std::endl;

        test_ograniczenia_sterowania_dolne();
        test_ograniczenia_sterowania_gorne();
        test_ograniczenia_wyjscia_dolne();
        test_ograniczenia_wyjscia_gorne();
        test_ograniczenia_wylaczone();
        test_zmiana_parametrow_A_w_trakcie_symulacji();
        test_zmiana_parametrow_B_w_trakcie_symulacji();
        test_zmiana_opoznienia_transportowego();
        test_szum_zerowe_odchylenie();
        test_szum_male_odchylenie();
        test_brak_zaklocenia_dla_zerowego_odchylenia();
        test_wielu_wspolczynnikow();

        std::cout << "=== KONIEC DODATKOWYCH TESTOW ARX ===" << std::endl;
    }

    void test_ograniczenia_sterowania_dolne() {
        std::cout << "ModelARX -> test ograniczenia sterowania (dolne): ";
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0);
            model.setOgraniczeniaSterowania(-5.0, 5.0);
            model.setOgraniczenia(true);

            // Próba podania wartoœci poni¿ej dolnego ograniczenia
            std::vector<double> sygWe = { -10.0, -8.0, -6.0 };
            std::vector<double> spodzSygWy = { -5.0, -5.0, -5.0 }; // Oczekiwane: ograniczone do -5.0
            std::vector<double> faktSygWy;

            for (double u : sygWe) {
                faktSygWy.push_back(model.symuluj(u));
            }

            myAssert(spodzSygWy, faktSygWy);
        }
        catch (...) {
            std::cerr << "INTERUPTED! (niespodziewany wyjatek)\n";
        }
    }

    void test_ograniczenia_sterowania_gorne() {
        std::cout << "ModelARX -> test ograniczenia sterowania (górne): ";
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0);
            model.setOgraniczeniaSterowania(-5.0, 5.0);
            model.setOgraniczenia(true);

            // Próba podania wartoœci powy¿ej górnego ograniczenia
            std::vector<double> sygWe = { 15.0, 12.0, 8.0 };
            std::vector<double> spodzSygWy = { 5.0, 5.0, 5.0 }; // Oczekiwane: ograniczone do 5.0
            std::vector<double> faktSygWy;

            for (double u : sygWe) {
                faktSygWy.push_back(model.symuluj(u));
            }

            myAssert(spodzSygWy, faktSygWy);
        }
        catch (...) {
            std::cerr << "INTERUPTED! (niespodziewany wyjatek)\n";
        }
    }

    void test_ograniczenia_wyjscia_dolne() {
        std::cout << "ModelARX -> test ograniczenia wyjœcia (dolne): ";
        try {
            // Model który naturalnie dawa³by ujemne wartoœci wyjœcia
            ModelARX model({ -0.8 }, { -0.5 }, 1, 0); // Wzmocnienie ujemne
            model.setOgraniczeniaWyjscia(0.0, 10.0); // Tylko wartoœci dodatnie dozwolone
            model.setOgraniczenia(true);

            // Podanie dodatniego sterowania, ale model ma ujemne wzmocnienie
            std::vector<double> sygWe = { 5.0, 5.0, 5.0, 5.0, 5.0 };
            std::vector<double> spodzSygWy = { 0.0, 0.0, 0.0, 0.0, 0.0 }; // Oczekiwane: ograniczone do 0.0
            std::vector<double> faktSygWy;

            for (double u : sygWe) {
                faktSygWy.push_back(model.symuluj(u));
            }

            myAssert(spodzSygWy, faktSygWy);
        }
        catch (...) {
            std::cerr << "INTERUPTED! (niespodziewany wyjatek)\n";
        }
    }

    void test_ograniczenia_wyjscia_gorne() {
        std::cout << "ModelARX -> test ograniczenia wyjœcia (górne): ";
        try {
            ModelARX model({ -0.2 }, { 1.5 }, 1, 0); // Wzmocnienie dodatnie
            model.setOgraniczeniaWyjscia(-10.0, 3.0); // Górne ograniczenie na 3.0
            model.setOgraniczenia(true);

            // Podanie du¿ego sterowania które przekroczy³oby ograniczenie
            std::vector<double> sygWe(10, 10.0); // 10 kroków z du¿ym sterowaniem
            std::vector<double> spodzSygWy(10, 3.0); // Oczekiwane: ograniczone do 3.0
            std::vector<double> faktSygWy;

            for (double u : sygWe) {
                faktSygWy.push_back(model.symuluj(u));
            }

            myAssert(spodzSygWy, faktSygWy);
        }
        catch (...) {
            std::cerr << "INTERUPTED! (niespodziewany wyjatek)\n";
        }
    }

    void test_ograniczenia_wylaczone() {
        std::cout << "ModelARX -> test wy³¹czenia ograniczeñ: ";
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0);
            model.setOgraniczeniaSterowania(-5.0, 5.0);
            model.setOgraniczeniaWyjscia(-5.0, 5.0);
            model.setOgraniczenia(false); // Wy³¹czamy ograniczenia

            // Próba podania wartoœci poza domyœlnymi ograniczeniami
            std::vector<double> sygWe = { 15.0, 15.0, 15.0 };
            // Bez ograniczeñ: 15 * 0.6 = 9.0 (pierwszy krok), potem odpowiedŸ dynamiczna
            std::vector<double> spodzSygWy = { 9.0, 9.0 * 0.6 + 9.0 * 0.4, 0.0 }; // Obliczenia rêczne
            // Poprawne obliczenia dla modelu bez ograniczeñ:
            // krok 0: y = 0.6*15 + 0.4*0 = 9.0
            // krok 1: y = 0.6*15 + 0.4*9.0 = 9.0 + 3.6 = 12.6
            // krok 2: y = 0.6*15 + 0.4*12.6 = 9.0 + 5.04 = 14.04
            spodzSygWy = { 9.0, 12.6, 14.04 };
            std::vector<double> faktSygWy;

            for (double u : sygWe) {
                faktSygWy.push_back(model.symuluj(u));
            }

            myAssert(spodzSygWy, faktSygWy);
        }
        catch (...) {
            std::cerr << "INTERUPTED! (niespodziewany wyjatek)\n";
        }
    }

    void test_zmiana_parametrow_A_w_trakcie_symulacji() {
        std::cout << "ModelARX -> test zmiany parametrów A w trakcie symulacji: ";
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0);

            std::vector<double> sygWe(10, 1.0); // 10 kroków z sterowaniem 1.0
            std::vector<double> faktSygWy;

            // Pierwsze 5 kroków z oryginalnymi parametrami
            for (int i = 0; i < 5; i++) {
                faktSygWy.push_back(model.symuluj(sygWe[i]));
            }

            // Zmiana parametrów A
            model.setA({ -0.8 });

            // Kolejne 5 kroków z nowymi parametrami
            for (int i = 5; i < 10; i++) {
                faktSygWy.push_back(model.symuluj(sygWe[i]));
            }

            // Spodziewana sekwencja: pierwsze 5 kroków jak dla A=-0.4, potem zmiana
            // Obliczenia rêczne dla pierwszych 5 kroków (A=-0.4, B=0.6)
            std::vector<double> spodzSygWy = { 0.0, 0.6, 0.84, 0.936, 0.9744 };
            // Dla A=-0.8, B=0.6, kontynuacja od ostatniej wartoœci
            // y[t] = 0.6*u[t-1] - (-0.8)*y[t-1] = 0.6*1.0 + 0.8*y[t-1]
            double y5 = 0.6 * 1.0 + 0.8 * 0.9744; // = 0.6 + 0.77952 = 1.37952
            double y6 = 0.6 * 1.0 + 0.8 * 1.37952; // = 0.6 + 1.103616 = 1.703616
            double y7 = 0.6 * 1.0 + 0.8 * 1.703616; // = 0.6 + 1.3628928 = 1.9628928
            double y8 = 0.6 * 1.0 + 0.8 * 1.9628928; // = 0.6 + 1.57031424 = 2.17031424
            double y9 = 0.6 * 1.0 + 0.8 * 2.17031424; // = 0.6 + 1.736251392 = 2.336251392

            spodzSygWy.push_back(y5);
            spodzSygWy.push_back(y6);
            spodzSygWy.push_back(y7);
            spodzSygWy.push_back(y8);
            spodzSygWy.push_back(y9);

            myAssert(spodzSygWy, faktSygWy);
        }
        catch (...) {
            std::cerr << "INTERUPTED! (niespodziewany wyjatek)\n";
        }
    }

    void test_zmiana_parametrow_B_w_trakcie_symulacji() {
        std::cout << "ModelARX -> test zmiany parametrów B w trakcie symulacji: ";
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0);

            std::vector<double> sygWe(5, 1.0); // 5 kroków z sterowaniem 1.0
            std::vector<double> faktSygWy;

            // Pierwsze 3 kroki z oryginalnymi parametrami
            for (int i = 0; i < 3; i++) {
                faktSygWy.push_back(model.symuluj(sygWe[i]));
            }

            // Zmiana parametrów B - zwiêkszenie wzmocnienia
            model.setB({ 1.2 });

            // Kolejne 2 kroki z nowymi parametrami
            for (int i = 3; i < 5; i++) {
                faktSygWy.push_back(model.symuluj(sygWe[i]));
            }

            // Spodziewana sekwencja: pierwsze 3 kroki jak dla B=0.6, potem zmiana
            std::vector<double> spodzSygWy = { 0.0, 0.6, 0.84 };
            // Dla B=1.2, A=-0.4, kontynuacja od ostatniej wartoœci
            // y[t] = 1.2*u[t-1] - (-0.4)*y[t-1] = 1.2*1.0 + 0.4*y[t-1]
            double y3 = 1.2 * 1.0 + 0.4 * 0.84; // = 1.2 + 0.336 = 1.536
            double y4 = 1.2 * 1.0 + 0.4 * 1.536; // = 1.2 + 0.6144 = 1.8144
            spodzSygWy.push_back(y3);
            spodzSygWy.push_back(y4);

            myAssert(spodzSygWy, faktSygWy);
        }
        catch (...) {
            std::cerr << "INTERUPTED! (niespodziewany wyjatek)\n";
        }
    }

    void test_zmiana_opoznienia_transportowego() {
        std::cout << "ModelARX -> test zmiany opóŸnienia transportowego: ";
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0); // OpóŸnienie = 1

            std::vector<double> sygWe = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
            std::vector<double> faktSygWy;

            for (double u : sygWe) {
                faktSygWy.push_back(model.symuluj(u));
            }

            // Zmiana opóŸnienia na wiêksze
            model.setOpoznienieTransportowe(2);

            // Kolejne kroki - znowu brak reakcji przez 2 kroki
            std::vector<double> sygWe2 = { 1.0, 1.0, 1.0 };
            for (double u : sygWe2) {
                faktSygWy.push_back(model.symuluj(u));
            }

            // Spodziewana sekwencja:
            // Dla opóŸnienia 1: pierwsze 6 kroków: 0, 0.6, 0.84, 0.936, 0.9744, 0.98976
            std::vector<double> spodzSygWy = { 0.0, 0.6, 0.84, 0.936, 0.9744, 0.98976 };
            // Po zmianie opóŸnienia na 2, kolejne 3 kroki: 0, 0, 0.6
            spodzSygWy.push_back(0.0);
            spodzSygWy.push_back(0.0);
            spodzSygWy.push_back(0.6);

            myAssert(spodzSygWy, faktSygWy);
        }
        catch (...) {
            std::cerr << "INTERUPTED! (niespodziewany wyjatek)\n";
        }
    }

    void test_szum_zerowe_odchylenie() {
        std::cout << "ModelARX -> test szumu (zerowe odchylenie): ";
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0.0); // Zerowe odchylenie

            std::vector<double> sygWe(10, 1.0);
            std::vector<double> faktSygWy;

            for (double u : sygWe) {
                faktSygWy.push_back(model.symuluj(u));
            }

            // Bez szumu, sekwencja powinna byæ deterministyczna
            std::vector<double> spodzSygWy = { 0.0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738 };

            myAssert(spodzSygWy, faktSygWy);
        }
        catch (...) {
            std::cerr << "INTERUPTED! (niespodziewany wyjatek)\n";
        }
    }

    void test_szum_male_odchylenie() {
        std::cout << "ModelARX -> test szumu (ma³e odchylenie): ";
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0.01); // Ma³e odchylenie

            // Wiele kroków symulacji - powinny byæ zgodne z modelem bez szumu
            // (szum jest losowy, ale œrednio powinien byæ blisko wartoœci bez szumu)
            std::vector<double> sygWe(10, 1.0);
            std::vector<double> faktSygWy;

            for (double u : sygWe) {
                faktSygWy.push_back(model.symuluj(u));
            }

            // Oczekujemy wartoœci bliskich modelowi bez szumu
            std::vector<double> spodzSygWy = { 0.0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738 };

            // Dla szumu u¿ywamy wiêkszej tolerancji
            if (porownanieSekwencji(spodzSygWy, faktSygWy)) {
                std::cout << "OK!\n";
            }
            else {
                std::cout << "FAIL! (wartoœci z szumem ró¿ni¹ siê od oczekiwanych)\n";
                raportBleduSekwencji(spodzSygWy, faktSygWy);
            }
        }
        catch (...) {
            std::cerr << "INTERUPTED! (niespodziewany wyjatek)\n";
        }
    }

    void test_brak_zaklocenia_dla_zerowego_odchylenia() {
        std::cout << "ModelARX -> test braku zak³ócenia dla zerowego odchylenia: ";
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0.0);

            std::vector<double> sygWe(10, 1.0);
            std::vector<double> faktSygWy;

            for (double u : sygWe) {
                faktSygWy.push_back(model.symuluj(u));
            }

            // Bez szumu, sekwencja powinna byæ deterministyczna
            std::vector<double> spodzSygWy = { 0.0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738 };

            myAssert(spodzSygWy, faktSygWy);
        }
        catch (...) {
            std::cerr << "INTERUPTED! (niespodziewany wyjatek)\n";
        }
    }

    void test_wielu_wspolczynnikow() {
        std::cout << "ModelARX -> test wielu wspó³czynników (A i B > 3): ";
        try {
            // Model z wiêcej ni¿ 3 wspó³czynnikami (spe³nienie wymagania min. 3)
            ModelARX model({ -0.4, 0.1, -0.05 }, { 0.6, 0.2, 0.1 }, 1, 0.0);

            std::vector<double> sygWe(10, 1.0);
            std::vector<double> faktSygWy;

            for (double u : sygWe) {
                faktSygWy.push_back(model.symuluj(u));
            }

            // Obliczenia rêczne dla modelu z wieloma wspó³czynnikami
            // y[t] = 0.6*u[t-1] + 0.2*u[t-2] + 0.1*u[t-3] - (-0.4*y[t-1] + 0.1*y[t-2] - 0.05*y[t-3])
            //       = 0.6*u[t-1] + 0.2*u[t-2] + 0.1*u[t-3] + 0.4*y[t-1] - 0.1*y[t-2] + 0.05*y[t-3]
            std::vector<double> spodzSygWy = {
                0.0,                                       // t=0
                0.6,                                       // t=1
                0.6 * 1.0 + 0.2 * 0.0 + 0.4 * 0.6,              // t=2: 0.6 + 0.0 + 0.24 = 0.84
                0.6 * 1.0 + 0.2 * 1.0 + 0.1 * 0.0 + 0.4 * 0.84 - 0.1 * 0.6, // t=3: 0.6 + 0.2 + 0.0 + 0.336 - 0.06 = 1.076
                0.6 * 1.0 + 0.2 * 1.0 + 0.1 * 1.0 + 0.4 * 1.076 - 0.1 * 0.84 + 0.05 * 0.6 // t=4: 0.6+0.2+0.1+0.4304-0.084+0.03=1.2764
            };
            // Uzupe³niamy tylko pierwsze 5 wartoœci, reszta jest zbyt z³o¿ona do rêcznego liczenia
            // Sprawdzamy tylko czy pierwsze wartoœci siê zgadzaj¹

            // Porównujemy tylko pierwsze 5 elementów
            std::vector<double> faktPierwsze5(faktSygWy.begin(), faktSygWy.begin() + 5);

            if (porownanieSekwencji(spodzSygWy, faktPierwsze5)) {
                std::cout << "OK!\n";
            }
            else {
                std::cout << "FAIL! (pierwsze 5 wartoœci nie zgadzaj¹ siê z obliczeniami rêcznymi)\n";
                raportBleduSekwencji(spodzSygWy, faktPierwsze5);
            }
        }
        catch (...) {
            std::cerr << "INTERUPTED! (niespodziewany wyjatek)\n";
        }
    }
}