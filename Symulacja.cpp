#include "Symulacja.h"
#include "ModelARX.h"
#include "RegulatorPID.h"
#include "GeneratorWartosciZadanej.h"
#include "ProstyUAR.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

void Symulacja::uruchomSinus() 
{
    ModelARX arx({ -0.4 }, { 0.6 }, 1, 0.0);
    RegulatorPID pid(0.5, 5.0, 0.2);
    ProstyUAR uar(arx, pid);

    GeneratorWartosciZadanej generator;
    generator.setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_SINUSOIDALNY);
    generator.setAmplituda(1.0);
    generator.setOkresRzeczywisty(2.0);
    generator.setInterwal(200);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Czas[s]\tWart.Zadana\tWart.Regulowana\tUchyb\tSterowanie\n";

    double czas = 0.0;

    while (true)
    {
        double wartoscZadana = generator.generuj();
        double wynik = uar.symuluj(wartoscZadana);

        std::cout << czas << "\t  "
            << wartoscZadana << "\t  "
            << wynik << "\t  "
            << uar.ostatniUchyb() << "\t  "
            << uar.ostatnieSterowanie() << std::endl;

        generator.krokSymulacji();
        czas += 0.2;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void Symulacja::uruchomProstokat()
{
    ModelARX arx({ -0.4 }, { 0.6 }, 1, 0.0);
    RegulatorPID pid(1.0, 2.0, 0.1);
    ProstyUAR uar(arx, pid);

    GeneratorWartosciZadanej generator;
    generator.setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_PROSTOKATNY);
    generator.setAmplituda(2.0);
    generator.setOkresRzeczywisty(3.0);
    generator.setWypelnienie(0.4);
    generator.setInterwal(100);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Czas[s]\tWart.Zadana\tWart.Regulowana\tUchyb\tSterowanie\n";

    double czas = 0.0;

    while (true)
    {
        double wartoscZadana = generator.generuj();
        double wynik = uar.symuluj(wartoscZadana);

        std::cout << czas << "\t  "
            << wartoscZadana << "\t  "
            << wynik << "\t  "
            << uar.ostatniUchyb() << "\t  "
            << uar.ostatnieSterowanie() << std::endl;

        generator.krokSymulacji();
        czas += 0.1;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Symulacja::uruchomStala()
{
    ModelARX arx({ -0.4 }, { 0.6 }, 1, 0.0);
    RegulatorPID pid(0.8, 3.0, 0.05);
    ProstyUAR uar(arx, pid);

    GeneratorWartosciZadanej generator;
    generator.setTypSygnalu(GeneratorWartosciZadanej::SYGNAL_STALY);
    generator.setSkladowaStala(1.5);
    generator.setInterwal(150);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Czas[s]\tWart.Zadana\tWart.Regulowana\tUchyb\tSterowanie\n";

    double czas = 0.0;

    while (true)
    {
        double wartoscZadana = generator.generuj();
        double wynik = uar.symuluj(wartoscZadana);

        std::cout << czas << "\t  "
            << wartoscZadana << "\t  "
            << wynik << "\t  "
            << uar.ostatniUchyb() << "\t  "
            << uar.ostatnieSterowanie() << std::endl;

        generator.krokSymulacji();
        czas += 0.15;
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}