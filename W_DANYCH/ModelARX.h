#ifndef MODELARX_H
#define MODELARX_H

#include <QObject> // ZMIANA 1: Zamiast QMainWindow
#include <deque>
#include <vector>  // Dodane, bo używasz std::vector
#include <random>
#include <memory>  // Dodane dla unique_ptr

// ZMIANA 2: Dziedziczymy po QObject, a nie QMainWindow
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
    // ZMIANA 3: Rodzicem QObject jest QObject, nie QWidget
    explicit ModelARX(QObject *parent = nullptr);

    ModelARX(const std::vector<double>& i_A,
             const std::vector<double>& i_B,
             int i_op = 1,
             double i_oss = 0.0,
             QObject *parent = nullptr); // Można dodać parent tutaj opcjonalnie

    double symuluj(double i_u);
    void resetuj();

    // Settery i Gettery bez zmian...
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

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);

signals:
    // Tu będziesz mógł kiedyś dodać np: void nowaWartosc(double y);
};

#endif // MODELARX_H
