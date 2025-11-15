#include "Symulacja.h"
#include "ModelARX.h"
#include "RegulatorPID.h"
#include "GeneratorWartosciZadanej.h"
#include "ProstyUAR.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

void Symulacja::uruchomSinus() {
    std::cout << "Uruchamianie symulacji z sygna³em sinusoidalnym...\n";
    std::cout << "Naciœnij Ctrl+C aby zatrzymaæ\n\n";

    // Inicjalizacja komponentów
    ModelARX arx({ -0.4 }, { 0.6 }, 1, 0.0);
    RegulatorPID pid(0.5, 5.0, 0.2);
    ProstyUAR uar(arx, pid);

    GeneratorWartosciZadanej generator;
    generator.ustawTypSygnalu(GeneratorWartosciZadanej::SYGNAL_SINUSOIDALNY);
    generator.ustawAmplitude(1.0);
    generator.ustawOkres(2.0);
    generator.ustawInterwal(200);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Czas[s]\tWart.Zadana\tWart.Regulowana\tUchyb\tSterowanie\n";

    double czas = 0.0;

    while (true) {
        double wartoscZadana = generator.generuj();
        double wynik = uar.symuluj(wartoscZadana);

        std::cout << czas << "\t"
            << wartoscZadana << "\t"
            << wynik << "\t"
            << uar.ostatniUchyb() << "\t"
            << uar.ostatnieSterowanie() << std::endl;

        generator.aktualizujCzas();
        czas += 0.2;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void Symulacja::uruchomProstokat() {
    std::cout << "Uruchamianie symulacji z sygna³em prostok¹tnym...\n";
    std::cout << "Naciœnij Ctrl+C aby zatrzymaæ\n\n";

    ModelARX arx({ -0.4 }, { 0.6 }, 1, 0.0);
    RegulatorPID pid(1.0, 2.0, 0.1);
    ProstyUAR uar(arx, pid);

    GeneratorWartosciZadanej generator;
    generator.ustawTypSygnalu(GeneratorWartosciZadanej::SYGNAL_PROSTOKATNY);
    generator.ustawAmplitude(2.0);
    generator.ustawOkres(3.0);
    generator.ustawWypelnienie(0.4);
    generator.ustawInterwal(100);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Czas[s]\tWart.Zadana\tWart.Regulowana\tUchyb\tSterowanie\n";

    double czas = 0.0;

    while (true) {
        double wartoscZadana = generator.generuj();
        double wynik = uar.symuluj(wartoscZadana);

        std::cout << czas << "\t"
            << wartoscZadana << "\t"
            << wynik << "\t"
            << uar.ostatniUchyb() << "\t"
            << uar.ostatnieSterowanie() << std::endl;

        generator.aktualizujCzas();
        czas += 0.1;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Symulacja::uruchomStala() {
    std::cout << "Uruchamianie symulacji z wartoœci¹ sta³¹...\n";
    std::cout << "Naciœnij Ctrl+C aby zatrzymaæ\n\n";

    ModelARX arx({ -0.4 }, { 0.6 }, 1, 0.0);
    RegulatorPID pid(0.8, 3.0, 0.05);
    ProstyUAR uar(arx, pid);

    GeneratorWartosciZadanej generator;
    generator.ustawTypSygnalu(GeneratorWartosciZadanej::SYGNAL_STALY);
    generator.ustawSkladowaStala(1.5);
    generator.ustawInterwal(150);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Czas[s]\tWart.Zadana\tWart.Regulowana\tUchyb\tSterowanie\n";

    double czas = 0.0;

    while (true) {
        double wartoscZadana = generator.generuj();
        double wynik = uar.symuluj(wartoscZadana);

        std::cout << czas << "\t"
            << wartoscZadana << "\t"
            << wynik << "\t"
            << uar.ostatniUchyb() << "\t"
            << uar.ostatnieSterowanie() << std::endl;

        generator.aktualizujCzas();
        czas += 0.15;
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}