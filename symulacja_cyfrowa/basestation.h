#ifndef BASESTATION_H_
#define BASESTATION_H_
#include <queue>
#include "package.h"
#include "logger.h"
#include "generator.h"
#include <vector>

class BaseStation
{
public:
	BaseStation(int id);
	~BaseStation();
	void GeneratePackage(size_t time_of_appearance); //Generacja pakietu
	void SetNrOfRetransmissionSucces(int retransmissions) { number_of_retransmission_succes_.push_back(retransmissions); }	//Ustawianie liczby retransmisji
	std::vector<int>* GetNrOfRetransmissionSucces() { return ptr_number_of_retransmission_succes_; }//Odczyt retransmisji
	std::queue<Package*>* GetPackageVector() { return ptr; }//metoda pobierajaca wskaznik na bufor pakietow
	int GetBaseStationId() { return basestation_id_; }	//Odczyt ID stacji bazowej
	bool GetAck() {	return ack;}//Odczyt ACK, True - > otrzymano False -> nie otrzymano
	void SetAck(bool status) { ack = status; }	//Ustawienie ACK 
	double GetWaitingTime() { return waiting_time_; }//Odczyt czasu oczekiwania na wolny kanal
	void SetWaitingTime(int time) { waiting_time_ = time; }	// ustawienie czasu oczekiwania na wolny kanal
	void SetListeningStatus(bool listening) { listening_ = listening; }	// Ustawienie nas³uchiwania kana³u
	bool getListening() { return listening_; }//Odczyt nas³uchiwania kana³u true -> nas³uchuje, false -> nie nas³uchuje
	void SetNrOfSucces() { ++nr_of_successes_; }//Zwiêkszenie liczby pomyslnie przeslanych pakietow
	long int GetNrofSucces() { return nr_of_successes_; }//Odczyt liczby pomyslnie przeslanych pakietow
	void SetNrOfFail() { ++nr_of_fails_; }	//Zwiêkszenie liczby b³êdnie przeslanych pakietow
	long int GetNrOfFail() { return nr_of_fails_; }	//Odczyt liczby b³êdnie przeslanych pakietow
	//metody odpowiedzialne za generacje rozkladow
	//CGPK
	void SetCgpkKernel(double& kernel) {cgpk_kernel_ = kernel;}
	double GetCgpkKernel(){ return cgpk_kernel_; }
	void InitializeCgpkGenerator(){ cgpk_generator_ = new Generator(cgpk_kernel_); }
	Generator* GetCgpkGenerator() { return cgpk_generator_; }
	//CTPK
	void SetCtpkKernel(double& kernel) { ctpk_kernel_ = kernel; }
	double GetCtpkKernel() { return ctpk_kernel_; }
	void InitializeCtpkGenerator() { ctpk_generator_ = new Generator(ctpk_kernel_); }
	Generator* GetCtpkGenerator() { return ctpk_generator_; }
	//R
	void SetRKernel(double& kernel) { r_kernel_ = kernel; }
	double GetRKernel() { return r_kernel_; }
	void InitializeRGenerator() { r_generator_ = new Generator(r_kernel_); }
	Generator* GetRGenerator() { return r_generator_; }
private:
	std::queue<Package*> packages_; // kolejka pakietow
	std::queue<Package*>* ptr = &packages_;
	std::vector<int> number_of_retransmission_succes_; //liczba pomyslnych retransmisji
	std::vector<int>* ptr_number_of_retransmission_succes_ = &number_of_retransmission_succes_;
	Logger* logger;//logger
	Generator* cgpk_generator_;	// generator rozkladu wykladniczego cgpk
	Generator* ctpk_generator_;	// generator rozkladu jednostajnego ctpk
	Generator* r_generator_;// generator rozkladu jednostajnego r
	double cgpk_kernel_;// ziarno generatora cgpk
	double ctpk_kernel_;// ziarno generatora ctpk
	double r_kernel_;// ziarno generatora r
	int basestation_id_;// ID nadajnika
	long int nr_of_successes_;	// liczba udanych transmisji 
	long int nr_of_fails_;	// liczba blednych transmisji    
	bool ack;// zmienna ACK True -> otrzymano
	int waiting_time_;//czas oczekiwania na wolny kanal
	bool listening_;// zmienna nas³uchiwania kana³u
	
};
#endif
