#ifndef PROSTYUAR_H
#define PROSTYUAR_H

#include "W_DANYCH/ModelARX.h"
#include "W_DANYCH/RegulatorPID.h"
#include <QObject> // <--- ZMIANA 1: QObject zamiast QMainWindow

// <--- ZMIANA 2: Dziedziczymy po QObject
class ProstyUAR : public QObject
{
    Q_OBJECT
private:
    ModelARX& m_arx;
    RegulatorPID& m_regulator;
    double m_poprzedniaWartoscRegulowana;
    double m_ostatniUchyb;
    double m_ostatnieSterowanie;

public:
    // <--- ZMIANA 3: Rodzicem jest QObject (nie QWidget)
    explicit ProstyUAR(ModelARX& arx, RegulatorPID& regulator, QObject *parent = nullptr);

    double symuluj(double wartoscZadana);
    void reset();

    double ostatniUchyb() const { return m_ostatniUchyb; }
    double ostatnieSterowanie() const { return m_ostatnieSterowanie; }
    double poprzedniaWartoscRegulowana() const { return m_poprzedniaWartoscRegulowana; }

signals:
    // Tu możesz dodać np. sygnał o przekroczeniu wartości krytycznej
};

#endif // PROSTYUAR_H
