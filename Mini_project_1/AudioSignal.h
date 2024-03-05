#pragma once
#include "gnuplot-iostream.h"

class AudioSignal
{
public:
	AudioSignal(std::vector<std::pair<double, double>> values, double sampling_rate);
	void timeShift(int shift_samples);
	void plot1() const;
	void plot2() const;

	std::vector<std::pair<double, double>> getValues();
	void setValues(std::vector<std::pair<double, double>> values);
	double getRate();
	void setRate(double rate);

	AudioSignal operator+(const AudioSignal& other) const;
	AudioSignal multiply(double constant) const;
private:
	std::vector<std::pair<double, double>> values;
	double rate;
};

