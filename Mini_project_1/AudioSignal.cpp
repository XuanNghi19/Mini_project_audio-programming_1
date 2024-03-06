#include<vector>
#include<numeric>
#include "AudioSignal.h"
#include "gnuplot-iostream.h"

AudioSignal::AudioSignal(std::vector<std::pair<double, double>> values, double rate)
{
	this->values = values;
	this->rate = rate;
}


void AudioSignal::timeShift(int pandemic) {
    std::vector<std::pair<double, double>> shifted_values(values.size() + std::abs(pandemic));

    if (pandemic < 0) {
        for (int i = 0; i < shifted_values.size(); i++)
        {
            if (i < values.size()) {
                shifted_values[i].first = values[i].first + pandemic;
                shifted_values[i].second = values[i].second;
            }
            else {
                shifted_values[i].first = shifted_values[i - 1].first + 1;
                shifted_values[i].second = 0;
            }

        }
    }
    else if (pandemic > 0) {
        for (int i = shifted_values.size() - 1; i >= 0; i--)
        {
            if (i > pandemic) {
                shifted_values[i].first = values[i - pandemic].first + pandemic;
                shifted_values[i].second = values[i - pandemic].second;
            }
            else {
                shifted_values[i].first = shifted_values[i + 1].first - 1;
                shifted_values[i].second = 0;
            }

        }
    }

    
    values = shifted_values;
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

std::vector<std::pair<double, double>> AudioSignal::getValues() {
    return this->values;
}

void AudioSignal::setValues(std::vector<std::pair<double, double>> values) {
    this->values = values;
}

double AudioSignal::getRate() {
    return this->rate;
}

void AudioSignal::setRate(double rate) {
    this->rate = rate;
}

AudioSignal AudioSignal::operator+(const AudioSignal& other) const {
    std::vector<std::pair<double, double>> result;
    
    if (this->rate != other.rate) {
        std::cerr << "Chỉ có thể cộng hai tín hiệu cùng rate\n";
        return AudioSignal(result, 0.0);
    }

    long long i = 0, j = 0;

    while (i < values.size() || j < other.values.size()) {
        if (i < values.size() && j < other.values.size()) {
            if (values[i].first == other.values[j].first) {
                result.push_back({ values[i].first, values[i].second + other.values[j].second });
                i++;
                j++;
            }
            else if (values[i].first < other.values[j].first) {
                result.push_back({ values[i].first, values[i].second });
                i++;
            }
            else {
                result.push_back({ other.values[j].first, other.values[j].second });
                j++;
            }
        }
        else if (i < values.size()) {
            result.push_back({ values[i].first, values[i].second });
            i++;
        }
        else {
            result.push_back({ other.values[j].first, other.values[j].second });
            j++;
        }
    }

    return AudioSignal(result, rate);
}

AudioSignal AudioSignal::multiply(double constant) const {
    std::vector<std::pair<double, double>> result = this->values;
    for (long long i = 0; i < this->values.size(); i++) {
        result[i].second *= constant;
    }
    return AudioSignal(result, rate);
}