#ifndef EVENT_H_
#define EVENT_H_
#include <functional>
#include <queue>
#include <vector>
#include "logger.h"

class Channel;
class Package;
class WiFi;

class Event
{
public:
	typedef std::priority_queue<Event*, std::vector<Event*>, std::function<bool(Event*, Event*)>> EventList;
	Event(size_t time, WiFi* wifi);
	~Event();
	virtual void Execute() = 0;
	long double get_time() {return time_;}
	virtual int GetId() { return NULL; };
protected:
	size_t time_; //czas symulacji
	Logger logger;
	WiFi* wifi_; //wskaünik na klase WiFi
};
#endif 
