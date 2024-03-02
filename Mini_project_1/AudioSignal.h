#pragma once
#include "gnuplot-iostream.h"

class AudioSignal
{
public:
	AudioSignal(std::vector<std::pair<int, int>> values, double sampling_rate);
	void timeShift(int shift_samples);
	void plot() const;

	std::vector<std::pair<int, int>> getValues();
	void setValues(std::vector<std::pair<int, int>> values);
	double getRate();
	void setRate(double rate);

private:
	std::vector<std::pair<int, int>> values;
	double rate;
};

