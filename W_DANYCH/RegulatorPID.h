#ifndef REGULATORPID_H
#define REGULATORPID_H

#include <QObject> // ZMIANA: QObject zamiast QMainWindow

class RegulatorPID : public QObject
{
    Q_OBJECT
public:
    enum class LiczCalk {
        Zew,  // stała całkowania przed sumą
        Wew   // stała całkowania pod sumą
    };

private:
    double m_k;          // wzmocnienie (nastawa P)
    double m_TI;         // stała całkowania (nastawa I)
    double m_TD;         // stała różniczkowania (nastawa D)
    LiczCalk m_trybCalk; // tryb liczenia składowej całkującej
    double m_calka;      // akumulator dla składowej całkującej
    double m_e_prev;     // poprzednia wartość uchybu

public:
    // ZMIANA: Jeden scalony konstruktor.
    // Domyślne wartości pozwalają tworzyć obiekt jako:
    // new RegulatorPID(1.0, 2.0, 0.5)  LUB  new RegulatorPID(1.0, 2.0, 0.5, parent)
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

    double getWzmocnienie() const { return m_k; }
    double getStalaCalk() const { return m_TI; }
    double getStalaRozn() const { return m_TD; }
    LiczCalk getLiczCalk() const { return m_trybCalk; }

signals:
    // Miejsce na ewentualne sygnały w przyszłości
};

#endif // REGULATORPID_H
