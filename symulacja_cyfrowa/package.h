#ifndef PACKAGE_H
#define PACKAGE_H

class Package
{
public:
	Package(unsigned long int package_id, size_t time_of_appearance);
	~Package();
	unsigned long int GetId() { return id_; }//Odczyt ID pakietu
	int GetNumberOfPacketTransmission() { return nr_of_packet_transmissions_; }	//Odczyt liczby transmisji pakietu
	void SetNumberOfPacketTransmission(int number) { nr_of_packet_transmissions_ = number; }//ustawienie liczby transmisji pakietu
	void SetCtpkTime(int ctpk) { ctpk_time_ = ctpk; }//ustawia czasu CTPK
	int GetCtpkTime() { return ctpk_time_; }//Odczyt czasu CTPK
	size_t GetTimeOfAppearance() { return time_of_appearance_; }//Odczyt czasu pojawienia siê pakietu
private:
	unsigned long int id_;//ID pakietu
	int nr_of_packet_transmissions_;//Liczba transmisji pakietu
	int* ptr_nr_of_packet_transmissions = &nr_of_packet_transmissions_;
	int ctpk_time_;	// czas CTPK
	size_t time_of_appearance_; //czas pojawienia siê pakietu
};
#endif 
