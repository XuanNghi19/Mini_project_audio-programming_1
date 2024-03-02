#include<vector>
#include "AudioSignal.h"
#include "gnuplot-iostream.h"

AudioSignal::AudioSignal(std::vector<std::pair<int, int>> values, double rate)
{
	this->values = values;
	this->rate = rate;
}

void AudioSignal::timeShift(int pandemic) {
    std::vector<std::pair<int, int>> shifted_values = values;

    for (int i = 0; i < shifted_values.size(); i++)
    {
        shifted_values[i].first = values[i].first + pandemic;
    }

    values = shifted_values;
    plot();
}

void AudioSignal::plot() const {
    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");

    gp << "set xlabel 'n'\nset ylabel 'x(n)'\n";
    gp << "plot '-' with points pointtype 7 title 'Point', '-' with lines title 'Line'" << std::endl;

    for (int i = 0; i < values.size(); i++) {

        gp << values[i].first << " " << values[i].second << std::endl;
    }
    gp << "e" << std::endl;

    for (int i = 0; i < values.size(); i++)
    {
        gp << values[i].first << " " << values[i].second << std::endl;
        gp << values[i].first << " 0" << std::endl;
        gp << std::endl;
    }
    
    gp << "e" << std::endl;


    std::cout << "Press enter to out!";
    std::cin.get();
}

std::vector<std::pair<int, int>> AudioSignal::getValues() {
    return this->values;
}

void AudioSignal::setValues(std::vector<std::pair<int, int>> values) {
    this->values = values;
}

double AudioSignal::getRate() {
    return this->rate;
}

void AudioSignal::setRate(double rate) {
    this->rate = rate;
}