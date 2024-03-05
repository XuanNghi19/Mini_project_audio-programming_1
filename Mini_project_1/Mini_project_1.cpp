// test_gnuplot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include<numeric>
#include<random>

#include"gnuplot-iostream.h"
#include"AudioSignal.h"
#include"Wav.h"


int main()
{

    double sampling_rate = 18000; // Hz
    //AudioSignal signal(signal_values, sampling_rate);

    Wav sample_wav("../assets/test.wav");
    sample_wav.read();
    /*sample_wav.playWav();*/
    //sample_wav.printData();

    AudioSignal sample_signal(sample_wav.toSignals(), sampling_rate);
    /*sample_signal.plot1();*/
    /*sample_signal.timeShift(-10000);
    sample_signal.plot1();
    sample_signal.reverseTime();*/
    sample_signal.upsample(2);

    return 0;
}