#pragma once
#include "gnuplot-iostream.h"
#include "wav.h"
#include <chrono>

class AudioSignal
{
public:
	AudioSignal(std::vector<std::pair<double, double>> values, double sampling_rate);
	void timeShift(long long shift_samples);
	void plot1() const;
	void plot2() const;
	void writeWavFile(WavHeader refWavHeader);

	std::vector<std::pair<double, double>> getValues();
	void setValues(std::vector<std::pair<double, double>> values);
	double getRate();
	void setRate(double rate);
	void reverseTime();
	void downsample(int M);
	void upsample(int L);
	void applyEcho(double delay, double decay);
	void adjustVolume(double factor);
	void fadeIn(double duration);
	/*void reverb(int delayMilliseconds, float decay);*/
	void applyReverb(double decay, double mix, double delayInMs, double wet, double reverberance);
	void fadeOut(double duration);
	AudioSignal operator+(const AudioSignal& other) const;
	AudioSignal operator*(const AudioSignal& other) const;
	AudioSignal multiplyConstant(double constant) const;
private:
	std::vector<std::pair<double, double>> values = {};
	uint32_t rate = 0;
};