#include "ModelARX.h"
#include <stdexcept>
#include <algorithm> // std::clamp, std::max

ModelARX::ModelARX(const std::vector<double>& i_A, const std::vector<double>& i_B, int i_op, double i_oss)
    : m_A(i_A), m_B(i_B),
    u_min(-10.0), u_max(10.0), y_min(-10.0), y_max(10.0), m_ograniczenia(true), rozklad_szumu(0.0, 1.0)
{
    if (m_A.empty()) m_A = {-0.4, 0.0, 0.0};
    if (m_B.empty()) m_B = {0.6, 0.0, 0.0};
    m_ot = std::max(1, i_op);
    generator_losowy.seed(std::random_device{}());
    ustawRozkladSzumu(std::max(0.0, i_oss));
    inicjalizujBufory();
}

void ModelARX::ustawRozkladSzumu(double odchylenie)
{
    m_oss = std::max(0.0, odchylenie);
    if (m_oss > 0.0)
        rozklad_szumu = std::normal_distribution<double>(0.0, m_oss);
    else
        rozklad_szumu.reset();
}

void ModelARX::inicjalizujBufory()
{
    m_u.assign(std::max<size_t>(1, m_ot + m_B.size()), 0.0);
    m_y.assign(std::max<size_t>(1, m_A.size()), 0.0);
}

double ModelARX::obliczWyjscie()
{
    double wyjscie = 0.0;

    for (size_t i = 0; i < m_B.size(); ++i)
    {
        int idx = (int)m_u.size() - 1 - m_ot - (int)i;
        if (idx >= 0) wyjscie += m_B[i] * m_u[idx];
    }

    for (size_t i = 0; i < m_A.size(); ++i)
    {
        int idx = (int)m_y.size() - 1 - (int)i;
        if (idx >= 0) wyjscie -= m_A[i] * m_y[idx];
    }

    if (m_oss > 0.000001)
    {
        wyjscie += rozklad_szumu(generator_losowy);
    }

    return wyjscie;
}
double ModelARX::symuluj(double i_u)
{
    double u_in = m_ograniczenia ? std::clamp(i_u, u_min, u_max) : i_u;

    m_u.push_back(u_in);
    if (m_u.size() > (size_t)(m_ot + m_B.size() + 10)) m_u.pop_front();

    double y_out = obliczWyjscie();

    if (m_ograniczenia) y_out = std::clamp(y_out, y_min, y_max);

    m_y.push_back(y_out);
    if (m_y.size() > (size_t)(m_A.size() + 10)) m_y.pop_front();

    return y_out;
}
void ModelARX::resetuj() { inicjalizujBufory(); }
void ModelARX::aktualizuj(const std::vector<double>& A, const std::vector<double>& B, int opoznienie, double szum)
{
    setA(A); setB(B); setOpoznienieTransportowe(opoznienie); setOdchylenieStandardoweSzumu(szum);
}

void dopasujBufor(std::deque<double>& buf, size_t minSize)
{
    if (buf.size() < minSize) buf.insert(buf.begin(), minSize - buf.size(), 0.0);
}

void ModelARX::setA(const std::vector<double>& i_A) { m_A = i_A; dopasujBufor(m_y, m_A.size()); }
void ModelARX::setB(const std::vector<double>& i_B) { m_B = i_B; dopasujBufor(m_u, m_ot + m_B.size()); }
void ModelARX::setOpoznienieTransportowe(int i_ot) { if (i_ot < 1) return; m_ot = i_ot; dopasujBufor(m_u, m_ot + m_B.size()); }
void ModelARX::setOdchylenieStandardoweSzumu(double s) { ustawRozkladSzumu(s); }
void ModelARX::setOgraniczeniaSterowania(double min, double max) { u_min = min; u_max = max; }
void ModelARX::setOgraniczeniaWyjscia(double min, double max) { y_min = min; y_max = max; }
void ModelARX::setOgraniczenia(bool ogr) { m_ograniczenia = ogr; }

std::vector<double> ModelARX::getA() const { return m_A; }
std::vector<double> ModelARX::getB() const { return m_B; }
int ModelARX::getOpoznienieTransportowe() const { return m_ot; }
double ModelARX::getOdchylenieStandardoweSzumu() const { return m_oss; }
double ModelARX::getUMIN() const { return u_min; }
double ModelARX::getUMAX() const { return u_max; }
double ModelARX::getYMIN() const { return y_min; }
double ModelARX::getYMAX() const { return y_max; }
