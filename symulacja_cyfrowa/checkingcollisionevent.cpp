#include "checkingcollisionevent.h"
#include "generateevent.h"
#include "ackevent.h"
#include "checkingchannelevent.h"
#include <iostream>

CheckingCollisionEvent::CheckingCollisionEvent(size_t time, WiFi* wifi, EventList* event_list, int id)
	:Event(time, wifi), event_list_(event_list), transmitter_id_(id)
{
}

void CheckingCollisionEvent::Execute()
{
	bool flag = false; //zmienna pomocnicza, wykorzystana w petli poszukiwania wektora
	int i = 0;		   //iterator w petli while

	logger.Info("Received a package! BS nr: ");
	cout << transmitter_id_ << " \n";

	cout << "number of current transmission: " << wifi_->GetChannelVector()->GetCurrentTranmission()->size() << "\n";
	int ter_rnd = wifi_->GetTerGenerator()->RndZeroOne(0.8); //1-brak bledu 0-blad
	wifi_->ter_numbers.push_back(ter_rnd);

	if (wifi_->GetChannelVector()->GetCurrentTranmission()->size() > 1) //jesli nadaje wiecej niz 1 nadajnik - ustawia kolizje
	{
		wifi_->GetChannelVector()->SetCollission(true);
		cout << "USTAWIONO KOLIZJE\n";
	}

	for (int i = 0; i < wifi_->GetChannelVector()->GetCurrentTranmission()->size(); i++)
	{
		BaseStation* bs = wifi_->GetChannelVector()->GetCurrentTranmission()->at(i);
		std::cout << "CURRENT TX ID: " << bs->GetBaseStationId() << "\n";
	}

	if ((wifi_->GetChannelVector()->GetCollision()) == true || ter_rnd == 0)
	{
		cout << "ERROR LOOP \n";
		if (wifi_->GetChannelVector()->GetCollision() == true)
		{
			logger.Info("COLLISION! \n");
			cout << "COLLISION FOR TX: " << transmitter_id_ << "\n";
			wifi_->GetBaseStationVector()->at(transmitter_id_)->SetAck(false);

			while (flag == false)
			{
				if (transmitter_id_ == wifi_->GetChannelVector()->GetCurrentTranmission()->at(i)->GetBaseStationId())
				{
					cout << "POP TX: " << wifi_->GetChannelVector()->GetCurrentTranmission()->at(i)->GetBaseStationId() << "\n";
					wifi_->GetChannelVector()->GetCurrentTranmission()->erase(wifi_->GetChannelVector()->GetCurrentTranmission()->begin() + i);
					flag = true;
				}
				++i;
			}
		}
		else if (ter_rnd == 0)
		{
			//wystapienie bledu TER
			wifi_->GetBaseStationVector()->at(transmitter_id_)->SetAck(false);	//ACK = false
			cout << "TER ERROR ! TX: ";
			cout << transmitter_id_ << " \n";
			wifi_->GetChannelVector()->GetCurrentTranmission()->pop_back();
			//-----------------------------------------------------------------------------------------------------------------------
		}

		if (wifi_->GetChannelVector()->GetCurrentTranmission()->empty())
		{
			cout << "Unset Collission...\n";
			wifi_->GetChannelVector()->SetCollission(false);
			wifi_->GetChannelVector()->SetChannelStatus(true);
		}
		event_list_->push(new ACKEvent(time_ + 10, wifi_, event_list_, transmitter_id_));	//zaplanuj zdarzenie odbioru ACK
	}

	else //brak kolizji i bledu ter
	{
		wifi_->GetChannelVector()->GetCurrentTranmission()->pop_back();
		wifi_->GetBaseStationVector()->at(transmitter_id_)->SetAck(true);  // ustaw status ACK w transmiterze 
		event_list_->push(new ACKEvent(time_ + 10, wifi_, event_list_, transmitter_id_));	//zaplanuj zdarzenie odbioru ACK
	}
}