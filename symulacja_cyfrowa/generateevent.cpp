#include "generateevent.h"
#include "checkingchannelevent.h"
#include<iostream>
#include <cstdlib>
#include <ctime>


GenerateEvent::GenerateEvent(size_t time, WiFi* wifi, EventList* event_list, int id)
	:Event(time, wifi), event_list_(event_list), transmitter_id_(id)
{
}


void GenerateEvent::Execute()
{

	double lambda = wifi_->GetLambda();
	wifi_->GetBaseStationVector()->at(transmitter_id_)->GeneratePackage(time_);	//wygenerowanie pakietu w stacji bazowej i umieszczenie go w buforze (otrzymuje on unikalne id)
	logger.Info("Packet generated for base station nr: ");
	std::cout << transmitter_id_ << "\n";
	//Generator czasu do wygenerowania nastêpnego pakietu
	int cgpk = wifi_->GetBaseStationVector()->at(transmitter_id_)->GetCgpkGenerator()->RndExp(lambda) * 10; //generacja czasu cgp (*10)
	wifi_->cgpk_numbers.push_back(cgpk);
	cout << "CGPK: " << cgpk << "\n";
	event_list_->push(new GenerateEvent(time_ + cgpk, wifi_, event_list_, transmitter_id_));//zaplanowanie generacji kolejnego pakietu
	//sprawdzenie zajêtoœci kana³u jeœli w stacji bazowej znajduje siê pakiet oraz jesli dana stacja bazowa jeszcze nie nas³uchuje
	if (wifi_->GetBaseStationVector()->at(transmitter_id_)->GetPackageVector()->size() > 0 && wifi_->GetBaseStationVector()->at(transmitter_id_)->getListening() == false)
	{
		event_list_->push(new CheckingChannelEvent(time_, wifi_, event_list_, transmitter_id_)); //sprawdzenie zajêtoœci kana³u
		wifi_->GetBaseStationVector()->at(transmitter_id_)->SetListeningStatus(true); //stacja bazowa zaczyna nas³uchiwaæ
	}
}
