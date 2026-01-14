#ifndef MODELARX_H
#define MODELARX_H

#include <deque>
#include <vector>
#include <random>
#include <memory>

class ModelARX
{
private:
    std::vector<double> m_A;
    std::vector<double> m_B;
    std::deque<double> m_u; // Bufor wejścia
    std::deque<double> m_y; // Bufor wyjścia
    int m_ot;               // Opóźnienie
    double m_oss;           // Szum

    double u_min, u_max;
    double y_min, y_max;
    bool m_ograniczenia;

    std::default_random_engine generator_losowy;
    std::normal_distribution<double> rozklad_szumu;

    void inicjalizujBufory();
    double obliczWyjscie();
    void ustawRozkladSzumu(double odchylenie);

public:
    ModelARX(const std::vector<double>& i_A = {-0.4, 0.0, 0.0},
             const std::vector<double>& i_B = {0.6, 0.0, 0.0},
             int i_op = 1,
             double i_oss = 0.0);

    double symuluj(double i_u);
    void resetuj();
    void aktualizuj(const std::vector<double>& A, const std::vector<double>& B, int opoznienie, double szum);

    void setA(const std::vector<double>& i_A);
    void setB(const std::vector<double>& i_B);
    void setOpoznienieTransportowe(int i_ot);
    void setOdchylenieStandardoweSzumu(double i_oss);
    void setOgraniczeniaSterowania(double i_umin, double i_umax);
    void setOgraniczeniaWyjscia(double i_ymin, double i_ymax);
    void setOgraniczenia(bool i_ograniczenia);

    std::vector<double> getA() const;
    std::vector<double> getB() const;
    int getOpoznienieTransportowe() const;
    double getOdchylenieStandardoweSzumu() const;
    double getUMIN() const;
    double getUMAX() const;
    double getYMIN() const;
    double getYMAX() const;
};

#endif // MODELARX_H
