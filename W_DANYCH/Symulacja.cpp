#include "Symulacja.h"
#include <QDebug>

Symulacja::Symulacja(QObject *parent)
    : QObject(parent),
    m_timer(new QTimer(this)),
    m_czyDziala(false),
    m_czas(0.0),
    m_wartoscZadana(0.0),
    m_wartoscWyjscie(0.0),
    m_sterowanie(0.0),
    m_uchyb(0.0)
{
    m_timer->setInterval(200);
    connect(m_timer, &QTimer::timeout, this, &Symulacja::onTimerTimeout);
}

QJsonObject Symulacja::toJson() const
{
    QJsonObject root;

    ModelARX model = m_prostyUAR.pobierzModel();
    QJsonObject modelObj;
    {
        QJsonArray arrA, arrB;
        for (double v : model.getA()) arrA.append(v);
        for (double v : model.getB()) arrB.append(v);

        modelObj["A"] = arrA;
        modelObj["B"] = arrB;
        modelObj["opoznienie"] = model.getOpoznienieTransportowe();
        modelObj["szum"] = model.getOdchylenieStandardoweSzumu();
        modelObj["u min"] = model.getUMIN();
        modelObj["u max"] = model.getUMAX();
        modelObj["y min"] = model.getYMIN();
        modelObj["y max"] = model.getYMAX();
    }
    root["modelARX"] = modelObj;

    RegulatorPID pid = m_prostyUAR.pobierzRegulator();
    QJsonObject pidObj;
    {
        pidObj["k"] = pid.getWzmocnienie();
        pidObj["TI"] = pid.getStalaCalk();
        pidObj["TD"] = pid.getStalaRozn();
        pidObj["trybCalk"] = static_cast<int>(pid.getLiczCalk());
    }
    root["regulatorPID"] = pidObj;

    QJsonObject genObj;
    {
        genObj["typ"] = static_cast<int>(m_generator.getTypSygnalu());
        genObj["amplituda"] = m_generator.getAmplituda();
        genObj["okres"] = m_generator.getOkresRzeczywisty();
        genObj["interwal"] = m_generator.getInterwal();
        genObj["skladowa"] = m_generator.getSkladowaStala();
        genObj["wypelnienie"] = m_generator.getWypelnienie();
    }
    root["generator"] = genObj;

    return root;
}

void Symulacja::fromJson(const QJsonObject& root)
{
    bool dzialalo = m_czyDziala;
    zatrzymaj();

    if (root.contains("modelARX")) {
        auto obj = root["modelARX"].toObject();
        std::vector<double> A, B;
        for (auto v : obj["A"].toArray()) A.push_back(v.toDouble());
        for (auto v : obj["B"].toArray()) B.push_back(v.toDouble());

        konfigurujModel(A, B, obj["opoznienie"].toInt(), obj["szum"].toDouble());
        ustawOgraniczenia(obj["u min"].toDouble(), obj["u max"].toDouble(),
                          obj["y min"].toDouble(), obj["y max"].toDouble());
    }

    if (root.contains("regulatorPID")) {
        auto obj = root["regulatorPID"].toObject();
        konfigurujRegulator(obj["k"].toDouble(), obj["TI"].toDouble(), obj["TD"].toDouble());
        konfigurujMetodePID(obj["trybCalk"].toInt());
    }

    if (root.contains("generator")) {
        auto obj = root["generator"].toObject();
        konfigurujGenerator(
            obj["amplituda"].toDouble(),
            obj["okres"].toDouble(),
            obj["interwal"].toInt(),
            static_cast<GeneratorWartosciZadanej::TypSygnalu>(obj["typ"].toInt()),
            obj["skladowa"].toDouble(),
            obj["wypelnienie"].toDouble()
            );
    }
}

void Symulacja::onTimerTimeout() {
    wykonajKrok();
    emit krokWykonany();
}

void Symulacja::uruchom() { m_czyDziala = true; m_timer->start(); }
void Symulacja::zatrzymaj() { m_czyDziala = false; m_timer->stop(); }

void Symulacja::setInterwal(int ms) {
    if (ms > 0) {
        m_timer->setInterval(ms);
        m_generator.setInterwal(ms);
    }
}

void Symulacja::konfigurujGenerator(double ampl, double okres, int interwal,
                                    GeneratorWartosciZadanej::TypSygnalu typ,
                                    double skladowa, double wypelnienie) {
    m_generator.setAmplituda(ampl);
    m_generator.setOkresRzeczywisty(okres);
    m_generator.setInterwal(interwal);
    m_generator.setTypSygnalu(typ);
    m_generator.setSkladowaStala(skladowa);
    m_generator.setWypelnienie(wypelnienie);
    m_generator.przeliczOkresDyskretny();
    if (interwal > 0) m_timer->setInterval(interwal);
}

void Symulacja::resetuj() {
    zatrzymaj();
    m_czas = 0.0;
    m_wartoscZadana = 0.0;
    m_wartoscWyjscie = 0.0;
    m_sterowanie = 0.0;
    m_uchyb = 0.0;
    m_generator.reset();
    m_prostyUAR.reset();
}

void Symulacja::wykonajKrok() {
    m_wartoscZadana = m_generator.generuj();
    m_generator.krokSymulacji();
    m_wartoscWyjscie = m_prostyUAR.symuluj(m_wartoscZadana);
    m_uchyb = m_prostyUAR.getOstatniUchyb();
    m_sterowanie = m_prostyUAR.getOstatnieSterowanie();
    m_czas += m_generator.getInterwal() / 1000.0;
}

void Symulacja::konfigurujModel(const std::vector<double>& A, const std::vector<double>& B, int opoznienie, double szum) {
    ModelARX temp = m_prostyUAR.pobierzModel();
    m_prostyUAR.konfigurujModel(A, B, opoznienie, szum, temp.getUMIN(), temp.getUMAX(), temp.getYMIN(), temp.getYMAX());
}
void Symulacja::ustawOgraniczenia(double u_min, double u_max, double y_min, double y_max) {
    ModelARX temp = m_prostyUAR.pobierzModel();
    m_prostyUAR.konfigurujModel(temp.getA(), temp.getB(), temp.getOpoznienieTransportowe(), temp.getOdchylenieStandardoweSzumu(), u_min, u_max, y_min, y_max);
}
void Symulacja::konfigurujRegulator(double k, double ti, double td) {
    RegulatorPID temp = m_prostyUAR.pobierzRegulator();
    m_prostyUAR.konfigurujRegulator(k, ti, td, (int)temp.getLiczCalk());
}
void Symulacja::konfigurujMetodePID(int indeksMetody) {
    RegulatorPID pid = m_prostyUAR.pobierzRegulator();
    m_prostyUAR.konfigurujRegulator(pid.getWzmocnienie(), pid.getStalaCalk(), pid.getStalaRozn(), indeksMetody);
}
void Symulacja::resetUAR() { m_prostyUAR.reset(); }

std::vector<double> Symulacja::getModelA() const { return m_prostyUAR.pobierzModel().getA(); }
std::vector<double> Symulacja::getModelB() const { return m_prostyUAR.pobierzModel().getB(); }
int Symulacja::getModelOpoznienie() const { return m_prostyUAR.pobierzModel().getOpoznienieTransportowe(); }
double Symulacja::getModelSzum() const { return m_prostyUAR.pobierzModel().getOdchylenieStandardoweSzumu(); }
double Symulacja::getModelUMIN() const { return m_prostyUAR.pobierzModel().getUMIN(); }
double Symulacja::getModelUMAX() const { return m_prostyUAR.pobierzModel().getUMAX(); }
double Symulacja::getModelYMIN() const { return m_prostyUAR.pobierzModel().getYMIN(); }
double Symulacja::getModelYMAX() const { return m_prostyUAR.pobierzModel().getYMAX(); }
double Symulacja::getWartoscZadana() const { return m_wartoscZadana; }
double Symulacja::getWartoscWyjscie() const { return m_wartoscWyjscie; }
double Symulacja::getSterowanie() const { return m_sterowanie; }
double Symulacja::getUchyb() const { return m_uchyb; }
double Symulacja::getCzas() const { return m_czas; }
bool Symulacja::czyDziala() const { return m_czyDziala; }
int Symulacja::getInterwalMs() const { return m_generator.getInterwal(); }
GeneratorWartosciZadanej Symulacja::pobierzGenerator() const { return m_generator; }
ModelARX Symulacja::pobierzModel() const { return m_prostyUAR.pobierzModel(); }
RegulatorPID Symulacja::pobierzRegulator() const { return m_prostyUAR.pobierzRegulator(); }
