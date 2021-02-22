#include "ackevent.h"
#include "checkingchannelevent.h"
#include "generateevent.h"
#include <iostream>
#include<iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

ACKEvent::ACKEvent(size_t time, WiFi* wifi, EventList* event_list, int id)
	:Event(time, wifi), event_list_(event_list), transmitter_id_(id)
{
}

void ACKEvent::Execute()
{

	size_t additional_time = 5; //Okres nasluchiwania kanalu
	int start_phase = wifi_->GetStartSimTime(); //Zmienna opisuj¹ca fazê pocz¹tkow¹
	int when_start = wifi_->GetActualNumberOfReceivedPackages();//Liczba odebranych pakietó
	bool IsMoreThanStartPhase = false; //Zmienna bool true ->zbierz statystyki, false -> nie zbieraj statystyk
	int number_of_retransmissions_ = wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->front()->GetNumberOfPacketTransmission();//pobierz liczbe transmisji pakietu

//czy rozpocz¹æ zbieranie statystyk
	if (when_start >= start_phase)
	{
		IsMoreThanStartPhase = true;
	}
	else IsMoreThanStartPhase = false;

	bool ack = wifi_->GetBaseStationVector()->at(transmitter_id_)->GetAck();//Odczyt ACK


	if (ack == true) //sprawdzenie ACK
	{
		cout << "SUCCES! BS nr:  " << transmitter_id_ << "\n";

		if (IsMoreThanStartPhase)
		{
			//Zbieranie statystyk
			size_t appearance_time = wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->front()->GetTimeOfAppearance();
			size_t delay_time = time_ - appearance_time;
			cout << "DELAY TIME: " << delay_time << "\n";
			wifi_->GetBaseStationVector()->at(transmitter_id_)->SetNrOfSucces();
			wifi_->GetBaseStationVector()->at(transmitter_id_)->SetNrOfRetransmissionSucces(number_of_retransmissions_);
			wifi_->SetPacketDelayTimes(delay_time);
		}
		wifi_->SetActualNumberOfReceivedPackages();
		wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->pop();
		wifi_->GetChannelVector()->SetChannelStatus(true);
		//zaplanowanie nas³uchiwania kana³u przez nastêpny pakiet
		if (wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->size() > 0)
		{
			event_list_->push(new CheckingChannelEvent(time_ + additional_time, wifi_, event_list_, transmitter_id_));
		}
		//Zakoñczenie nas³uchiwania przez dan¹ stacjê bazow¹
		else
		{
			wifi_->GetBaseStationVector()->at(transmitter_id_)->SetListeningStatus(false);
		}
	}
	else //brak ACK
	{

		if (number_of_retransmissions_ < 4) //sprawdzenie czy przekroczono liczbe retransmisji
		{
			//Zaplanowanie retransmisji
			wifi_->SetNumberOfRetransmissions();
			cout << "RETRANSMISSION! BS nr:" << transmitter_id_ << " \n";
			int temp_nr_retransmission = wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->front()->GetNumberOfPacketTransmission() + 1;
			wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->front()->SetNumberOfPacketTransmission(temp_nr_retransmission);  //ustaw liczbe transmisji pakietu
			int ctpk = wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->front()->GetCtpkTime();
			int r = wifi_->GetBaseStationVector()->at(transmitter_id_)->GetRGenerator()->Rnd(0, pow(2, temp_nr_retransmission) - 1) * 10;
			wifi_->r_numbers.push_back(r);
			int crp_ = r * ctpk;
			cout << "CRP: " << crp_ << "\n";
			event_list_->push(new CheckingChannelEvent(time_ + crp_, wifi_, event_list_, transmitter_id_));
		}
		else
		{
			//przekroczono limit transmisji
			//usun pakiet
			cout << "ERROR...Delete Package! \n";
			if (IsMoreThanStartPhase)
			{
				wifi_->GetBaseStationVector()->at(transmitter_id_)->SetNrOfFail();
			}
			wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->pop();
			if (!wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->empty())
			{
				event_list_->push(new CheckingChannelEvent(time_ + additional_time, wifi_, event_list_, transmitter_id_));
			}
			else wifi_->GetBaseStationVector()->at(transmitter_id_)->SetListeningStatus(false);
		}

	}
}