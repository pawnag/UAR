#pragma once
#include <array>
class ModelARX 
{
private:
	std::array<double, 2> m_wektorA;
	std::array<double, 2> m_wektorB;
	int m_ot; // OpóŸnienie transportowe
	// Pamiêæ stanów:
	std::array<double, 10> m_poprzednieWe = { 0.0 };
	std::array<double, 10> m_poprzednieWy = { 0.0 };
public:
	ModelARX(const std::array<double, 2>& wektorA, const std::array<double, 2>& wektorB, int ot, int zaklocenia)
	{
		setWektorA(wektorA);
		setWektorB(wektorB);
		setOT(ot);
	}
	double symuluj(double sygnalWe1);

	void setWektorA(const std::array<double, 2>& wektorA);
	void setWektorB(const std::array<double, 2>& wektorB);
	void setOT(int OT);

	std::array<double, 2> getWektorA();
	std::array<double, 2> getWektorB();
	int getOT();
};