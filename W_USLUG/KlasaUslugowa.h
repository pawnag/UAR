#ifndef KLASAUSLUGOWA_H
#define KLASAUSLUGOWA_H

#include <QObject>
#include "W_USLUG/Symulacja.h"

class KlasaUslugowa : public QObject
{
    Q_OBJECT
private:
    Symulacja m_symulacja;

public:
    explicit KlasaUslugowa(QObject *parent = nullptr);

    void nowyGenerator(double amplituda,
                       double okres,
                       int interwalMs,
                       GeneratorWartosciZadanej::TypSygnalu typ,
                       double skladowa = 0.0,
                       double wypelnienie = 0.5);

    void nowyModelARX(const std::vector<double>& A,
                      const std::vector<double>& B,
                      int opoznienie = 1,
                      double szum = 0.0);

    void nowyRegulator(double k, double TI, double TD);

    void start();
    void stop();
    void reset();

    double getCzas() const;
    double getWartoscZadana() const;
    double getWartoscWyjscie() const;
    double getSterowanie() const;
    double getUchyb() const;

    // Gettery potrzebne aby wyświetlić w oknie ParametryARX.ui
    std::vector<double> getWektorA() const;
    std::vector<double> getWektorB() const;
    int getOpoznienie() const;
    double getSzum() const;

    void wykonajKrokSymulacji();

signals:
};

#endif // KLASAUSLUGOWA_H
