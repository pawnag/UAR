#include "W_USLUG/ProstyUAR.h"

ProstyUAR::ProstyUAR(QObject *parent)
    : QObject(parent),
    m_model(parent),
    m_regulator(1.0, 0.0, 0.0, this), // Domyślne
    m_trybOtwarty(true),
    m_ostatniUchyb(0.0),
    m_ostatnieSterowanie(0.0),
    m_ostatnieWyjscie(0.0)
{
}

// IMPLEMENTACJA KONSTRUKTORA POD TEST
ProstyUAR::ProstyUAR(const ModelARX& model, const RegulatorPID& reg, QObject *parent)
    : QObject(parent),
    m_model(this),                   // Tworzymy pusty model (z tym obiektem jako rodzicem)
    m_regulator(0.0, 0.0, 0.0, this), // Tworzymy pusty regulator
    m_trybOtwarty(true), // Domyślnie otwarty, ale test może wymagać zamkniętego
    m_ostatniUchyb(0.0),
    m_ostatnieSterowanie(0.0),
    m_ostatnieWyjscie(0.0)
{
    // RĘCZNE KOPIOWANIE PARAMETRÓW (bo QObject nie można kopiować automatycznie)

    // 1. Przepisujemy Model ARX
    m_model.setA(model.getA());
    m_model.setB(model.getB());
    m_model.setOpoznienieTransportowe(model.getOpoznienieTransportowe());
    m_model.setOdchylenieStandardoweSzumu(model.getOdchylenieStandardoweSzumu());

    // 2. Przepisujemy Regulator PID
    m_regulator.setWzmocnienie(reg.getWzmocnienie());
    m_regulator.setStalaCalk(reg.getStalaCalk());
    m_regulator.setStalaRozn(reg.getStalaRozn());
    m_regulator.setLiczCalk(reg.getLiczCalk());

    // WAŻNE: Test "brakPobudzenia" zadziała tylko jeśli układ jest w pętli zamkniętej
    // lub jeśli wejście jest 0. W teście wejście (sygWe) to same zera.
    // Ale w UAR zazwyczaj testujemy układ zamknięty:
    setTrybOtwarty(false);
}

void ProstyUAR::reset()
{
    m_model.resetuj();
    m_regulator.resetuj(); // Upewnij się, że to NIE kasuje Kp, Ti, Td!
    m_ostatniUchyb = 0.0;
    m_ostatnieSterowanie = 0.0;
    m_ostatnieWyjscie = 0.0;
    m_trybOtwarty = true;
}

void ProstyUAR::setTrybOtwarty(bool tak)
{
    m_trybOtwarty = tak;
    if (m_trybOtwarty) m_regulator.resetuj();
}


double ProstyUAR::symuluj(double wartoscZadana)
{
    // 1. Obliczamy uchyb: e(k) = w(k) - y(k-1)
    double uchyb = wartoscZadana - m_ostatnieWyjscie;
    double sterowanie = 0.0;

    if (m_trybOtwarty) {
        // --- TRYB OTWARTY (RĘCZNY) ---
        // Wartość zadana z generatora traktowana jest bezpośrednio jako sterowanie
        sterowanie = wartoscZadana;

        // Ważne: Gdy sterujemy ręcznie, regulator musi "odpoczywać".
        // Resetujemy jego całkę, aby po przełączeniu na automat nie "zwariował".
        m_regulator.resetuj();

    } else {
        // --- TRYB ZAMKNIĘTY (AUTOMAT) ---
        // Regulator wylicza sterowanie na podstawie uchybu
        sterowanie = m_regulator.symuluj(uchyb);
    }

    // 2. Symulacja obiektu (Modelu ARX)
    double noweWyjscie = m_model.symuluj(sterowanie);

    // 3. Zapamiętanie historii
    m_ostatniUchyb = uchyb;
    m_ostatnieSterowanie = sterowanie;
    m_ostatnieWyjscie = noweWyjscie;

    return noweWyjscie;
}
void ProstyUAR::aktualizujModel(const std::vector<double>& A,
                                const std::vector<double>& B,
                                int opoznienie,
                                double szum)
{
    m_model.aktualizuj(A, B, opoznienie, szum);
}
