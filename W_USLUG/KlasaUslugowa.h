#ifndef KLASAUSLUGOWA_H
#define KLASAUSLUGOWA_H

#include <QObject>
#include <vector>
#include "W_DANYCH/Symulacja.h"
#include "W_DANYCH/GeneratorWartosciZadanej.h"
#include "W_DANYCH/RegulatorPID.h"

class KlasaUslugowa : public QObject
{
    Q_OBJECT

private:
    Symulacja m_symulacja;

public:
    explicit KlasaUslugowa(QObject *parent = nullptr);

    GeneratorWartosciZadanej pobierzGenerator() const;
    RegulatorPID pobierzRegulator() const;
    ModelARX pobierzModel() const;

    void start();
    void stop();
    void reset();
    void resetPID(); // Resetuje tylko całkę w regulatorze
    void wykonajKrokSymulacji();

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);

    void setGenerator(double amplituda, double okres, int interwalMs,
                      GeneratorWartosciZadanej::TypSygnalu typ,
                      double skladowa = 0.0, double wypelnienie = 0.5);

    void setModelARX(const std::vector<double>& A, const std::vector<double>& B,
                     int opoznienie, double szum,
                     double u_min, double u_max, double y_min, double y_max);

    void setRegulator(double k, double TI, double TD);
    void setPidMetodaCalkowania(int indeks);


    // Symulacja
    double getCzas() const;
    double getWartoscZadana() const;
    double getWartoscWyjscie() const;
    double getSterowanie() const;
    double getUchyb() const;

    // Parametry Modelu
    double getModelUMIN() const;
    double getModelUMAX() const;
    double getModelYMIN() const;
    double getModelYMAX() const;
    std::vector<double> getWektorA() const;
    std::vector<double> getWektorB() const;
    int getOpoznienie() const;
    double getSzum() const;

    // Parametry PID
    double getPidKp() const;
    double getPidTi() const;
    double getPidTd() const;
    int getPidMetodaCalkowania() const;
};

#endif // KLASAUSLUGOWA_H
