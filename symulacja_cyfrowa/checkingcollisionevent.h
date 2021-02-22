#ifndef CHECKINGCOLLISIONEVENT_H_
#define CHECKINGCOLLISIONEVENT_H_

#include "event.h"
#include "wifi.h"
#include <functional>
#include "basestation.h"
#include "package.h"
#include "channel.h"

class CheckingCollisionEvent : public Event
{
public:
	CheckingCollisionEvent(size_t time, WiFi* wifi, EventList* event_list, int id);
	int GetId() override { return transmitter_id_; };
	void Execute() override;

private:
	EventList* event_list_ = nullptr;
	int transmitter_id_;
};
#endif 
