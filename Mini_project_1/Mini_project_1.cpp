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

    Wav sample_wav("../assets/test.wav");
    /*sample_wav.playWav();*/

    //AudioSignal sample_signal(sample_wav.toSignals(), 44100);
    //std::vector<std::pair<double, double>> values;
    //values.push_back({ 0, 1 });
    //values.push_back({ 1, 2 });
    //values.push_back({ 2, 3 });
    //values.push_back({ 3, 4 });
    //values.push_back({ 4, 5 });
    AudioSignal sample_signal(sample_wav.toSignals(), 44100);
    /*sample_signal.plot1();*/
    /*sample_signal.upsample(5);*/
    sample_signal.writeWavFile();

    return 0;
}