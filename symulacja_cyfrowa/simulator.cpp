#include "simulator.h"
#include "generator.h"
#include <fstream>
#include <iterator>
#include <vector>
#include "basestation.h"


Simulator::Simulator(WiFi* wifi) : wifi_(wifi)
{
}
//---------------------------------Metoda wyboru trybu symulacji-------------------------------------------------------------
void Simulator::StepByStepMode()
{
    char znak;
    cout << "Press Enter to continue... \n";
    znak = getchar();
}

void Simulator::GetStats()
{
    int j = 0;                              // pomocniczy iterator petli
    double max_error_rate = 0.0;
    double error_system_average = 0.0;
    double sum_of_succes_tranmission = 0.0;
    double tmp_retransmissions = 0;
    double sum_tx_retransmissions = 0;
    double sum_retransmissions = 0;
    double sum_of_appearance_delay = 0;
    double sum_of_waiting_time = 0;
    vector<double> retranmissions_tx_;
    vector<double> errors_tx_average;
    vector<size_t>* packet_delay_times = wifi_->GetPacketDelayTimes();
    vector<size_t>* packet_waiting_times = wifi_->GetPacketWaitingTime();
    double sum = 0.0;
    size_t sim_time;

    cout << "\n";
    for (int i = 0; i < wifi_->GetBaseStationVector()->size(); i++)
    {
        double error_tx_average = 0.0;
        double succes = wifi_->GetBaseStationVector()->at(i)->GetNrofSucces();
        double fail = wifi_->GetBaseStationVector()->at(i)->GetNrOfFail();
        if ((wifi_->GetBaseStationVector()->at(i)->GetNrOfRetransmissionSucces()->size()) > 0)
        {
            for (j = 0; j < wifi_->GetBaseStationVector()->at(i)->GetNrOfRetransmissionSucces()->size(); j++)
            {
                tmp_retransmissions = wifi_->GetBaseStationVector()->at(i)->GetNrOfRetransmissionSucces()->at(j);
                sum_tx_retransmissions += tmp_retransmissions;
            }
            sum_retransmissions += sum_tx_retransmissions;
            sum_of_succes_tranmission += succes;
            sum_tx_retransmissions = 0;
            j = 0;
        }

        if (succes != 0)
            error_tx_average = fail / succes;
        else error_tx_average = 0.0;

        errors_tx_average.push_back(error_tx_average);
    }
    for (int i = 0; i < errors_tx_average.size(); i++)
    {
        if (max_error_rate < errors_tx_average.at(i))
        {
            max_error_rate = errors_tx_average.at(i);
        }
    }
    while (!errors_tx_average.empty())
    {
        sum += errors_tx_average.back();
        errors_tx_average.pop_back();
    }

    int size_of_packet_delay_times = packet_delay_times->size();
    while (!packet_delay_times->empty())
    {
        sum_of_appearance_delay += packet_delay_times->back();
        packet_delay_times->pop_back();
    }

    int size_of_packet_waiting_times = packet_waiting_times->size();
    while (!packet_waiting_times->empty())
    {
        sum_of_waiting_time += packet_waiting_times->back();
        packet_waiting_times->pop_back();
    }
    error_system_average = (sum / 10) * 100;
    sim_time = (clock_ - wifi_->GetStartSimTime()) / 10;
    //cout << "SIM TIME: " << sim_time/1000 << "\n";
    cout << "Average of packet delay time: " << (sum_of_appearance_delay / size_of_packet_delay_times) / 10 << " ms\n";
    cout << "Average of Packet waiting time: " << (sum_of_waiting_time / size_of_packet_waiting_times) / 10 << " ms\n";
    cout << "Average of error system rate(BER): " << error_system_average << " % \n";
    cout << "Max Tx error rate: " << max_error_rate * 100 << " % \n";
    cout << "Average number of retransmissions: " << sum_retransmissions / sum_of_succes_tranmission << "\n";
    cout << "Average number of packets per second: " << sum_of_succes_tranmission / (double(sim_time) / 1000) << "\n";
    cout << "number of succes: " << sum_of_succes_tranmission;
    cout << "\n";
}

//---------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------przyjalem, iz 1 kwant czasu w symulacji to 0.5 ms-----------------------------

void Simulator::M2(int time)
{
    char mode;                           // Tryb symulacji     1-krokowa 2-ciagla                             
    bool flag_mode;                      // Flaga do wyboru trybu symulacji 
    double lambda = wifi_->GetLambda();
    clock_ = 0;
    std::cout << "Started Simulation method M2: \n \n";
    std::cout << "Select Mode: ------- 1=Step by Step ------- 2=continous : ";
    cin >> mode;
    switch (mode)
    {
    case '1': flag_mode = true;
        break;
    case '2': flag_mode = false;
        break;
    default: flag_mode = false;
    }
    std::cout << "\n";

    auto cmp = [](Event* left, Event* right) { return left->get_time() > right->get_time(); };
    Event::EventList event_list(cmp);

    //------------------------------generowanie pierwszych pakietow dla nadajnikow-------------------------
    for (int i = 0; i < 8; i++)
    {
        int cgpk = wifi_->GetBaseStationVector()->at(i)->GetCgpkGenerator()->RndExp(lambda) * 10;
        wifi_->cgpk_numbers.push_back(cgpk);
        event_list.push(new GenerateEvent(cgpk, wifi_, &event_list, i));
    }
    //----------------------------------------------------------------------------------------------------

    while (clock_ < static_cast<size_t>(time))
    {
        Event* event = event_list.top();
        event_list.pop();
        clock_ = event->get_time();
        if (flag_mode == true)
            StepByStepMode();
        std::cout << "Simulation Time:" << clock_ << "\n";
        event->Execute();
    }

}
