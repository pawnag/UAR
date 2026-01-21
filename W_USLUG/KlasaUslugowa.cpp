#include "KlasaUslugowa.h"
#include <QJsonArray>
#include <QJsonDocument>

KlasaUslugowa::KlasaUslugowa(QObject *parent)
    : QObject(parent),
    m_timer(new QTimer(this))
{
    connect(m_timer, &QTimer::timeout, this, &KlasaUslugowa::cyklSymulacji);
    m_timer->setInterval(200);
}

void KlasaUslugowa::start() {
    m_symulacja.uruchom();
    m_timer->start();
}

void KlasaUslugowa::stop() {
    m_symulacja.zatrzymaj();
    m_timer->stop();
}

bool KlasaUslugowa::czyDziala() const {
    return m_timer->isActive();
}

void KlasaUslugowa::cyklSymulacji() {
    m_symulacja.wykonajKrok();
    emit noweDaneDostepne();
}

void KlasaUslugowa::reset() {
    stop();
    m_symulacja.resetuj();
    emit noweDaneDostepne();
}

void KlasaUslugowa::resetPID() {
    m_symulacja.resetUAR();
}

void KlasaUslugowa::setInterwal(int ms) {
    if (ms > 0) {
        m_timer->setInterval(ms);
        auto gen = m_symulacja.pobierzGenerator();
        m_symulacja.konfigurujGenerator(gen.getAmplituda(), gen.getOkresRzeczywisty(), ms,
                                        gen.getTypSygnalu(), gen.getSkladowaStala(), gen.getWypelnienie());
    }
}

// ---------------------------------------------------------
// SETTERY
// ---------------------------------------------------------

void KlasaUslugowa::setGenerator(double a, double T, int dt, GeneratorWartosciZadanej::TypSygnalu typ, double skl, double wyp) {
    m_symulacja.konfigurujGenerator(a, T, dt, typ, skl, wyp);
    if(dt > 0) m_timer->setInterval(dt);
}

void KlasaUslugowa::setRegulator(double k, double ti, double td) {
    m_symulacja.konfigurujRegulator(k, ti, td);
}

void KlasaUslugowa::setPidMetodaCalkowania(int i) {
    m_symulacja.konfigurujMetodePID(i);
}

void KlasaUslugowa::setModelARX(const std::vector<double>& A, const std::vector<double>& B, int k, double szum, double umin, double umax, double ymin, double ymax) {
    m_symulacja.konfigurujModel(A, B, k, szum);
    // Teraz to zadziała, bo dodaliśmy tę metodę w Symulacja.h/cpp
    m_symulacja.ustawOgraniczenia(umin, umax, ymin, ymax);
}

// ---------------------------------------------------------
// GETTERY DANYCH CHWILOWYCH
// ---------------------------------------------------------

double KlasaUslugowa::getCzas() const { return m_symulacja.getCzas(); }
double KlasaUslugowa::getWartoscZadana() const { return m_symulacja.getWartoscZadana(); }
double KlasaUslugowa::getWartoscWyjscie() const { return m_symulacja.getWartoscWyjscie(); }
double KlasaUslugowa::getSterowanie() const { return m_symulacja.getSterowanie(); }
double KlasaUslugowa::getUchyb() const { return m_symulacja.getUchyb(); }

double KlasaUslugowa::getPidLastP() const { return m_symulacja.pobierzRegulator().getLastP(); }
double KlasaUslugowa::getPidLastI() const { return m_symulacja.pobierzRegulator().getLastI(); }
double KlasaUslugowa::getPidLastD() const { return m_symulacja.pobierzRegulator().getLastD(); }

// ---------------------------------------------------------
// FASADA - GETTERY KONFIGURACYJNE
// ---------------------------------------------------------

// USUNIĘTO: GeneratorWartosciZadanej KlasaUslugowa::pobierzGenerator() {...} (To powodowało błędy)
// USUNIĘTO: RegulatorPID KlasaUslugowa::pobierzRegulator() {...} (To powodowało błędy)

double KlasaUslugowa::getGenAmplituda() const { return m_symulacja.pobierzGenerator().getAmplituda(); }
double KlasaUslugowa::getGenOkres() const { return m_symulacja.pobierzGenerator().getOkresRzeczywisty(); }
int KlasaUslugowa::getGenInterwal() const { return m_symulacja.pobierzGenerator().getInterwal(); }
double KlasaUslugowa::getGenSkladowa() const { return m_symulacja.pobierzGenerator().getSkladowaStala(); }
double KlasaUslugowa::getGenWypelnienie() const { return m_symulacja.pobierzGenerator().getWypelnienie(); }
int KlasaUslugowa::getGenTyp() const { return static_cast<int>(m_symulacja.pobierzGenerator().getTypSygnalu()); }

double KlasaUslugowa::getPidKp() const { return m_symulacja.pobierzRegulator().getWzmocnienie(); }
double KlasaUslugowa::getPidTi() const { return m_symulacja.pobierzRegulator().getStalaCalk(); }
double KlasaUslugowa::getPidTd() const { return m_symulacja.pobierzRegulator().getStalaRozn(); }
int KlasaUslugowa::getPidMetodaCalkowania() const { return static_cast<int>(m_symulacja.pobierzRegulator().getLiczCalk()); }

ModelARX KlasaUslugowa::pobierzModel() const { return m_symulacja.pobierzModel(); }

// ---------------------------------------------------------
// SERIALIZACJA
// ---------------------------------------------------------

QJsonObject KlasaUslugowa::toJson() const {
    QJsonObject root;

    auto model = m_symulacja.pobierzModel();
    QJsonObject modelObj;
    {
        QJsonArray arrA, arrB;
        for (double v : model.getA()) arrA.append(v);
        for (double v : model.getB()) arrB.append(v);
        modelObj["A"] = arrA; modelObj["B"] = arrB;
        modelObj["opoznienie"] = model.getOpoznienieTransportowe();
        modelObj["szum"] = model.getOdchylenieStandardoweSzumu();
        modelObj["u min"] = model.getUMIN(); modelObj["u max"] = model.getUMAX();
        modelObj["y min"] = model.getYMIN(); modelObj["y max"] = model.getYMAX();
    }
    root["modelARX"] = modelObj;

    auto pid = m_symulacja.pobierzRegulator();
    QJsonObject pidObj;
    {
        pidObj["k"] = pid.getWzmocnienie();
        pidObj["TI"] = pid.getStalaCalk();
        pidObj["TD"] = pid.getStalaRozn();
        pidObj["trybCalk"] = static_cast<int>(pid.getLiczCalk());
    }
    root["regulatorPID"] = pidObj;

    auto gen = m_symulacja.pobierzGenerator();
    QJsonObject genObj;
    {
        genObj["typ"] = static_cast<int>(gen.getTypSygnalu());
        genObj["amplituda"] = gen.getAmplituda();
        genObj["okres"] = gen.getOkresRzeczywisty();
        genObj["interwal"] = gen.getInterwal();
        genObj["skladowa"] = gen.getSkladowaStala();
        genObj["wypelnienie"] = gen.getWypelnienie();
    }
    root["generator"] = genObj;

    return root;
}

void KlasaUslugowa::fromJson(const QJsonObject& root) {
    if (root.contains("modelARX")) {
        auto obj = root["modelARX"].toObject();
        std::vector<double> A, B;
        for (auto v : obj["A"].toArray()) A.push_back(v.toDouble());
        for (auto v : obj["B"].toArray()) B.push_back(v.toDouble());
        setModelARX(A, B, obj["opoznienie"].toInt(), obj["szum"].toDouble(),
                    obj["u min"].toDouble(), obj["u max"].toDouble(),
                    obj["y min"].toDouble(), obj["y max"].toDouble());
    }

    if (root.contains("regulatorPID")) {
        auto obj = root["regulatorPID"].toObject();
        setRegulator(obj["k"].toDouble(), obj["TI"].toDouble(), obj["TD"].toDouble());
        setPidMetodaCalkowania(obj["trybCalk"].toInt());
    }

    if (root.contains("generator")) {
        auto obj = root["generator"].toObject();
        setGenerator(obj["amplituda"].toDouble(), obj["okres"].toDouble(), obj["interwal"].toInt(),
                     static_cast<GeneratorWartosciZadanej::TypSygnalu>(obj["typ"].toInt()),
                     obj["skladowa"].toDouble(), obj["wypelnienie"].toDouble());
    }

    m_timer->setInterval(m_symulacja.pobierzGenerator().getInterwal());
    m_symulacja.resetuj();
    emit noweDaneDostepne();
}
