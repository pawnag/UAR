#include "KlasaUslugowa.h"
// #include <QJsonArray> -> Już niepotrzebne
// #include <QJsonDocument>

KlasaUslugowa::KlasaUslugowa(QObject *parent)
    : QObject(parent)
{
    m_symulacja.setParent(this);
    connect(&m_symulacja, &Symulacja::krokWykonany, this, &KlasaUslugowa::noweDaneDostepne);
}

// ---------------------------------------------------------
// JSON (TERAZ TYLKO DELEGACJA)
// ---------------------------------------------------------

QJsonObject KlasaUslugowa::toJson() const
{
    // Warstwa usług tylko prosi warstwę danych o zrzut
    return m_symulacja.toJson();
}

void KlasaUslugowa::fromJson(const QJsonObject& root)
{
    // Przekazujemy JSON do symulacji
    m_symulacja.fromJson(root);

    // Po wczytaniu danych odświeżamy GUI, żeby pokazało nowe nastawy
    emit noweDaneDostepne();
}

// --- POZOSTAŁE METODY (BEZ ZMIAN - DELEGACJA) ---

void KlasaUslugowa::start() { m_symulacja.uruchom(); }
void KlasaUslugowa::stop() { m_symulacja.zatrzymaj(); }
bool KlasaUslugowa::czyDziala() const { return m_symulacja.czyDziala(); }

void KlasaUslugowa::reset() {
    m_symulacja.resetuj();
    emit noweDaneDostepne();
}
void KlasaUslugowa::resetPID() { m_symulacja.resetUAR(); }
void KlasaUslugowa::setInterwal(int ms) { m_symulacja.setInterwal(ms); }

void KlasaUslugowa::setGenerator(double a, double T, int dt, GeneratorWartosciZadanej::TypSygnalu typ, double skl, double wyp) {
    m_symulacja.konfigurujGenerator(a, T, dt, typ, skl, wyp);
}
void KlasaUslugowa::setRegulator(double k, double ti, double td) {
    m_symulacja.konfigurujRegulator(k, ti, td);
}
void KlasaUslugowa::setPidMetodaCalkowania(int i) {
    m_symulacja.konfigurujMetodePID(i);
}
void KlasaUslugowa::setModelARX(const std::vector<double>& A, const std::vector<double>& B, int k, double szum, double umin, double umax, double ymin, double ymax) {
    m_symulacja.konfigurujModel(A, B, k, szum);
    m_symulacja.ustawOgraniczenia(umin, umax, ymin, ymax);
}

// Gettery
double KlasaUslugowa::getCzas() const { return m_symulacja.getCzas(); }
double KlasaUslugowa::getWartoscZadana() const { return m_symulacja.getWartoscZadana(); }
double KlasaUslugowa::getWartoscWyjscie() const { return m_symulacja.getWartoscWyjscie(); }
double KlasaUslugowa::getSterowanie() const { return m_symulacja.getSterowanie(); }
double KlasaUslugowa::getUchyb() const { return m_symulacja.getUchyb(); }

double KlasaUslugowa::getPidLastP() const { return m_symulacja.pobierzRegulator().getLastP(); }
double KlasaUslugowa::getPidLastI() const { return m_symulacja.pobierzRegulator().getLastI(); }
double KlasaUslugowa::getPidLastD() const { return m_symulacja.pobierzRegulator().getLastD(); }

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
