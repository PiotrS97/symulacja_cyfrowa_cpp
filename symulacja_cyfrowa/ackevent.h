#ifndef ACKEVENT_H_
#define ACKEVENT_H_

#include "event.h"
#include "wifi.h"
#include <functional>
#include "channel.h"
#include "basestation.h"
#include <vector>
class ACKEvent : public Event
{
public:
	ACKEvent(size_t time, WiFi* wifi, EventList* eventlist_, int id); 
	void Execute() override;
private:
	EventList* event_list_ = nullptr;
	const int lr_ = 10;
	int transmitter_id_;
};
#endif 
