#include "checkingchannelevent.h"
#include "difswaitingevent.h"
#include <iostream>

CheckingChannelEvent::CheckingChannelEvent(size_t time, WiFi* wifi, EventList* event_list, int id)
	:Event(time, wifi), event_list_(event_list), transmitter_id_(id)
{
}

void CheckingChannelEvent::Execute()
{
	bool chanell_status = wifi_->GetChannelVector()->GetChannelStatus(); // zajetosc kana³u
	if (!wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->empty())
	{
		if (chanell_status) //kana³ wolny
		{
			//zaplanowanie zdarzenia odczekania czasu DIFS
			event_list_->push(new DIFSWaitingEvent(time_ + waiting_for_channel_, wifi_, event_list_, transmitter_id_));
			logger.Info("Waiting for a channel... ");
			cout << "BS nr: " << transmitter_id_ << " \n";
		}
		else //kanal zajety
		{
			int waiting_time_ = 0;	//zerowanie oczekiwania na kanal (nalezy zliczac od poczatku, poniewaz kanal jest zajety)
			wifi_->GetBaseStationVector()->at(transmitter_id_)->SetWaitingTime(waiting_time_);
			logger.Info("Channel is busy! \n");
			event_list_->push(new CheckingChannelEvent(time_ + waiting_for_channel_, wifi_, event_list_, transmitter_id_)); //planowanie zdarzenia nasluchiwania kanalu
		}
	}
	else wifi_->GetBaseStationVector()->at(transmitter_id_)->SetListeningStatus(false);
}