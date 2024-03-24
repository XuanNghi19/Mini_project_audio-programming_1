#pragma once
#include "gnuplot-iostream.h"
#include "wav.h"

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
	AudioSignal operator+(const AudioSignal& other) const;
	AudioSignal operator*(const AudioSignal& other) const;
	AudioSignal multiplyConstant(double constant) const;
	static void LPF(AudioSignal& refAudioSignal, double fc, double fs, int N);
	static void HPF(AudioSignal& refAudioSignal, double fc, double fs, int N);
	static void BPF(AudioSignal& refAudioSignal, double fc1, double fc2, double fs, int N);
	static void BSF(AudioSignal& refAudioSignal, double fc1, double fc2, double fs, int N);
private:
	std::vector<std::pair<double, double>> values = {};
	uint32_t rate = 0;
};