#include "basestation.h"
#include <iostream>

BaseStation::BaseStation(int id)
{
	//ustawienie warunków pocz¹tkowych stacji bazowej
	basestation_id_ = id;
	logger = new Logger();
	listening_ = false;
	while (!packages_.empty())
	{
		packages_.pop();
	}
	waiting_time_ = 0;
	nr_of_fails_ = 0;
	nr_of_successes_ = 0;
}

BaseStation::~BaseStation()
{
	packages_.pop();
}

void BaseStation::GeneratePackage(size_t time_of_appearance)
{
	static size_t package_id = 0;
	++package_id;
	auto package_ = new Package(package_id, time_of_appearance);
	packages_.push(package_);
}