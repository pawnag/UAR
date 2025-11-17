#include "ModelARX.h"
#include <algorithm>
#include <stdexcept>
#include <random>
#include <cmath>
#include <memory>

ModelARX::ModelARX(const std::vector<double>& i_A,
    const std::vector<double>& i_B,
    int i_op,
    double i_oss)
    : m_A(i_A), m_B(i_B), m_ot(i_op), m_oss(0.0),
    u_min(-10.0), u_max(10.0), y_min(-10.0), y_max(10.0),
    m_ograniczenia(true),
    rozklad_szumu(nullptr)
{
    if (i_A.empty() || i_B.empty())
    {
        throw std::invalid_argument("Wymagany co najmniej 1 wspolczynnik w kazdym wielomianie");
    }
    if (i_op < 1)
    {
        throw std::invalid_argument("Opoznienie transportowe musi byc >= 1");
    }

    ustawRozkladSzumu(i_oss);
    inicjalizujBufory();
    generator_losowy.seed(std::random_device{}());
}

void ModelARX::ustawRozkladSzumu(double odchylenie)
{
    if (odchylenie < 0.0) 
    {
        throw std::invalid_argument("Odchylenie standardowe szumu musi być nieujemne");
    }

    m_oss = odchylenie;

    if (odchylenie > 0.0) 
    {
        // std::make_unique to bezpieczny sposób tworzenia obiektów na stercie.
        // std::make_unique<Type>(argumenty...) tworzy:
        // 1. Obiekt Type na stercie
        // 2. Owija go w unique_ptr
        // 3. Zwraca unique_ptr
        rozklad_szumu = std::make_unique<std::normal_distribution<double>>(0.0, odchylenie); // 0.0 to srednia
    }
    else 
    {
        rozklad_szumu.reset();
    }
}

void ModelARX::inicjalizujBufory()
{
    size_t rozmiar_buforu_ster = m_ot + m_B.size() - 1;
    size_t rozmiar_buforu_wyj = m_A.size();

    m_u.clear();
    m_y.clear();

    for (size_t i = 0; i < rozmiar_buforu_ster; i++) 
    {
        m_u.push_back(0.0);
    }
    for (size_t i = 0; i < rozmiar_buforu_wyj; i++) 
    {
        m_y.push_back(0.0);
    }
}

double ModelARX::zastosujOgraniczenia(double i_wart, double i_min, double i_max)
{
    if (i_wart < i_min) return i_min;
    if (i_wart > i_max) return i_max;
    return i_wart;
}

double ModelARX::obliczWyjscie()
{
    double wyjscie = 0.0;

    // Część związana z wejściem (wielomian B)
    for (size_t i = 0; i < m_B.size(); i++) 
    {
        wyjscie += m_B[i] * m_u[m_B.size() - 1 - i];
    }

    // Część związana z wyjściem (wielomian A)
    for (size_t i = 0; i < m_A.size(); i++) 
    {
        wyjscie -= m_A[i] * m_y[m_y.size() - 1 - i];
    }

    if (rozklad_szumu) 
    {
        wyjscie += (*rozklad_szumu)(generator_losowy); // Dereferencja wskaźnika i wywołanie operatora()
    }

    return wyjscie;
}

double ModelARX::symuluj(double i_wej)
{
    // 1. SPRAWDZENIE OGRANICZEŃ STEROWANIA PRZED JAKIMIKOLWIEK OBLICZENIAMI
    double sterowanie_ograniczone = m_ograniczenia ?
        zastosujOgraniczenia(i_wej, u_min, u_max) : i_wej;

    // 2. OBLICZENIE WYJŚCIA NA PODSTAWIE OBECNYCH BUFORÓW (przed aktualizacją!)
    double y = obliczWyjscie();

    // 3. SPRAWDZENIE OGRANICZEŃ WARTOŚCI REGULOWANEJ ZARAZ PO WYLICZENIU
    double y_ograniczone = m_ograniczenia ?
        zastosujOgraniczenia(y, y_min, y_max) : y;

    // 4. AKTUALIZACJA BUFORU WARTOŚCI REGULOWANEJ (PRZED zapisaniem do bufora)
    if (!m_y.empty()) m_y.pop_front();
    m_y.push_back(y_ograniczone);

    // 5. AKTUALIZACJA BUFORU STEROWANIA (PO obliczeniach - dla następnego kroku)
    m_u.pop_front();
    m_u.push_back(sterowanie_ograniczone);

    return y_ograniczone;
}

void ModelARX::resetuj()
{
    inicjalizujBufory();
}

void ModelARX::setA(const std::vector<double>& i_A)
{
    if (i_A.empty())
    {
        throw std::invalid_argument("Wymagany co najmniej 1 wspolczynnik A");
    }
    m_A = i_A;
    size_t nowy_rozmiar = m_A.size();
    while (m_y.size() < nowy_rozmiar)
    {
        m_y.push_back(0.0);
    }
    while (m_y.size() > nowy_rozmiar)
    {
        m_y.pop_front();
    }
}

void ModelARX::setB(const std::vector<double>& i_B)
{
    if (i_B.empty()) 
    {
        throw std::invalid_argument("Wymagany co najmniej 1 wspolczynnik B");
    }
    m_B = i_B;
    size_t nowy_rozmiar = m_ot + m_B.size() - 1;
    while (m_u.size() < nowy_rozmiar) 
    {
        m_u.push_back(0.0);
    }
    while (m_u.size() > nowy_rozmiar) 
    {
        m_u.pop_front();
    }
}

void ModelARX::setOpoznienieTransportowe(int i_ot)
{
    if (i_ot < 1) 
    {
        throw std::invalid_argument("Opoznienie transportowe musi byc >= 1");
    }
    m_ot = i_ot;
    inicjalizujBufory();
}

void ModelARX::setOdchylenieStandardoweSzumu(double i_oss)
{
    ustawRozkladSzumu(i_oss);
}

void ModelARX::setOgraniczeniaSterowania(double i_umin, double i_umax)
{
    u_min = i_umin;
    u_max = i_umax;
}

void ModelARX::setOgraniczeniaWyjscia(double i_ymin, double i_ymax)
{
    y_min = i_ymin;
    y_max = i_ymax;
}

void ModelARX::setOgraniczenia(bool i_ograniczenia)
{
    m_ograniczenia = i_ograniczenia;
}

std::vector<double> ModelARX::getA() const { return m_A; }
std::vector<double> ModelARX::getB() const { return m_B; }
int ModelARX::getOpoznienieTransportowe() const { return m_ot; }
double ModelARX::getOdchylenieStandardoweSzumu() const { return m_oss; }