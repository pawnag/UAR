#ifndef REGULATORPID_H
#define REGULATORPID_H

#include <QObject>

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

    double m_lastP = 0.0;
    double m_lastI = 0.0;
    double m_lastD = 0.0;

public:
    explicit RegulatorPID(double k = 1.0,
                          double TI = 0.0,
                          double TD = 0.0,
                          QObject *parent = nullptr);

    double symuluj(double e);
    void resetuj();

    void setWzmocnienie(double k);
    void setStalaCalk(double TI);
    void setStalaRozn(double TD);
    void setLiczCalk(LiczCalk tryb);

    LiczCalk getLiczCalk() const;
    double getWzmocnienie() const;
    double getStalaCalk() const;
    double getStalaRozn() const;
    double getLastP() const;
    double getLastI() const;
    double getLastD() const;


};

#endif // REGULATORPID_H
