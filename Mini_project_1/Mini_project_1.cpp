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
  
    double sampling_rate = 44100; // Hz
    //AudioSignal signal(signal_values, sampling_rate);

    Wav sample_wav("../assets/test.wav");
    sample_wav.read();
    AudioSignal sample_signal(sample_wav.toSignals(), sampling_rate);

    Wav sample_wav2("../assets/test2.wav");
    sample_wav2.read();
    AudioSignal sample_signal2(sample_wav2.toSignals(), sampling_rate);
    
    //sample_signal.timeShift(-10000);
    AudioSignal sample_signal3 = sample_signal.multiply(2);
    sample_signal3.plot1();
    //sample_signal2.timeShift(10000);
    //sample_signal2.plot1();
    //AudioSignal addSignal = sample_signal + sample_signal2;

    //addSignal.plot1();
    //sample_wav.playWav();
    //sample_wav.printData();


    //sample_signal.plot1();
    //sample_signal.plot1();

    return 0;
}