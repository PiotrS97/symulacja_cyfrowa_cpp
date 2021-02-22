#ifndef WIFI_H_
#define WIFI_H_

#include "basestation.h"
#include "channel.h"
#include"generator.h"
#include "event.h"
#include <vector>
#include <queue>
using namespace std;

class WiFi
{
public:

	WiFi();
	~WiFi();
	vector<BaseStation*>* GetBaseStationVector() { return ptr_t; }	//Pobranie wskaznika na wektor stacji bazowej
	Channel* GetChannelVector() { return channel; }//Odczyt kanalu
	//Generatory
	void SetTerKernel(int& kernel) { ter_kernel_ = kernel; }//ustawienie ziarna TER
	double GetTerKernel() { return ter_kernel_; }//odczyt ziarna dla TER
	void InitializeTerGenerator() { ter_generator_ = new Generator(ter_kernel_); }	//inicjacja generatora TER
	Generator* GetTerGenerator() { return ter_generator_; }//Odczyt generatora TER
	vector<double> cgpk_numbers;
	vector<double> ctpk_numbers;
	vector<double> r_numbers;
	vector<double> ter_numbers;
	void ImportNumbersToFile(std::vector<double>& numbers, std::string filename);//Zapis wygenerowanych liczb do pliku
	//Zbieranie statystyk
	void SetStartSimTime(int start_from_the_package) { start_from_the_package_ = start_from_the_package; } //Ustawienie fazy pocz¹tkowej
	int GetStartSimTime() { return start_from_the_package_; }//Odczyt fazy pocz¹tkowej
	void SetPacketDelayTimes(size_t time) { packet_delay_times.push_back(time); }// Ustawienie opoznienia pakietu
	vector<size_t>* GetPacketDelayTimes() { return ptr_packet_delay_times; }//Odczyt opoznienia pakietu
	void SaveAverageNumberOfRetransmission();// Zapis sredniej liczby retransmisji
	int GetNumberOfAllPackages();//Odczyt liczby wszystkich wygenerowanych pakietow
	int GetNumberOfRetransmissions() { return number_of_retransmissions_; }	// Odczyt liczby retransmisji
	void SetNumberOfRetransmissions() { ++number_of_retransmissions_; }	// Ustawienie liczby retransmisji
	void SetActualNumberOfReceivedPackages() { ++actual_number_of_received_packages_; }// Ustawienie liczby otrzymanych pakietow
	int GetActualNumberOfReceivedPackages() { return actual_number_of_received_packages_; }	//Odczyt liczby otrzymanych pakietow
	void SetPacketWaitingTime(size_t time) { packet_waiting_time_.push_back(time); } // Ustawienie czasu oczekiwania pakietu
	vector<size_t>* GetPacketWaitingTime() { return ptr_packet_waiting_time_; }	//Odczyt czas oczekiwania pakietu
	int GetNumberOfTransmitters() { return k_; }//Odzyt liczby stacji bazowych
	void SetLambda(double lambda) {lambda_ = lambda;} //Ustawinie lambdy
	double GetLambda() { return lambda_; }//Odczyt lambdy
private:
	vector<BaseStation*> bs_;// wektor stacji nadawczych
	vector<BaseStation*>* ptr_t = &bs_;
	Channel* channel;// kanal 
	vector<size_t> packet_delay_times;// Wektor opoznien pakietow
	vector<size_t>* ptr_packet_delay_times = &packet_delay_times;
	vector<size_t> packet_waiting_time_;//wektor oczekiwania pakietow
	vector<size_t>* ptr_packet_waiting_time_ = &packet_waiting_time_;
	const int k_ = 8;// liczba stacji bazowych
	double ter_kernel_;	// ziarno generatora TER
	Generator* ter_generator_;// Generator TER
	int start_from_the_package_;//Faza pocz¹tkowa
	int number_of_retransmissions_;	//liczba retransmisji
	int actual_number_of_received_packages_;//liczba odebranych pakietow
	double lambda_;
};
#endif
