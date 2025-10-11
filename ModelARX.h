#pragma once
#include <array>
class ModelARX 
{
private:
	std::array<double, 2> m_wektorA;
	std::array<double, 2> m_wektorB;
	int m_sygnalWejsciowy;
	int zaklocenia;
	// Pamiêæ stanów
	double m_poprzednieWe[2] = { 0.0, 0.0 };
	double m_poprzednieWy[2] = { 0.0, 0.0 };
public:
	ModelARX(const std::array<double, 2>& wektorA, const std::array<double, 2>& wektorB, int sygnalWejsciowy, int zaklocenia)
	{
		setWektorA(wektorA);
		setWektorB(wektorB);
		setSygnalWejsciowy(sygnalWejsciowy);
	}
	double symuluj(double sygnalWe1);

	void setWektorA(const std::array<double, 2>& wektorA);
	void setWektorB(const std::array<double, 2>& wektorB);
	void setSygnalWejsciowy(int sygnalWejsciowy);

	std::array<double, 2> getWektorA();
	std::array<double, 2> getWektorB();
	int getSygnalWejsciowy();
};