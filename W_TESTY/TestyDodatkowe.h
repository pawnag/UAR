#ifndef TESTYDODATKOWE_H
#define TESTYDODATKOWE_H
#include "W_DANYCH/GeneratorWartosciZadanej.h"
#include "W_DANYCH/RegulatorPID.h"
#include "W_DANYCH/Symulacja.h"
#include <vector>
#include <string>

#include "qobject.h"
namespace TESTY_Dodatkowe {
void wykonaj_wszystkie_testy();

void test_generator_sygnal_staly();
void test_generator_sygnal_sinusoidalny();
void test_generator_sygnal_prostokatny();
void test_generator_zmiana_parametrow();
void test_generator_zerowa_amplituda();

void test_regulator_reset_calki();
void test_regulator_zmiana_trybu_calkowania();
void test_regulator_skladowa_D();
void test_regulator_brak_akcji();

void test_symulacja_tylko_generator();
void test_symulacja_generator_i_model();
void test_symulacja_pelny_uar();
}

class TestyDodatkowe : public QObject

{
    Q_OBJECT
public:
    explicit TestyDodatkowe(QObject *parent = nullptr);

signals:
};

#endif // TESTYDODATKOWE_H
