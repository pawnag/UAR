#ifndef REGULATORPID_H
#define REGULATORPID_H

#include <QObject>
#include <QJsonObject>

class RegulatorPID : public QObject
{
    Q_OBJECT
public:
    enum class LiczCalk { Zew, Wew };

private:
    double m_k;
    double m_TI;
    double m_TD;
    LiczCalk m_trybCalk;
    double m_calka;
    double m_e_prev;

public:
    // KONSTRUKTOR DLA TESTU: Test podaje 3 double, parent jest opcjonalny
    explicit RegulatorPID(double k = 1.0,
                          double TI = 0.0,
                          double TD = 0.0,
                          QObject *parent = nullptr);

    double symuluj(double e);
    void resetuj();

    // Gettery potrzebne do przepisania wartości w ProstyUAR
    double getWzmocnienie() const { return m_k; }
    double getStalaCalk() const { return m_TI; }
    double getStalaRozn() const { return m_TD; }
    LiczCalk getLiczCalk() const { return m_trybCalk; }

    void setWzmocnienie(double k);
    void setStalaCalk(double TI);
    void setStalaRozn(double TD);
    void setLiczCalk(LiczCalk tryb);

};

#endif // REGULATORPID_H
