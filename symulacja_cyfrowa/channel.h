#ifndef CHANNEL_H
#define CHANNEL_H

#include <vector>
#include <queue>
#include "basestation.h"

class WiFi;

class Channel
{
public:
	Channel();
	~Channel();
	void SetCollission(bool status) { collision_ = status; }// ustawienie statusu kolizji
	bool GetCollision() { return collision_;}//odczyt statusu kolizji True - tak False - nie
	void SetChannelStatus(bool status) { channel_ = status; }// Ustawienie statusu zajêtoœci kanalu 
	bool GetChannelStatus() { return channel_; }//Odczyt statusu zajêtoœci kanalu 
	std::vector<BaseStation*>* GetCurrentTranmission() { return ptr_current_transmitters; } //odczyt wektora aktualnie nadajacych nadajnikow
	void SetCurrentTransmission(BaseStation* bs) {current_transmitters.push_back(bs); }	// ustawienie wektora aktualnie nadajacych nadajnikow
	

private:
	bool collision_;//Status kolizji True - kolizja False - brak kolizji
	bool channel_;// zajêtoœæ kana³u TRUE -> wolny FALSE -> zajety
	std::vector<BaseStation*> current_transmitters;	//wektor zajetosci kanalu
	std::vector<BaseStation*>* ptr_current_transmitters = &current_transmitters; 
};
#endif
