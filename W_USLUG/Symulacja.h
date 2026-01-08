#ifndef SYMULACJA_H
#define SYMULACJA_H

#include <QObject> // <--- ZMIANA: QObject zamiast QMainWindow
#include <memory>
#include "W_DANYCH/ModelARX.h"
#include "W_DANYCH/RegulatorPID.h"
#include "W_DANYCH/GeneratorWartosciZadanej.h"
#include "W_USLUG/ProstyUAR.h"

// <--- ZMIANA: Dziedziczymy po QObject
class Symulacja : public QObject
{
    Q_OBJECT
private:
    std::shared_ptr<ModelARX> m_model;
    std::shared_ptr<RegulatorPID> m_regulator;
    std::shared_ptr<GeneratorWartosciZadanej> m_generator;
    std::shared_ptr<ProstyUAR> m_prostyUAR;

    bool m_czyDziala;
    double m_czas;

    double m_wartoscZadana;
    double m_wartoscWyjscie;
    double m_sterowanie;
    double m_uchyb;

    void utworzProstyUAR();

public:
    // <--- ZMIANA: Konstruktor przyjmuje QObject* parent
    explicit Symulacja(QObject *parent = nullptr);

    // Usuń pusty konstruktor Symulacja(), ten powyżej wystarczy (ma wartość domyślną)

    void setModel(std::shared_ptr<ModelARX> i_model);
    void setRegulator(std::shared_ptr<RegulatorPID> i_regulator);
    void setGenerator(std::shared_ptr<GeneratorWartosciZadanej> i_generator);

    void uruchom();
    void zatrzymaj();
    void resetuj();
    void wykonajKrok();

    // Gettery bez zmian...
    double getWartoscZadana() const { return m_wartoscZadana; }
    double getWartoscWyjscie() const { return m_wartoscWyjscie; }
    double getSterowanie() const { return m_sterowanie; }
    double getUchyb() const { return m_uchyb; }
    double getCzas() const { return m_czas; }
    int getInterwalMs() const;

    // Konfiguracje bez zmian...
    void tylkoGenerator();
    void generatorIModel();
    void pelnyUAR();

signals:
    // Tu ewentualnie sygnały o nowym kroku
};

#endif // SYMULACJA_H
