#ifndef REGULATORPID_H
#define REGULATORPID_H

class RegulatorPID
{
public:
    enum class LiczCalk { Zew, Wew };

private:
    LiczCalk m_trybCalk;
    double m_k;
    double m_TI;
    double m_TD;
    double m_calka;
    double m_e_prev;

    double m_lastP = 0.0;
    double m_lastI = 0.0;
    double m_lastD = 0.0;
public:
    RegulatorPID(double k = 0.0, double TI = 0.0, double TD = 0.0);

    double symuluj(double e);
    void resetuj();

    void setLiczCalk(LiczCalk tryb);
    void setWzmocnienie(double k);
    void setStalaCalk(double TI);
    void setStalaRozn(double TD);

    LiczCalk getLiczCalk() const;
    double getWzmocnienie() const;
    double getStalaCalk() const;
    double getStalaRozn() const;
    double getLastP() const;
    double getLastI() const;
    double getLastD() const;
};

#endif // REGULATORPID_H
