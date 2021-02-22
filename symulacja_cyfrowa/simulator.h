#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include "wifi.h"
#include "event.h"
#include "generateevent.h"
#include "difswaitingevent.h"
#include "checkingchannelevent.h"
#include "ackevent.h"
#include "checkingcollisionevent.h"


//Klasa zawierajaca czas systemowy oraz liste zdarzen.
class Simulator
{
public:
	explicit Simulator(WiFi* wifi);
	void M2(int time); //Metoda symulacji M2
	void StepByStepMode();//Tryb wyswietlania
	void GetStats();//Statystyki symulacji


private:

	size_t clock_ = 0;
	WiFi* wifi_ = nullptr;

};

#endif

