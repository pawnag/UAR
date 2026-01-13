#ifndef MODELARX_H
#define MODELARX_H

#include <QObject>
#include <deque>
#include <vector>
#include <random>
#include <memory>

class ModelARX : public QObject
{
    Q_OBJECT
private:
    std::vector<double> m_A;
    std::vector<double> m_B;
    std::deque<double> m_u;
    std::deque<double> m_y;
    int m_ot;
    double m_oss;

    double u_min, u_max;
    double y_min, y_max;
    bool m_ograniczenia;

    std::default_random_engine generator_losowy;
    std::unique_ptr<std::normal_distribution<double>> rozklad_szumu;

    void inicjalizujBufory();
    double zastosujOgraniczenia(double i_wart, double i_min, double i_max);
    double obliczWyjscie();
    void ustawRozkladSzumu(double odchylenie);

public:
    explicit ModelARX(QObject *parent = nullptr);

    // KONSTRUKTOR DLA TESTU: Dzięki wartościom domyślnym test może podać tylko A i B
    ModelARX(const std::vector<double>& i_A,
             const std::vector<double>& i_B,
             int i_op = 1,              // Domyślne opóźnienie
             double i_oss = 0.0,        // Domyślny brak szumu
             QObject *parent = nullptr); // Domyślny rodzic

    double symuluj(double i_u);
    void resetuj();

    // Gettery potrzebne do przepisania wartości w ProstyUAR
    std::vector<double> getA() const;
    std::vector<double> getB() const;
    int getOpoznienieTransportowe() const;
    double getOdchylenieStandardoweSzumu() const;

    double getUMIN() const;
    double getUMAX() const;
    double getYMIN() const;
    double getYMAX() const;

    void aktualizuj(const std::vector<double>& A,
               const std::vector<double>& B,
               int opoznienie,
                    double szum);

    // Settery...
    void setA(const std::vector<double>& i_A);
    void setB(const std::vector<double>& i_B);
    void setOpoznienieTransportowe(int i_ot);
    void setOdchylenieStandardoweSzumu(double i_oss);
    void setOgraniczeniaSterowania(double i_umin, double i_umax);
    void setOgraniczeniaWyjscia(double i_ymin, double i_ymax);
    void setOgraniczenia(bool i_ograniczenia);

};

#endif // MODELARX_H
