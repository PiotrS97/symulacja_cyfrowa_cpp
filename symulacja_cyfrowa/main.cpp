#include <iostream>
#include "channel.h"
#include "package.h"
#include "simulator.h"
#include "basestation.h"
#include "wifi.h"
#include "event.h"
#include "generateevent.h"
#include "difswaitingevent.h"
#include "checkingchannelevent.h"
#include "ackevent.h"
#include "checkingcollisionevent.h"
#include "logger.h"
#include "generator.h"
#include "simulator.h"
#include <fstream>
#include <iterator>
#include <vector>

using namespace std;
int main()
{
	auto wifi = new WiFi();
	auto logger = new Logger();

	int seed = 123;
	auto generator = Generator(seed);
	vector<int> cgpk_kernels;
	vector<int> ctpk_kernels;
	vector<int> r_kernels;
	int ter_kernel;
	//---------------------------------generowanie ziaren dla CGP ---------------
	generator.GenerateKernels(80, cgpk_kernels);
	generator.ImportKernelsToFile(cgpk_kernels, "cgpk_kernels.txt");
	//---------------------------------generowanie ziaren dla CTP ---------------
	generator.GenerateKernels(80, ctpk_kernels);
	generator.ImportKernelsToFile(ctpk_kernels, "ctpk_kernels.txt");
	//---------------------------------generowanie ziaren dla R -----------------
	generator.GenerateKernels(80, r_kernels);
	generator.ImportKernelsToFile(r_kernels, "r_kernels.txt");
	//-----------------------------generowanie ziaren dla TER -------------------
	generator.GenerateKernels(ter_kernel);
	generator.ImportKernelsToFile(ter_kernel, "ter_kernel.txt");
	//---------------------------------------------------------------------------

	int sim_number = 1;
	int sim_loop_iterator;
	cout << "Suggested parameters: \n";
	cout << "Time -> 100000 \n";
	cout << "Initial phase -> 140 \n";
	cout << "Lambda -> 0.00585 \n \n";
	
	cout << "Enter Simulation number 1-8: \n";
	cin >> sim_number;
	cout << "\n";
	switch (sim_number)
	{
	case 1:
		sim_loop_iterator = 0;
		break;
	case 2:
		sim_loop_iterator = 10;
		break;
	case 3:
		sim_loop_iterator = 20;
		break;
	case 4:
		sim_loop_iterator = 30;
		break;
	case 5:
		sim_loop_iterator = 40;
		break;
	case 6:
		sim_loop_iterator = 50;
		break;
	case 7:
		sim_loop_iterator = 60;
		break;
	case 8:
		sim_loop_iterator = 70;
		break;
	default:
		sim_loop_iterator = 0;
		break;
	}

	int j = 0;
	for (int i = sim_loop_iterator; i < (sim_loop_iterator + 8); i++)
	{
		double kernel_cgpk = cgpk_kernels.at(i);
		wifi->GetBaseStationVector()->at(j)->SetCgpkKernel(kernel_cgpk);
		wifi->GetBaseStationVector()->at(j)->InitializeCgpkGenerator();

		double kernel_ctpk = ctpk_kernels.at(i);
		wifi->GetBaseStationVector()->at(j)->SetCtpkKernel(kernel_ctpk);
		wifi->GetBaseStationVector()->at(j)->InitializeCtpkGenerator();

		double kernel_r = r_kernels.at(i);
		wifi->GetBaseStationVector()->at(j)->SetRKernel(kernel_r);
		wifi->GetBaseStationVector()->at(j)->InitializeRGenerator();
		j++;
	}
	wifi->SetTerKernel(ter_kernel);
	wifi->InitializeTerGenerator();

	int sim_time = 500;
	double lambda = 0.05;
	int start_from_the_package = 0;
	cout << "Number of quantum simulation time - 1 quantum == 0.1ms \n";
	cin >> sim_time;
	cout << "\n";
	cout << "End of the initial phase (nr of package): \n";
	cin >> start_from_the_package;
	cout << "\n";
	cout << "Lambda parameter: \n";
	cin >> lambda;
	cout << "\n";
	Simulator simulator = Simulator (wifi);
	wifi->SetStartSimTime(start_from_the_package);
	wifi->SetLambda(lambda);
	simulator.M2(sim_time); //czas *10

	//--------------------zapis wygenerowanych wartosci---------------------------
	vector<double> numbers_cgpk = wifi->cgpk_numbers;
	vector<double> numbers_ctpk = wifi->ctpk_numbers;
	vector<double> numbers_r = wifi->r_numbers;
	vector<double> numbers_ter = wifi->ter_numbers;

	wifi->ImportNumbersToFile(numbers_cgpk, "cgpk_numbers.txt");
	wifi->ImportNumbersToFile(numbers_ctpk, "ctpk_numbers.txt");
	wifi->ImportNumbersToFile(numbers_r, "r_numbers.txt");
	wifi->ImportNumbersToFile(numbers_ter, "ter_numbers.txt");

	simulator.GetStats();
	return 0;
}