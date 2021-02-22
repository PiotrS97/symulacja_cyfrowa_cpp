#ifndef CHECKINGCHANNLEVENT_H_
#define CHECKINGCHANNLEVENT_H_

#include "event.h"
#include "channel.h"
#include "wifi.h"
#include <functional>
#include "basestation.h"
#include "logger.h"

class Channel;
class WiFi;

class CheckingChannelEvent : public Event
{
public:
	CheckingChannelEvent(size_t time, WiFi* wifi, EventList* event_list, int id);
	void Execute() override;
	int GetId() override { return transmitter_id_; };
private:
	EventList* event_list_ = nullptr;
	const int waiting_for_channel_ = 5;	//czas oczekiwania na kanal (0.5ms * 10)
	int transmitter_id_;//id nadajnika
};
#endif 