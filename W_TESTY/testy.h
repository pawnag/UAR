#ifndef TESTY_H
#define TESTY_H

#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

inline int liczbaTestow = 0;
inline int liczbaUdanych = 0;

inline void zarejestrujTest(const std::string& nazwa, bool wynik) {
    liczbaTestow++;
    if (wynik) liczbaUdanych++;
    else std::cerr << "FAIL: " << nazwa << "\n";
}

inline bool porownanieSekwencji(const std::vector<double>& spodz, const std::vector<double>& fakt)
{
    constexpr double TOL = 1e-3;
    if (fakt.size() != spodz.size()) return false;
    for (size_t i = 0; i < fakt.size(); i++)
        if (std::fabs(fakt[i] - spodz[i]) >= TOL) return false;
    return true;
}

inline void raportBleduSekwencji(const std::vector<double>& spodz, const std::vector<double>& fakt)
{
    constexpr size_t PREC = 3;
    std::cerr << std::fixed << std::setprecision(PREC);
    std::cerr << "  Oczekiwany:\t";
    for (auto& el : spodz) std::cerr << el << ", ";
    std::cerr << "\n  Faktyczny:\t";
    for (auto& el : fakt) std::cerr << el << ", ";
    std::cerr << "\n\n";
}

inline void myAssert(const std::string& nazwaTestu,
                     const std::vector<double>& spodz,
                     const std::vector<double>& fakt)
{
    bool wynik = porownanieSekwencji(spodz, fakt);
    zarejestrujTest(nazwaTestu, wynik);

    if (!wynik)
        raportBleduSekwencji(spodz, fakt);
}

inline void wynikiTestow() {
    std::cerr << "\nTesty zakonczone powodzeniem: "
              << liczbaUdanych << "/" << liczbaTestow << "\n";
}

#endif // TESTY_H