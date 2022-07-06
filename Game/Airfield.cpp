#include "stdafx.h"
#include "Airfield.h"
#include <ctime>

TAirfield::TAirfield(TAircraftList& list, HDC dc) : m_aircraft_list(list), m_dc(dc)
{
	srand(time(0));
	m_interval = 500 + rand()%500; 
}

// Новий літак на випадковій висоті з випадковою швидкістю
TAircraft* TAirfield::SendAircraft()
{
	return new TAircraft(5+rand()%200,1+rand()%8,m_dc);
}

// Якщо час очікування вичерпався, до списку літаків додається новостворений літак
void TAirfield::Spent(int t)
{
	m_interval -= t;
	if (m_interval <= 0)
	{
		m_aircraft_list.Put(this->SendAircraft());
		m_interval = 100 + rand()%1900;
	}
}