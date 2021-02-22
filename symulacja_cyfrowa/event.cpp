#include "event.h"

Event::Event(size_t time, WiFi* wifi) :
	time_(time), wifi_(wifi)
{

}

Event::~Event()
{
}