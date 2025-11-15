#pragma once

class RegulatorPID
{
public:
    enum class LiczCalk {
        Zew,  // sta³a ca³kowania przed sum¹
        Wew   // sta³a ca³kowania pod sum¹
    };
private:
    double m_k;          // wzmocnienie (nastawa P)
    double m_TI;         // sta³a ca³kowania (nastawa I)
    double m_TD;         // sta³a ró¿niczkowania (nastawa D)
    LiczCalk m_trybCalk; // tryb liczenia sk³adowej ca³kuj¹cej
    double m_calka;      // akumulator dla sk³adowej ca³kuj¹cej
    double m_e_prev;     // poprzednia wartoœæ uchybu

public:
    RegulatorPID(double k, double TI = 0.0, double TD = 0.0);

    double symuluj(double e);
    void resetuj();

    void setWzmocnienie(double k);
    void setStalaCalk(double TI);
    void setStalaRozn(double TD);
    void setLiczCalk(LiczCalk tryb);

    double getWzmocnienie() const { return m_k; }
    double getStalaCalk() const { return m_TI; }
    double getStalaRozn() const { return m_TD; }
    LiczCalk getLiczCalk() const { return m_trybCalk; }
};