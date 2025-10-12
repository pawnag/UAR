#include "ModelARX.h"
#include <iostream>
double ModelARX::symuluj(double sygnalWe) 
{
    double y = m_wektorB[0] * m_poprzednieWe[0] + m_wektorB[1] * m_poprzednieWe[1]
        - m_wektorA[0] * m_poprzednieWy[0] - m_wektorA[1] * m_poprzednieWy[1];

    // Przesuniêcie pamiêci
    m_poprzednieWe[1] = m_poprzednieWe[0];
    m_poprzednieWe[0] = sygnalWe;

    m_poprzednieWy[1] = m_poprzednieWy[0];
    m_poprzednieWy[0] = y;

    // TODO: Bufor kolejka z opóŸnieniem transportowym

    return y;
}

void ModelARX::setWektorA(const std::array<double, 2>& wektorA)
{
    m_wektorA = wektorA;
}

void ModelARX::setWektorB(const std::array<double, 2>& wektorB)
{
    m_wektorB = wektorB;
}

void ModelARX::setOT(int ot) 
{
    m_ot = ot;
}

std::array<double, 2> ModelARX::getWektorA() { return m_wektorA; }
std::array<double, 2> ModelARX::getWektorB() { return m_wektorB; }
int ModelARX::getOT() { return m_ot; }