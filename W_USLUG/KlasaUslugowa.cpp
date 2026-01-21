#include "KlasaUslugowa.h"
#include <QFile>
#include <QJsonDocument>

KlasaUslugowa::KlasaUslugowa(QObject *parent)
    : QObject(parent)
{
    m_symulacja.setParent(this);

    connect(&m_symulacja, &Symulacja::krokWykonany, this, [this]() {
        emit noweDane(
            m_symulacja.getCzas(),
            m_symulacja.getWartoscZadana(),
            m_symulacja.getWartoscWyjscie(),
            m_symulacja.getSterowanie(),
            m_symulacja.getUchyb()
            );
    });
}

void KlasaUslugowa::start() { m_symulacja.uruchom(); }
void KlasaUslugowa::stop() { m_symulacja.zatrzymaj(); }
bool KlasaUslugowa::czyDziala() const { return m_symulacja.czyDziala(); }
void KlasaUslugowa::reset() { m_symulacja.resetuj(); }
void KlasaUslugowa::resetPID() { m_symulacja.resetUAR(); }
void KlasaUslugowa::setInterwal(int ms) { m_symulacja.setInterwal(ms); }

void KlasaUslugowa::ustawGenerator(double a, double T, int dt, int typ, double skl, double wyp) {
    auto typEnum = static_cast<GeneratorWartosciZadanej::TypSygnalu>(typ);
    m_symulacja.konfigurujGenerator(a, T, dt, typEnum, skl, wyp);
}

void KlasaUslugowa::ustawPID(double k, double ti, double td, int metoda) {
    m_symulacja.konfigurujRegulator(k, ti, td);
    m_symulacja.konfigurujMetodePID(metoda);
}

void KlasaUslugowa::ustawModel(const std::vector<double>& A, const std::vector<double>& B,
                               int opoznienie, double szum,
                               double umin, double umax, double ymin, double ymax) {
    m_symulacja.konfigurujModel(A, B, opoznienie, szum);
    m_symulacja.ustawOgraniczenia(umin, umax, ymin, ymax);
}

// ETTERY DANYCH CHWILOWYCH
double KlasaUslugowa::getCzas() const { return m_symulacja.getCzas(); }
double KlasaUslugowa::getWartoscZadana() const { return m_symulacja.getWartoscZadana(); }
double KlasaUslugowa::getWartoscWyjscie() const { return m_symulacja.getWartoscWyjscie(); }
double KlasaUslugowa::getSterowanie() const { return m_symulacja.getSterowanie(); }
double KlasaUslugowa::getUchyb() const { return m_symulacja.getUchyb(); }

double KlasaUslugowa::getPidLastP() const { return m_symulacja.pobierzRegulator().getLastP(); }
double KlasaUslugowa::getPidLastI() const { return m_symulacja.pobierzRegulator().getLastI(); }
double KlasaUslugowa::getPidLastD() const { return m_symulacja.pobierzRegulator().getLastD(); }

// GETTERY KONFIGURACYJNE
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

// POBIERANIE MODELU
void KlasaUslugowa::pobierzModel(std::vector<double>& A, std::vector<double>& B,
                                 int& opoznienie, double& szum,
                                 double& uMin, double& uMax, double& yMin, double& yMax) const
{
    ModelARX m = m_symulacja.pobierzModel();
    A = m.getA();
    B = m.getB();
    opoznienie = m.getOpoznienieTransportowe();
    szum = m.getOdchylenieStandardoweSzumu();
    uMin = m.getUMIN();
    uMax = m.getUMAX();
    yMin = m.getYMIN();
    yMax = m.getYMAX();
}

// JSON
QJsonObject KlasaUslugowa::toJson() const {
    return m_symulacja.toJson();
}

void KlasaUslugowa::fromJson(const QJsonObject& root) {
    m_symulacja.fromJson(root);
    reset();
}

