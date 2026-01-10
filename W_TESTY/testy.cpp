#include <iostream>

#include "testy.h"
#include "W_TESTY/TestyDodatkowe.h"
#include "W_TESTY/Testy_UAR.h"

#include "W_DANYCH/ModelARX.h"
#include "W_DANYCH/RegulatorPID.h"
#include "W_DANYCH/GeneratorWartosciZadanej.h"
#include "W_USLUG/ProstyUAR.h"
#include "W_USLUG/Symulacja.h" // Odkomentuj jeśli chcesz testować klasę Symulacja
#include "W_TESTY/TestyDodatkowe.h"      // Odkomentuj jeśli masz ten plik

Testy::Testy() {

}

void uruchomWszystkieTesty()
{
    std::cerr << "=== ROZPOCZYNAM TESTY JEDNOSTKOWE ===\n\n";

    TESTY_ModelARX::wykonaj_testy();
    TESTY_RegulatorPID::wykonaj_testy();
    TESTY_ProstyUAR::wykonaj_testy();

    TESTY_Dodatkowe::wykonaj_wszystkie_testy();

    std::cerr << "=== KONIEC TESTOW ===\n";
}
