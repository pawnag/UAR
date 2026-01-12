#include "W_USLUG/KlasaUslugowa.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

KlasaUslugowa::KlasaUslugowa(QObject *parent)
    : QObject(parent)
{
}

void KlasaUslugowa::nowyGenerator(double amplituda,
                                  double okres,
                                  int interwalMs,
                                  GeneratorWartosciZadanej::TypSygnalu typ,
                                  double skladowa,
                                  double wypelnienie)
{
    m_symulacja.konfigurujGenerator(amplituda, okres, interwalMs, typ, skladowa, wypelnienie);
}

void KlasaUslugowa::nowyModelARX(const std::vector<double>& A, const std::vector<double>& B, int opoznienie, double szum)
{
    if (A == m_symulacja.getModelA() &&
        B == m_symulacja.getModelB() &&
        opoznienie == m_symulacja.getModelOpoznienie() &&
        szum == m_symulacja.getModelSzum())
    {
        return;
    }

    m_symulacja.konfigurujModel(A, B, opoznienie);
    m_symulacja.getModel().setOdchylenieStandardoweSzumu(szum);
}


void KlasaUslugowa::nowyRegulator(double k, double TI, double TD)
{
    m_symulacja.konfigurujRegulator(k, TI, TD);
}

double KlasaUslugowa::getCzas() const { return m_symulacja.getCzas(); }
double KlasaUslugowa::getWartoscZadana() const { return m_symulacja.getWartoscZadana(); }
double KlasaUslugowa::getWartoscWyjscie() const { return m_symulacja.getWartoscWyjscie(); }
double KlasaUslugowa::getSterowanie() const { return m_symulacja.getSterowanie(); }
double KlasaUslugowa::getUchyb() const { return m_symulacja.getUchyb(); }

std::vector<double> KlasaUslugowa::getWektorA() const
{
    return m_symulacja.getModelA();
}

std::vector<double> KlasaUslugowa::getWektorB() const
{
    return m_symulacja.getModelB();
}

int KlasaUslugowa::getOpoznienie() const
{
    return m_symulacja.getModelOpoznienie();
}

double KlasaUslugowa::getSzum() const
{
    return m_symulacja.getModelSzum();
}

void KlasaUslugowa::start() { m_symulacja.uruchom(); }
void KlasaUslugowa::stop()  { m_symulacja.zatrzymaj(); }
void KlasaUslugowa::reset() { m_symulacja.resetuj(); }

void KlasaUslugowa::wykonajKrokSymulacji()
{
    m_symulacja.wykonajKrok();
}

QJsonObject KlasaUslugowa::toJson() const
{
    QJsonObject root;

    // --- MODEL ARX ---
    QJsonObject modelObj;
    {
        auto A = m_symulacja.getModelA();
        auto B = m_symulacja.getModelB();

        QJsonArray arrA;
        for (double v : A) arrA.append(v);
        modelObj["A"] = arrA;

        QJsonArray arrB;
        for (double v : B) arrB.append(v);
        modelObj["B"] = arrB;

        modelObj["opoznienie"] = m_symulacja.getModelOpoznienie();
        modelObj["szum"] = m_symulacja.getModelSzum();
    }
    root["modelARX"] = modelObj;

    // --- REGULATOR PID ---
    QJsonObject pidObj;
    {
        const auto& pid = m_symulacja.getRegulator();
        pidObj["k"]  = pid.getWzmocnienie();
        pidObj["TI"] = pid.getStalaCalk();
        pidObj["TD"] = pid.getStalaRozn();
        pidObj["trybCalk"] = static_cast<int>(pid.getLiczCalk());
    }
    root["regulatorPID"] = pidObj;

    // --- GENERATOR ---
    QJsonObject genObj;
    {
        genObj["typ"] = static_cast<int>(m_symulacja.getGenerator().getTypSygnalu());
        genObj["amplituda"] = m_symulacja.getGenerator().getAmplituda();
        genObj["okres"] = m_symulacja.getGenerator().getOkresRzeczywisty();
        genObj["interwal"] = m_symulacja.getGenerator().getInterwal();
        genObj["skladowa"] = m_symulacja.getGenerator().getSkladowaStala();
        genObj["wypelnienie"] = m_symulacja.getGenerator().getWypelnienie();
    }
    root["generator"] = genObj;

    return root;
}

void KlasaUslugowa::fromJson(const QJsonObject& root)
{
    // --- MODEL ARX ---
    if (root.contains("modelARX"))
    {
        auto obj = root["modelARX"].toObject();

        std::vector<double> A, B;

        for (auto v : obj["A"].toArray()) A.push_back(v.toDouble());
        for (auto v : obj["B"].toArray()) B.push_back(v.toDouble());

        int op = obj["opoznienie"].toInt();
        double szum = obj["szum"].toDouble();

        nowyModelARX(A, B, op, szum);
    }

    // --- PID ---
    if (root.contains("regulatorPID"))
    {
        auto obj = root["regulatorPID"].toObject();

        double k = obj["k"].toDouble();
        double TI = obj["TI"].toDouble();
        double TD = obj["TD"].toDouble();
        int tryb = obj["trybCalk"].toInt();

        nowyRegulator(k, TI, TD);
        m_symulacja.getRegulator().setLiczCalk(
            static_cast<RegulatorPID::LiczCalk>(tryb)
        );
    }

    // --- GENERATOR ---
    if (root.contains("generator"))
    {
        auto obj = root["generator"].toObject();

        auto typ = static_cast<GeneratorWartosciZadanej::TypSygnalu>(obj["typ"].toInt());
        double ampl = obj["amplituda"].toDouble();
        double okres = obj["okres"].toDouble();
        int interwal = obj["interwal"].toInt();
        double skladowa = obj["skladowa"].toDouble();
        double wypelnienie = obj["wypelnienie"].toDouble();

        nowyGenerator(ampl, okres, interwal, typ, skladowa, wypelnienie);
    }

    //m_symulacja.resetUAR();
    m_symulacja.resetuj();
}
