#include "ProstyUAR.h"

// Konstruktor domyślny
ProstyUAR::ProstyUAR(QObject *parent)
    : QObject(parent),
    m_arx(),       // Domyślny konstruktor ARX
    m_regulator(), // Domyślny konstruktor PID
    m_poprzedniaWartoscRegulowana(0.0),
    m_ostatniUchyb(0.0),
    m_ostatnieSterowanie(0.0)
{
}

// Konstruktor dla testów - "Ręczne Kopiowanie Ustawień"
ProstyUAR::ProstyUAR(const ModelARX& wzorzecARX, const RegulatorPID& wzorzecPID, QObject *parent)
    : QObject(parent),
    m_arx(),
    m_regulator(),
    m_poprzedniaWartoscRegulowana(0.0),
    m_ostatniUchyb(0.0),
    m_ostatnieSterowanie(0.0)
{
    // TERAZ RĘCZNIE PRZEPISUJEMY WARTOŚCI Z WZORCA DO NASZEGO OBIEKTU WEWNĘTRZNEGO

    // 1. Przepisanie nastaw Modelu ARX
    m_arx.setA(wzorzecARX.getA());
    m_arx.setB(wzorzecARX.getB());
    m_arx.setOpoznienieTransportowe(wzorzecARX.getOpoznienieTransportowe());
    m_arx.setOdchylenieStandardoweSzumu(wzorzecARX.getOdchylenieStandardoweSzumu());
    // Jeśli są limity, też je przepisz:
    // m_arx.setLimitGora(wzorzecARX.getLimitGora()); ...
    m_arx.resetuj(); // Na wszelki wypadek

    // 2. Przepisanie nastaw Regulatora PID
    m_regulator.setWzmocnienie(wzorzecPID.getWzmocnienie());
    m_regulator.setStalaCalk(wzorzecPID.getStalaCalk());
    m_regulator.setStalaRozn(wzorzecPID.getStalaRozn());
    // Przepisz inne ustawienia (np. tryb całki, limity) jeśli istnieją gettery:
    // m_regulator.setTrybCalki(wzorzecPID.getTrybCalki());
    m_regulator.resetuj();
}

double ProstyUAR::symuluj(double wartoscZadana)
{
    m_ostatniUchyb = wartoscZadana - m_poprzedniaWartoscRegulowana;
    m_ostatnieSterowanie = m_regulator.symuluj(m_ostatniUchyb);
    double noweWyjscie = m_arx.symuluj(m_ostatnieSterowanie);
    m_poprzedniaWartoscRegulowana = noweWyjscie;
    return noweWyjscie;
}

void ProstyUAR::reset()
{
    m_poprzedniaWartoscRegulowana = 0.0;
    m_ostatniUchyb = 0.0;
    m_ostatnieSterowanie = 0.0;
    m_arx.resetuj();
    m_regulator.resetuj();
}
