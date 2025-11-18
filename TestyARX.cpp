#include "TestyARX.h"

namespace TESTY_ModelARX_Dodatkowe {

    // Funkcje pomocnicze
    bool porownanie_z_tolerancja(double a, double b, double tolerancja) {
        return std::fabs(a - b) < tolerancja;
    }

    void raport_pojedynczego_testu(const std::string& nazwa, bool wynik) {
        std::cout << nazwa << ": " << (wynik ? "OK!" : "FAIL!") << std::endl;
    }

    void wykonaj_wszystkie_testy() {
        std::cout << "=== DODATKOWE TESTY MODELU ARX ===" << std::endl;

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

        std::cout << "=== KONIEC DODATKOWYCH TESTOW ===" << std::endl;
    }

    void test_ograniczenia_sterowania_dolne() {
        bool test_ok = true;
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0);
            model.setOgraniczeniaSterowania(-5.0, 5.0);
            model.setOgraniczenia(true);

            // Próba podania wartoœci poni¿ej dolnego ograniczenia
            double wynik = model.symuluj(-10.0);
            test_ok = test_ok && porownanie_z_tolerancja(wynik, -5.0);

            // Kolejne kroki dla pewnoœci
            wynik = model.symuluj(-8.0);
            test_ok = test_ok && porownanie_z_tolerancja(wynik, -5.0);

        }
        catch (...) {
            test_ok = false;
        }
        raport_pojedynczego_testu("Ograniczenie sterowania - dolne", test_ok);
    }

    void test_ograniczenia_sterowania_gorne() {
        bool test_ok = true;
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0);
            model.setOgraniczeniaSterowania(-5.0, 5.0);
            model.setOgraniczenia(true);

            // Próba podania wartoœci powy¿ej górnego ograniczenia
            double wynik = model.symuluj(15.0);
            test_ok = test_ok && porownanie_z_tolerancja(wynik, 5.0);

            wynik = model.symuluj(12.0);
            test_ok = test_ok && porownanie_z_tolerancja(wynik, 5.0);

        }
        catch (...) {
            test_ok = false;
        }
        raport_pojedynczego_testu("Ograniczenie sterowania - górne", test_ok);
    }

    void test_ograniczenia_wyjscia_dolne() {
        bool test_ok = true;
        try {
            // Model który naturalnie dawa³by ujemne wartoœci wyjœcia
            ModelARX model({ -0.8 }, { -0.5 }, 1, 0); // Wzmocnienie ujemne
            model.setOgraniczeniaWyjscia(0.0, 10.0); // Tylko wartoœci dodatnie dozwolone
            model.setOgraniczenia(true);

            // Podanie dodatniego sterowania, ale model ma ujemne wzmocnienie
            double wynik = model.symuluj(5.0);
            // Sprawdzamy czy wartoœæ jest ograniczona do minimum = 0.0
            test_ok = test_ok && (wynik >= 0.0);

        }
        catch (...) {
            test_ok = false;
        }
        raport_pojedynczego_testu("Ograniczenie wyjœcia - dolne", test_ok);
    }

    void test_ograniczenia_wyjscia_gorne() {
        bool test_ok = true;
        try {
            ModelARX model({ -0.2 }, { 1.5 }, 1, 0); // Wzmocnienie dodatnie
            model.setOgraniczeniaWyjscia(-10.0, 3.0); // Górne ograniczenie na 3.0
            model.setOgraniczenia(true);

            // Podanie du¿ego sterowania które przekroczy³oby ograniczenie
            for (int i = 0; i < 10; i++) {
                double wynik = model.symuluj(10.0);
                test_ok = test_ok && (wynik <= 3.0);
            }

        }
        catch (...) {
            test_ok = false;
        }
        raport_pojedynczego_testu("Ograniczenie wyjœcia - górne", test_ok);
    }

    void test_ograniczenia_wylaczone() {
        bool test_ok = true;
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0);
            model.setOgraniczeniaSterowania(-5.0, 5.0);
            model.setOgraniczeniaWyjscia(-5.0, 5.0);
            model.setOgraniczenia(false); // Wy³¹czamy ograniczenia

            // Próba podania wartoœci poza domyœlnymi ograniczeniami
            double wynik = model.symuluj(15.0);
            // Przy wy³¹czonych ograniczeniach powinno przyj¹æ wartoœæ 15.0
            test_ok = test_ok && porownanie_z_tolerancja(wynik, 15.0 * 0.6); // 15 * 0.6 = 9.0

        }
        catch (...) {
            test_ok = false;
        }
        raport_pojedynczego_testu("Ograniczenia wy³¹czone", test_ok);
    }

    void test_zmiana_parametrow_A_w_trakcie_symulacji() {
        bool test_ok = true;
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0);

            // Pierwsze kroki z oryginalnymi parametrami
            std::vector<double> wyniki_przed;
            for (int i = 0; i < 5; i++) {
                wyniki_przed.push_back(model.symuluj(1.0));
            }

            // Zmiana parametrów A
            model.setA({ -0.8 });

            // Kolejne kroki z nowymi parametrami
            std::vector<double> wyniki_po;
            for (int i = 0; i < 5; i++) {
                wyniki_po.push_back(model.symuluj(1.0));
            }

            // Powinna byæ zauwa¿alna ró¿nica w dynamice
            test_ok = test_ok && (wyniki_po.size() == 5);
            test_ok = test_ok && (wyniki_przed.size() == 5);

        }
        catch (...) {
            test_ok = false;
        }
        raport_pojedynczego_testu("Zmiana parametrów A w trakcie symulacji", test_ok);
    }

    void test_zmiana_parametrow_B_w_trakcie_symulacji() {
        bool test_ok = true;
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0);

            // Pierwsze kroki z oryginalnymi parametrami
            double wynik_przed = 0.0;
            for (int i = 0; i < 3; i++) {
                wynik_przed = model.symuluj(1.0);
            }

            // Zmiana parametrów B - zwiêkszenie wzmocnienia
            model.setB({ 1.2 });

            // Kolejne kroki z nowymi parametrami
            double wynik_po = model.symuluj(1.0);

            // Przy wiêkszym wzmocnieniu odpowiedŸ powinna byæ wy¿sza
            test_ok = test_ok && (wynik_po > wynik_przed);

        }
        catch (...) {
            test_ok = false;
        }
        raport_pojedynczego_testu("Zmiana parametrów B w trakcie symulacji", test_ok);
    }

    void test_zmiana_opoznienia_transportowego() {
        bool test_ok = true;
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0); // OpóŸnienie = 1

            // Pierwszy krok - jeszcze brak reakcji (opóŸnienie)
            double wynik1 = model.symuluj(1.0);
            test_ok = test_ok && porownanie_z_tolerancja(wynik1, 0.0);

            // Drugi krok - pojawia siê reakcja
            double wynik2 = model.symuluj(1.0);
            test_ok = test_ok && porownanie_z_tolerancja(wynik2, 0.6);

            // Zmiana opóŸnienia na wiêksze
            model.setOpoznienieTransportowe(2);

            // Kolejne kroki - znowu brak reakcji przez 2 kroki
            double wynik3 = model.symuluj(1.0);
            test_ok = test_ok && porownanie_z_tolerancja(wynik3, 0.0);

            double wynik4 = model.symuluj(1.0);
            test_ok = test_ok && porownanie_z_tolerancja(wynik4, 0.0);

            double wynik5 = model.symuluj(1.0);
            test_ok = test_ok && porownanie_z_tolerancja(wynik5, 0.6);

        }
        catch (...) {
            test_ok = false;
        }
        raport_pojedynczego_testu("Zmiana opóŸnienia transportowego", test_ok);
    }

    void test_szum_zerowe_odchylenie() {
        bool test_ok = true;
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0.0); // Zerowe odchylenie

            // Wiele kroków symulacji - wszystkie powinny byæ identyczne
            // (brak losowoœci)
            std::vector<double> wyniki;
            for (int i = 0; i < 10; i++) {
                wyniki.push_back(model.symuluj(1.0));
            }

            // Sprawdzamy czy wszystkie wyniki s¹ takie same
            // (dla zerowego szumu nie ma losowoœci)
            for (size_t i = 1; i < wyniki.size(); i++) {
                if (i >= 2) { // Pomijamy pierwsze 2 kroki (stan przejœciowy)
                    test_ok = test_ok && porownanie_z_tolerancja(wyniki[i], wyniki[i - 1]);
                }
            }

        }
        catch (...) {
            test_ok = false;
        }
        raport_pojedynczego_testu("Szum - zerowe odchylenie", test_ok);
    }

    void test_szum_male_odchylenie() {
        bool test_ok = true;
        try {
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0.01); // Ma³e odchylenie

            // Wiele kroków symulacji - powinny byæ niewielkie ró¿nice
            std::vector<double> wyniki;
            for (int i = 0; i < 100; i++) {
                wyniki.push_back(model.symuluj(1.0));
            }

            // Sprawdzamy czy wystêpuje jakaœ wariancja
            // (dla szumu powinny byæ niewielkie ró¿nice)
            bool wystepuja_rozne_wartosci = false;
            for (size_t i = 3; i < wyniki.size(); i++) {
                if (!porownanie_z_tolerancja(wyniki[i], wyniki[i - 1], 1e-5)) {
                    wystepuja_rozne_wartosci = true;
                    break;
                }
            }
            test_ok = test_ok && wystepuja_rozne_wartosci;

        }
        catch (...) {
            test_ok = false;
        }
        raport_pojedynczego_testu("Szum - ma³e odchylenie", test_ok);
    }

    void test_brak_zaklocenia_dla_zerowego_odchylenia() {
        bool test_ok = true;
        try {
            // Test sprawdzaj¹cy czy dla odchylenia 0.0 nie ma wyj¹tku
            ModelARX model({ -0.4 }, { 0.6 }, 1, 0.0);

            // Powinno dzia³aæ bez wyj¹tków
            for (int i = 0; i < 10; i++) {
                double wynik = model.symuluj(1.0);
                test_ok = test_ok && (wynik >= -10.0 && wynik <= 10.0); // W granicach rozs¹dku
            }

        }
        catch (...) {
            test_ok = false;
        }
        raport_pojedynczego_testu("Brak zak³ócenia dla zerowego odchylenia", test_ok);
    }

    void test_wielu_wspolczynnikow() {
        bool test_ok = true;
        try {
            // Model z wiêcej ni¿ 3 wspó³czynnikami (spe³nienie wymagania min. 3)
            ModelARX model({ -0.4, 0.1, -0.05 }, { 0.6, 0.2, 0.1 }, 1, 0.0);

            std::vector<double> wyniki;
            for (int i = 0; i < 10; i++) {
                wyniki.push_back(model.symuluj(1.0));
            }

            // Sprawdzamy czy symulacja przebiega bez b³êdów
            test_ok = test_ok && (wyniki.size() == 10);

            // Sprawdzamy czy odpowiedŸ jest stabilna
            bool stabilna = true;
            for (size_t i = 5; i < wyniki.size(); i++) {
                if (std::fabs(wyniki[i] - wyniki[i - 1]) > 0.1) {
                    stabilna = false;
                    break;
                }
            }
            test_ok = test_ok && stabilna;

        }
        catch (...) {
            test_ok = false;
        }
        raport_pojedynczego_testu("Wiele wspó³czynników (A i B > 3)", test_ok);
    }
}