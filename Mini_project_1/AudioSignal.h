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
	void applyReverb(double decay, double mix, double delayInMs, double wet, double reverberance, WavHeader wavHeader);
	void fadeOut(double duration);
	void applyFlangingEffect(double A, double r0, double f);
	AudioSignal operator+(const AudioSignal& other) const;
	AudioSignal operator*(const AudioSignal& other) const;
	AudioSignal multiplyConstant(double constant) const;
	AudioSignal cross_correlation(AudioSignal other);
	static double windows(int window, int n, int N);
	static void LPF(AudioSignal& refAudioSignal, double fc, double fs, int N, int window);
	static void HPF(AudioSignal& refAudioSignal, double fc, double fs, int N, int window);
	static void BPF(AudioSignal& refAudioSignal, double fc1, double fc2, double fs, int N, int window);
	static void BSF(AudioSignal& refAudioSignal, double fc1, double fc2, double fs, int N, int window);
private:
	std::vector<std::pair<double, double>> values = {};
	uint32_t rate = 0;
};