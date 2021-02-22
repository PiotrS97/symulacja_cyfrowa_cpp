#include "package.h"
Package::Package(unsigned long int id, size_t time_of_appearance)
{
	id_ = id;
	nr_of_packet_transmissions_ = 0;
	time_of_appearance_ = time_of_appearance;
}

Package::~Package()
{
}