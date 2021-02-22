#include "wifi.h"
#include <iostream>
WiFi::WiFi()
{
	for (int i = 0; i < k_; i++)
	{
		auto n_transmitter_ = new BaseStation(i);
		bs_.push_back(n_transmitter_);

	}
	number_of_retransmissions_ = 0;
	channel = new Channel();
	actual_number_of_received_packages_ = 0;
	lambda_ = 0.05;
}

WiFi::~WiFi()
= default;

void WiFi::ImportNumbersToFile(std::vector<double>& numbers, std::string filename)
{
	std::ofstream output_file("./" + filename);
	std::ostream_iterator<double> output_iterator(output_file, "\n");
	std::copy(numbers.begin(), numbers.end(), output_iterator);
}

void WiFi::SaveAverageNumberOfRetransmission()
{
	static size_t package_id = 0;
	++package_id;
	double average = 0;

	if (GetNumberOfAllPackages() == 0)
		average = (double)GetNumberOfRetransmissions();
	else
		average = (double)GetNumberOfRetransmissions() / GetNumberOfAllPackages();

	ofstream save_average_number_of_retransmission("AverageNumberOfRetransmission.txt", ios_base::app);
	save_average_number_of_retransmission << "package: " + to_string(package_id)
		+ " average: " + to_string(average) << endl;
	save_average_number_of_retransmission.close();
}

int WiFi::GetNumberOfAllPackages()
{
	int tx_sum = 0;
	int sum = 0;
	for (int i = 0; i < 10; i++)
	{
		sum += (bs_.at(i)->GetNrofSucces() + bs_.at(i)->GetNrOfFail());
	}
	return sum;
}