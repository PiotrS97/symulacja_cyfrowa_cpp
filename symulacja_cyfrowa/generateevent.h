#ifndef GENERATEEVENT_H_
#define GENERATEEVENT_H_

#include "event.h"
#include "wifi.h"
#include <functional>
#include "basestation.h"
#include "logger.h"
#include "generator.h"

//Klasa okreslajaca zdarzenie wygenerowania pakietu
class GenerateEvent : public Event
{
public:
	GenerateEvent(size_t time, WiFi* wifi, EventList* event_list, int id);
	void Execute() override;
	int GetId() override { return transmitter_id_; };
private:
	EventList* event_list_ = nullptr;
	const size_t waiting_for_channel = 5;//czas oczekiwania na kanal (0.5ms * 10)
	int transmitter_id_;//id nadajnika
};
#endif 
