#pragma once

#include <vector>
#include <deque>
#include <random>

class ModelARX
{
private:
    // vectory nie maj¹ pop i push front dlatego ró¿nica
    std::vector<double> m_A;
    std::vector<double> m_B;
    std::deque<double> m_u; // Bufor wartoœci steruj¹cej
    std::deque<double> m_y; // Bufor wartoœci regulowanej
    int m_ot; // OpóŸnienie transportowe
    double m_oss; // Odchylenie standardowe szumu

    double u_min, u_max;
    double y_min, y_max;
    bool m_ograniczenia;

    std::default_random_engine generator_losowy;
    std::unique_ptr<std::normal_distribution<double>> rozklad_szumu;
    /*
    UNIQUE_PTR - inteligentny wskaŸnik
    - Tylko JEDEN unique_ptr mo¿e posiadaæ dany obiekt
    - Nie mo¿na go kopiowaæ, tylko przenosiæ
    - Gdy unique_ptr jest niszczony, automatycznie usuwa obiekt
    NORMAL_DISTRIBUTION - rozk³ad normalny Gaussa
    */
    void inicjalizujBufory();
    double zastosujOgraniczenia(double i_wart, double i_min, double i_max);
    double obliczWyjscie();

    void ustawRozkladSzumu(double odchylenie);

public:
    ModelARX(const std::vector<double>& i_A,
        const std::vector<double>& i_B,
        int i_op = 1,
        double i_oss = 0.0);

    ModelARX(const ModelARX&) = delete;
    ModelARX& operator=(const ModelARX&) = delete;

    double symuluj(double i_u);
    void resetuj();

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
};