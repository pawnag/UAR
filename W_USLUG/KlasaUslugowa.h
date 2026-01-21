#ifndef KLASAUSLUGOWA_H
#define KLASAUSLUGOWA_H

#include <QObject>
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
    void setInterwal(int ms); // To było w błędzie, ale musi tu być
    bool czyDziala() const;

    // --- SETTERY KONFIGURACYJNE (Dopasowane nazwy do MainWindow) ---
    void ustawGenerator(double amplituda, double okres, int interwalMs,
                        int typ, // int zamiast enuma dla uproszczenia GUI
                        double skladowa, double wypelnienie);

    void ustawPID(double k, double TI, double TD, int metoda);

    void ustawModel(const std::vector<double>& A, const std::vector<double>& B,
                    int opoznienie, double szum,
                    double u_min, double u_max, double y_min, double y_max);

    // --- GETTERY DANYCH CHWILOWYCH (do wykresów) ---
    double getCzas() const;
    double getWartoscZadana() const;
    double getWartoscWyjscie() const;
    double getSterowanie() const;
    double getUchyb() const;

    // Składowe PID (Dopasowane do MainWindow)
    double getPidLastP() const;
    double getPidLastI() const;
    double getPidLastD() const;

    // --- GETTERY KONFIGURACYJNE ---
    double getGenAmplituda() const;
    double getGenOkres() const;
    int    getGenInterwal() const;
    double getGenSkladowa() const;
    double getGenWypelnienie() const;
    int    getGenTyp() const;

    double getPidKp() const;
    double getPidTi() const;
    double getPidTd() const;
    int    getPidMetodaCalkowania() const;

    // Nowa metoda: Pobiera parametry modelu "luzem" (przez referencje)
    void pobierzModel(std::vector<double>& A, std::vector<double>& B,
                      int& opoznienie, double& szum,
                      double& uMin, double& uMax, double& yMin, double& yMax) const;

    // Serializacja
    void zapiszKonfiguracje(const QString& sciezka);
    void wczytajKonfiguracje(const QString& sciezka);

    // Zachowujemy JSON wewnętrznie
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);

signals:
    // Sygnał "rozbity" na typy proste (zgodnie z życzeniem)
    void noweDane(double czas, double zadana, double wyjscie, double sterowanie, double uchyb);
};

#endif // KLASAUSLUGOWA_H
