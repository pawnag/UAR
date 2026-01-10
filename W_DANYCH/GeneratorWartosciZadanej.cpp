#include "W_DANYCH/GeneratorWartosciZadanej.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>


// ZMIANA: Scalony konstruktor inicjalizujący QObject i zmienne
GeneratorWartosciZadanej::GeneratorWartosciZadanej(QObject *parent)
    : QObject(parent), // Inicjalizacja rodzica
    m_typ(SYGNAL_STALY),
    m_amplituda(1.0),
    m_okresDyskretny(5),
    m_skladowaStala(0.0),
    m_wypelnienie(0.5),
    m_licznikKrokow(0),
    m_okresRzeczywisty(1.0),
    m_interwal(200)
{
    // Konstruktor jest pusty, bo wszystko jest na liście inicjalizacyjnej
}

void GeneratorWartosciZadanej::setOkresRzeczywisty(double T_RZ)
{
    m_okresRzeczywisty = T_RZ;
    przeliczOkresDyskretny();
}

void GeneratorWartosciZadanej::setInterwal(int T_T_ms)
{
    m_interwal = T_T_ms;
    przeliczOkresDyskretny();
}

void GeneratorWartosciZadanej::przeliczOkresDyskretny()
{
    if (m_interwal > 0 && m_okresRzeczywisty > 0)
    {
        double interwalSekundy = m_interwal / 1000.0;
        double T = m_okresRzeczywisty / interwalSekundy;
        m_okresDyskretny = static_cast<int>(std::round(T));
        m_okresDyskretny = std::max(1, m_okresDyskretny);
    }
    else
    {
        m_okresDyskretny = 1;
    }
}

double GeneratorWartosciZadanej::generuj()
{
    switch (m_typ)
    {
    case SYGNAL_SINUSOIDALNY:
    {
        if (m_okresDyskretny <= 0) return m_skladowaStala;
        double faza = (m_licznikKrokow % m_okresDyskretny) / static_cast<double>(m_okresDyskretny) * 2.0 * M_PI;
        return m_amplituda * std::sin(faza) + m_skladowaStala;
    }

    case SYGNAL_PROSTOKATNY:
    {
        if (m_okresDyskretny <= 0) return m_skladowaStala;
        int pozycjaWOkresie = m_licznikKrokow % m_okresDyskretny;
        double prog = m_wypelnienie * m_okresDyskretny;
        return (pozycjaWOkresie < prog) ? (m_amplituda + m_skladowaStala) : m_skladowaStala;
    }

    case SYGNAL_STALY:
    default:
        return m_skladowaStala;
    }
}

QJsonObject GeneratorWartosciZadanej::toJson() const
{
    QJsonObject obj;

    obj["typ"] = static_cast<int>(m_typ);
    obj["amplituda"] = m_amplituda;
    obj["okresDyskretny"] = m_okresDyskretny;
    obj["skladowaStala"] = m_skladowaStala;
    obj["wypelnienie"] = m_wypelnienie;
    obj["okresRzeczywisty"] = m_okresRzeczywisty;
    obj["interwal"] = m_interwal;

    return obj;
}

void GeneratorWartosciZadanej::fromJson(const QJsonObject& obj)
{
    m_typ = static_cast<TypSygnalu>(obj["typ"].toInt());
    m_amplituda = obj["amplituda"].toDouble();
    m_okresDyskretny = obj["okresDyskretny"].toInt();
    m_skladowaStala = obj["skladowaStala"].toDouble();
    m_wypelnienie = obj["wypelnienie"].toDouble();
    m_okresRzeczywisty = obj["okresRzeczywisty"].toDouble();
    m_interwal = obj["interwal"].toInt();

    // Stan wewnętrzny generatora NIE jest wczytywany
    m_licznikKrokow = 0;
}

