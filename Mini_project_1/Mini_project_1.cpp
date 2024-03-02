// test_gnuplot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include<numeric>
#include<random>

#include"gnuplot-iostream.h"
#include"AudioSignal.h"


int main()
{
    std::vector<std::pair<int, int>> signal_values;
    for (int i = 0; i < 5; i++)
    {
        std::pair<int, int> tmp;
        tmp.first = i;
        tmp.second = (i);
        signal_values.push_back(tmp);
    }
  
    double sampling_rate = 44100; // Hz
    AudioSignal signal(signal_values, sampling_rate);

    signal.plot();
    signal.timeShift(-2);
    
    return 0;
}

