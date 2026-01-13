#include "W_DANYCH/ModelARX.h"
#include <stdexcept>
#include <algorithm> // Do std::max
#include <QDebug>

// --- 1. KONSTRUKTORY (Poprawione pod QObject) ---
ModelARX::ModelARX(QObject *parent) : QObject(parent)
{
    // Ustawienie wartości domyślnych
    m_ot = 1;
    m_oss = 0.0;
    u_min = -10.0; u_max = 10.0;
    y_min = -10.0; y_max = 10.0;
    m_ograniczenia = true;

    // Inicjalizacja pustych wektorów, żeby nie było błędów przy resize
    m_A = {-0.4};
    m_B = {0.6};

    inicjalizujBufory();
}

ModelARX::ModelARX(const std::vector<double>& i_A,
                   const std::vector<double>& i_B,
                   int i_op,
                   double i_oss,
                   QObject *parent)
    : QObject(parent),
    m_A(i_A), m_B(i_B), m_ot(i_op), m_oss(0.0),
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

// --- 2. LOGIKA SYMULACJI (To Ci zniknęło!) ---

void ModelARX::ustawRozkladSzumu(double odchylenie)
{
    if (odchylenie < 0.0)
    {
        throw std::invalid_argument("Odchylenie standardowe szumu musi być nieujemne");
    }

    m_oss = odchylenie;

    if (odchylenie > 0.0)
    {
        rozklad_szumu = std::make_unique<std::normal_distribution<double>>(0.0, odchylenie);
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

    // Zabezpieczenie przed zerowym rozmiarem buforów
    if (rozmiar_buforu_ster == 0) rozmiar_buforu_ster = 1;
    if (rozmiar_buforu_wyj == 0) rozmiar_buforu_wyj = 1;

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
        if (m_B.size() - 1 - i < m_u.size())
            wyjscie += m_B[i] * m_u[m_B.size() - 1 - i];
    }

    // Część związana z wyjściem (wielomian A)
    for (size_t i = 0; i < m_A.size(); i++)
    {
        if (m_y.size() - 1 - i < m_y.size())
            wyjscie -= m_A[i] * m_y[m_y.size() - 1 - i];
    }

    if (rozklad_szumu)
    {
        wyjscie += (*rozklad_szumu)(generator_losowy);
    }

    return wyjscie;
}

double ModelARX::symuluj(double i_wej)
{
    // 1. Ograniczenie wejścia
    double sterowanie_ograniczone = m_ograniczenia ?
                                        zastosujOgraniczenia(i_wej, u_min, u_max) : i_wej;

    // 2. Aktualizacja bufora sterowania (push front, pop back - jak kolejka przesuwna)
    // UWAGA: W Twoim kodzie było pop_front i push_back, co działa jak kolejka FIFO.
    // Dla równania różnicowego zazwyczaj nowa wartość wchodzi na początek lub koniec.
    // Zakładam, że Twoja logika (m_B.size() - 1 - i) pasuje do push_back.

    m_u.push_back(sterowanie_ograniczone);
    if (m_u.size() > (m_ot + m_B.size())) { // Trzymamy historię
        m_u.pop_front();
    }

    // 3. Obliczenie wyjścia
    double y = obliczWyjscie();

    // 4. Ograniczenie wyjścia
    double y_ograniczone = m_ograniczenia ?
                               zastosujOgraniczenia(y, y_min, y_max) : y;

    // 5. Aktualizacja bufora wyjścia
    m_y.push_back(y_ograniczone);
    if (m_y.size() > m_A.size()) {
        m_y.pop_front();
    }

    return y_ograniczone;
}

void ModelARX::resetuj()
{
    inicjalizujBufory();
}

// --- 3. SETTERY I GETTERY ---

void ModelARX::setA(const std::vector<double>& i_A)
{
    m_A = i_A;

    // NAPRAWA: Jeśli nowy wektor A jest dłuższy, musimy "dopychać" zera na POCZĄTEK.
    // Dzięki temu aktualna historia przesuwa się na właściwe indeksy, a model "widzi" nowe dane.
    while (m_y.size() < m_A.size()) {
        m_y.push_front(0.0);
    }

    // Diagnostyka: Zobaczysz to w konsoli
    //qDebug() << "ModelARX::setA -> Nowy rozmiar A:" << m_A.size() << "Bufor y:" << m_y.size();
}

void ModelARX::setB(const std::vector<double>& i_B)
{
    m_B = i_B;
    size_t wymaganyRozmiar = m_ot + m_B.size();

    // To samo dla sterowania
    while (m_u.size() < wymaganyRozmiar) {
        m_u.push_front(0.0);
    }

    //qDebug() << "ModelARX::setB -> Nowy rozmiar B:" << m_B.size() << "Bufor u:" << m_u.size();
}

void ModelARX::setOpoznienieTransportowe(int i_ot)
{
    if (i_ot < 1) return;
    m_ot = i_ot;

    size_t wymaganyRozmiar = m_ot + m_B.size();
    while (m_u.size() < wymaganyRozmiar) {
        m_u.push_front(0.0);
    }
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

double ModelARX::getUMIN() const { return u_min; };
double ModelARX::getUMAX() const { return u_max; };
double ModelARX::getYMIN() const { return y_min; };
double ModelARX::getYMAX() const { return y_max; };


void ModelARX::aktualizuj(const std::vector<double>& A,
                          const std::vector<double>& B,
                          int opoznienie,
                          double szum)
{
    setA(A);
    setB(B);
    setOpoznienieTransportowe(opoznienie);
    setOdchylenieStandardoweSzumu(szum);
}
