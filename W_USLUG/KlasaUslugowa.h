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

    // --- DOSTĘP DO OBIEKTÓW (KOPIE) ---
    // Służą do pobrania całego stanu obiektu (np. do GUI lub zapisu JSON)
    GeneratorWartosciZadanej pobierzGenerator() const;
    RegulatorPID pobierzRegulator() const;
    ModelARX pobierzModel() const; // Jeśli potrzebujesz całego modelu

    // --- STEROWANIE ---
    void start();
    void stop();
    void reset();
    void resetPID(); // Resetuje tylko całkę w regulatorze
    void wykonajKrokSymulacji();

    // --- SERIALIZACJA (JSON) ---
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);

    // --- KONFIGURACJA (SETTERY) ---
    void setGenerator(double amplituda, double okres, int interwalMs,
                      GeneratorWartosciZadanej::TypSygnalu typ,
                      double skladowa = 0.0, double wypelnienie = 0.5);

    void setModelARX(const std::vector<double>& A, const std::vector<double>& B,
                     int opoznienie, double szum,
                     double u_min, double u_max, double y_min, double y_max);

    void setRegulator(double k, double TI, double TD);
    void setPidMetodaCalkowania(int indeks);

    // --- ODCZYT DANYCH BIEŻĄCYCH (GETTERY WARTOŚCI) ---

    // Symulacja
    double getCzas() const;
    double getWartoscZadana() const;
    double getWartoscWyjscie() const;
    double getSterowanie() const;
    double getUchyb() const;

    // Parametry Modelu (pomocnicze)
    double getModelUMIN() const;
    double getModelUMAX() const;
    double getModelYMIN() const;
    double getModelYMAX() const;
    std::vector<double> getWektorA() const;
    std::vector<double> getWektorB() const;
    int getOpoznienie() const;
    double getSzum() const;

    // Parametry PID (pomocnicze)
    double getPidKp() const;
    double getPidTi() const;
    double getPidTd() const;
    int getPidMetodaCalkowania() const;
};

#endif // KLASAUSLUGOWA_H
