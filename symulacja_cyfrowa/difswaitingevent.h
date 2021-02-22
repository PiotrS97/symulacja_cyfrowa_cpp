#ifndef DIFS_WAITING_EVENT
#define DIFS_WAITING_EVENT

#include "event.h"
#include "channel.h"
#include "wifi.h"
#include "basestation.h"

class DIFSWaitingEvent : public Event
{
public:
	DIFSWaitingEvent(size_t time, WiFi* wifi, EventList* event_list, int id);
	void Execute() override;						
	int GetId() override { return transmitter_id_; };
private:
	EventList* event_list_ = nullptr;
	const int difs_time_ = 50;	// czas DIFS [ms](5ms *10)
	const size_t waiting_for_channel_ = 5;	// czas oczekiwania na kanal (0.5ms * 10)
	int transmitter_id_;// id nadajnika
};
#endif 
