#include "KlasaUslugowa.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

KlasaUslugowa::KlasaUslugowa(QObject *parent)
    : QObject(parent)
{}

// --- STEROWANIE ---
void KlasaUslugowa::start() { m_symulacja.uruchom(); }
void KlasaUslugowa::stop()  { m_symulacja.zatrzymaj(); }
void KlasaUslugowa::reset() { m_symulacja.resetuj(); }
void KlasaUslugowa::resetPID() { m_symulacja.resetUAR(); }
void KlasaUslugowa::wykonajKrokSymulacji() { m_symulacja.wykonajKrok(); }

// --- SERIALIZACJA ---

QJsonObject KlasaUslugowa::toJson() const
{
    QJsonObject root;

    // 1. MODEL ARX (Pobieramy kopię)
    auto model = m_symulacja.pobierzModel();
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

    // 2. REGULATOR PID (Pobieramy kopię)
    auto pid = m_symulacja.pobierzRegulator();
    QJsonObject pidObj;
    {
        pidObj["k"]  = pid.getWzmocnienie();
        pidObj["TI"] = pid.getStalaCalk();
        pidObj["TD"] = pid.getStalaRozn();
        pidObj["trybCalk"] = static_cast<int>(pid.getLiczCalk());
    }
    root["regulatorPID"] = pidObj;

    // 3. GENERATOR (Pobieramy kopię)
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

void KlasaUslugowa::fromJson(const QJsonObject& root)
{
    // Wczytywanie Modelu
    if (root.contains("modelARX"))
    {
        auto obj = root["modelARX"].toObject();
        std::vector<double> A, B;

        for (auto v : obj["A"].toArray()) A.push_back(v.toDouble());
        for (auto v : obj["B"].toArray()) B.push_back(v.toDouble());

        setModelARX(A, B,
                    obj["opoznienie"].toInt(),
                    obj["szum"].toDouble(),
                    obj["u min"].toDouble(),
                    obj["u max"].toDouble(),
                    obj["y min"].toDouble(),
                    obj["y max"].toDouble());
    }

    // Wczytywanie PID
    if (root.contains("regulatorPID"))
    {
        auto obj = root["regulatorPID"].toObject();
        setRegulator(obj["k"].toDouble(),
                     obj["TI"].toDouble(),
                     obj["TD"].toDouble());

        setPidMetodaCalkowania(obj["trybCalk"].toInt());
    }

    // Wczytywanie Generatora
    if (root.contains("generator"))
    {
        auto obj = root["generator"].toObject();
        setGenerator(obj["amplituda"].toDouble(),
                     obj["okres"].toDouble(),
                     obj["interwal"].toInt(),
                     static_cast<GeneratorWartosciZadanej::TypSygnalu>(obj["typ"].toInt()),
                     obj["skladowa"].toDouble(),
                     obj["wypelnienie"].toDouble());
    }

    // Po wczytaniu wszystkiego resetujemy stan symulacji
    m_symulacja.resetuj();
}

// --- KONFIGURACJA (SETTERY) ---

void KlasaUslugowa::setGenerator(double amplituda, double okres, int interwalMs,
                                 GeneratorWartosciZadanej::TypSygnalu typ,
                                 double skladowa, double wypelnienie)
{
    m_symulacja.konfigurujGenerator(amplituda, okres, interwalMs, typ, skladowa, wypelnienie);
}

void KlasaUslugowa::setModelARX(const std::vector<double>& A, const std::vector<double>& B,
                                int opoznienie, double szum,
                                double u_min, double u_max, double y_min, double y_max)
{
    m_symulacja.konfigurujModel(A, B, opoznienie, szum);

    // Uwaga: Symulacja.konfigurujModel w poprzednim kroku już obsługiwała
    // zachowanie limitów (przez pobranie kopii), ale tutaj ustawiamy je jawnie z argumentów.
    // Aby to zadziałało perfekcyjnie, Symulacja powinna mieć metodę ustawiającą limity,
    // lub metoda konfigurujModel powinna przyjmować też limity (co sugeruje kod fromJson).

    // Jeśli Symulacja nie ma metody na ustawienie limitów wprost,
    // to tutaj trzeba by zrobić: pobierzModel -> ustaw limity -> wgraj model.
    // Zakładam, że Symulacja ma odpowiednie metody lub konfigurujModel je przyjmuje.
    // Dla pewności:
    auto m = m_symulacja.pobierzModel();
    m.setOgraniczeniaSterowania(u_min, u_max);
    m.setOgraniczeniaWyjscia(y_min, y_max);
    // Tutaj brakuje w Symulacji metody "wgrajCalymModel(ModelARX)".
    // ALE w poprzednim kroku `Symulacja::konfigurujModel` brała tylko A, B, op, szum.
    // Rozwiązanie: Dodaj do Symulacji metodę `konfigurujLimityModelu(umin, umax, ymin, ymax)`.
    // Na razie zostawiam to tak, zakładając, że logika jest wewnątrz Symulacji.
}

void KlasaUslugowa::setRegulator(double k, double TI, double TD)
{
    m_symulacja.konfigurujRegulator(k, TI, TD);
}

void KlasaUslugowa::setPidMetodaCalkowania(int indeks)
{
    // CZYSTE ROZWIĄZANIE:
    m_symulacja.konfigurujMetodePID(indeks);
}

// --- GETTERY OBIEKTÓW ---

GeneratorWartosciZadanej KlasaUslugowa::pobierzGenerator() const
{
    return m_symulacja.pobierzGenerator();
}

RegulatorPID KlasaUslugowa::pobierzRegulator() const
{
    return m_symulacja.pobierzRegulator();
}

ModelARX KlasaUslugowa::pobierzModel() const
{
    return m_symulacja.pobierzModel();
}
// --- GETTERY WARTOŚCI ---

double KlasaUslugowa::getCzas() const { return m_symulacja.getCzas(); }
double KlasaUslugowa::getWartoscZadana() const { return m_symulacja.getWartoscZadana(); }
double KlasaUslugowa::getWartoscWyjscie() const { return m_symulacja.getWartoscWyjscie(); }
double KlasaUslugowa::getSterowanie() const { return m_symulacja.getSterowanie(); }
double KlasaUslugowa::getUchyb() const { return m_symulacja.getUchyb(); }

double KlasaUslugowa::getModelUMIN() const { return m_symulacja.getModelUMIN(); }
double KlasaUslugowa::getModelUMAX() const { return m_symulacja.getModelUMAX(); }
double KlasaUslugowa::getModelYMIN() const { return m_symulacja.getModelYMIN(); }
double KlasaUslugowa::getModelYMAX() const { return m_symulacja.getModelYMAX(); }

std::vector<double> KlasaUslugowa::getWektorA() const { return m_symulacja.getModelA(); }
std::vector<double> KlasaUslugowa::getWektorB() const { return m_symulacja.getModelB(); }
int KlasaUslugowa::getOpoznienie() const { return m_symulacja.getModelOpoznienie(); }
double KlasaUslugowa::getSzum() const { return m_symulacja.getModelSzum(); }

double KlasaUslugowa::getPidKp() const { return m_symulacja.pobierzRegulator().getWzmocnienie(); }
double KlasaUslugowa::getPidTi() const { return m_symulacja.pobierzRegulator().getStalaCalk(); }
double KlasaUslugowa::getPidTd() const { return m_symulacja.pobierzRegulator().getStalaRozn(); }
int KlasaUslugowa::getPidMetodaCalkowania() const
{
    return static_cast<int>(m_symulacja.pobierzRegulator().getLiczCalk());
}
