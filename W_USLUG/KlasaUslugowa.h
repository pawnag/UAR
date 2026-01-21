#ifndef KLASAUSLUGOWA_H
#define KLASAUSLUGOWA_H

#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <vector>
#include "W_DANYCH/Symulacja.h"

class KlasaUslugowa : public QObject
{
    Q_OBJECT

private:
    Symulacja m_symulacja;

public:
    explicit KlasaUslugowa(QObject *parent = nullptr);

    // Sterowanie
    void start();
    void stop();
    void reset();
    void resetPID();
    void setInterwal(int ms);
    bool czyDziala() const;

    // Settery (bez zmian)
    void setGenerator(double amplituda, double okres, int interwalMs,
                      GeneratorWartosciZadanej::TypSygnalu typ,
                      double skladowa, double wypelnienie);
    void setRegulator(double k, double TI, double TD);
    void setPidMetodaCalkowania(int indeks);
    void setModelARX(const std::vector<double>& A, const std::vector<double>& B,
                     int opoznienie, double szum,
                     double u_min, double u_max, double y_min, double y_max);

    // --- GETTERY DANYCH CHWILOWYCH (do wykresów) ---
    double getCzas() const;
    double getWartoscZadana() const;
    double getWartoscWyjscie() const;
    double getSterowanie() const;
    double getUchyb() const;
    double getPidLastP() const;
    double getPidLastI() const;
    double getPidLastD() const;

    // --- NOWE GETTERY KONFIGURACYJNE (Do GUI - typy proste) ---
    // Generator
    double getGenAmplituda() const;
    double getGenOkres() const;
    int    getGenInterwal() const;
    double getGenSkladowa() const;
    double getGenWypelnienie() const;
    int    getGenTyp() const; // Zwraca int (indeks combo boxa)

    // Regulator PID
    double getPidKp() const;
    double getPidTi() const;
    double getPidTd() const;
    int    getPidMetodaCalkowania() const; // Zwraca int (indeks combo boxa)

    // Model ARX (Potrzebne do okna dialogowego, ew. tutaj też proste typy)
    // Zostawiam pobierzModel() jako wyjątek dla okna dialogowego,
    // lub można rozbić na getModelA(), getModelB()...
    ModelARX pobierzModel() const;

    // Serializacja
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);

signals:
    void noweDaneDostepne();

};

#endif // KLASAUSLUGOWA_H
