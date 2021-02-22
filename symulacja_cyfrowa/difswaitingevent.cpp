#include "difswaitingevent.h"
#include "checkingchannelevent.h"
#include "checkingcollisionevent.h"
#include <iostream>


DIFSWaitingEvent::DIFSWaitingEvent(size_t time, WiFi* wifi, EventList* event_list, int id)
	:Event(time, wifi), event_list_(event_list), transmitter_id_(id)
{
}

void DIFSWaitingEvent::Execute()
{
	bool channel_status = wifi_->GetChannelVector()->GetChannelStatus();							//pobierz stan kanalu wolny/zajety
	int  w_time = wifi_->GetBaseStationVector()->at(transmitter_id_)->GetWaitingTime();			//pobierz czas oczekiwania na kanal
	w_time = w_time + waiting_for_channel_;																		//zwieksz czas oczekiwania na kanal (poniewaz w planowaniu zdarzenia uwzgledniono jednostke czasu)
	int ctpk = wifi_->GetBaseStationVector()->at(transmitter_id_)->GetCtpkGenerator()->Rnd(1, 10) * 10; //generuj czas wysylania pakietu (od 1 do 100 kwantow czasu)
	wifi_->ctpk_numbers.push_back(ctpk);
	static size_t start_transmission_time = 0;	//zmienna pomocnicza do wyznaczania kolizji

	if (!wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->empty()) //sprawdzenie warunku, czy aby na pewno nadajnik nie ma pustego bufora(zabezpieczenie)
	{
		if (channel_status)
		{
			if (w_time > difs_time_)				//jesli czas oczekiwania jest wiekszy od DIFS...
			{
				BaseStation* bs = wifi_->GetBaseStationVector()->at(transmitter_id_);
				start_transmission_time = time_;	//przypisuje do zmiennej pomocniczej aktualny czas
				wifi_->GetChannelVector()->SetChannelStatus(false);								//Ustaw kanal jako zajety
				wifi_->GetBaseStationVector()->at(transmitter_id_)->SetWaitingTime(0);			//wyzeruj zmienna waiting_time
				//wifi_->GetBaseStationVector()->at(transmitter_id_)->StartTransmission(bs);			//rozpoczecie transmisji
				wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->front()->SetCtpkTime(ctpk);	//ustawia czas ctpk dla pakietu
				wifi_->GetChannelVector()->SetCurrentTransmission(wifi_->GetBaseStationVector()->at(transmitter_id_)); //Dodanie do wektora zajetosci nadajnika, który aktualnie nadaje
				cout << "Getting Transmitter TEST: " << wifi_->GetChannelVector()->GetCurrentTranmission()->back()->GetBaseStationId() << "\n";
				event_list_->push(new CheckingCollisionEvent(time_ + ctpk, wifi_, event_list_, transmitter_id_));	//zaplanuj zdarzenie odebrania pakietu 
				cout << "CTPK: " << ctpk << "\n";
				Package* package = wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->front();
				size_t appearance_time = package->GetTimeOfAppearance();
				size_t waiting_packet_time = time_ - appearance_time;
				wifi_->SetPacketWaitingTime(waiting_packet_time);
			}
			else //nie minal czas konieczny do odczekania
			{
				wifi_->GetBaseStationVector()->at(transmitter_id_)->SetWaitingTime(w_time);							   //Ustaw czas oczekiwania 
				event_list_->push(new CheckingChannelEvent(time_, wifi_, event_list_, transmitter_id_));		   //za krotki czas wolnego kanalu
				logger.Info("Channel isn't free more than 3ms... Waiting time is: ");
				std::cout << (double)w_time / 10 << " [ms] TX: " << transmitter_id_ << " \n";
			}
		}
		else    //kanal zajety
		{
			if (w_time > difs_time_ && start_transmission_time == time_)
			{
				//-------------------------------------------------realizacja kolizji------------------------------------------------------------------
				wifi_->GetChannelVector()->SetCurrentTransmission(wifi_->GetBaseStationVector()->at(transmitter_id_)); //Dodanie do wektora zajetosci nadajnika, który aktualnie nadaje
				cout << "It will be a collission! " << transmitter_id_ << "\n";
				wifi_->GetBaseStationVector()->at(transmitter_id_)->SetWaitingTime(0);				//wyzeruj zmienna waiting_time
				BaseStation* bs = wifi_->GetBaseStationVector()->at(transmitter_id_);
				//wifi_->GetBaseStationVector()->at(transmitter_id_)->StartTransmission(tx);				//rozpoczecie transmisji
				cout << "Getting Transmitter TEST: " << wifi_->GetChannelVector()->GetCurrentTranmission()->back()->GetBaseStationId() << "\n";
				event_list_->push(new CheckingCollisionEvent(time_ + ctpk, wifi_, event_list_, transmitter_id_));	//zaplanuj zdarzenie odebrania pakietu 
				Package* package = wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->front();
				size_t appearance_time = package->GetTimeOfAppearance();
				size_t waiting_packet_time = time_ - appearance_time;
				wifi_->SetPacketWaitingTime(waiting_packet_time);
				//---------------------------------------------------------------------------------------------------------------------------------------
			}
			else
			{
				wifi_->GetBaseStationVector()->at(transmitter_id_)->SetWaitingTime(0);				//wyzeruj zmienna waiting_time
				event_list_->push(new CheckingChannelEvent(time_, wifi_, event_list_, transmitter_id_)); //kanal zajety wiec od poczatku nasluchuj kanal
				logger.Info("Channel is busy now... \n ");
			}
		}
	}
	else wifi_->GetBaseStationVector()->at(transmitter_id_)->SetListeningStatus(false); //NAdajnik juz nie nasluchuje
}