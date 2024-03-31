#define _USE_MATH_DEFINES
#include <cmath>
#include <ios>
#include <iostream>
#include <vector>
#include <utility>
#include <cstdio>
#include <numeric>
#include <stdio.h>
#include "AudioSignal.h"
#include "gnuplot-iostream.h"
#include "wav.h"
#include <chrono>

AudioSignal::AudioSignal(std::vector<std::pair<double, double>> values, double rate)
{
    this->values = values;
    this->rate = rate;
}

//void AudioSignal::reverb( int delayMilliseconds, float decay) {
//    //int delayMilliseconds = 500; // half a second
//    int delaySamples =
//        (int)((float)delayMilliseconds * 44.1f); // assumes 44100 Hz sample rate
//    /*float decay = 0.5f;*/
//    for (int i = 0; i < values.size() - delaySamples; i++)
//    {
//        // WARNING: overflow potential
//        values[i + delaySamples] += (short)((float)values[i] * decay);
//    }
//}
void AudioSignal::applyReverb(double decay, double mix, double delayInMs, double wet, double reverberance) {
    int delayInSamples = static_cast<int>(delayInMs * rate / 1000.0);
    std::vector<std::pair<double, double>> reverbBuffer(delayInSamples, { 0.0, 0.0 });

    for (int i = 0; i < values.size(); ++i) {
        // Calculate reverb by mixing delayed samples with original samples
        values[i].first = (1.0 - wet) * values[i].first + wet * (values[i].first + mix * reverbBuffer[i % delayInSamples].first);
        values[i].second = (1.0 - wet) * values[i].second + wet * (values[i].second + mix * reverbBuffer[i % delayInSamples].second);

        // Update reverb buffer with decay
        reverbBuffer[i % delayInSamples].first = (1.0 - reverberance) * values[i].first * decay + reverberance * reverbBuffer[i % delayInSamples].first;
        reverbBuffer[i % delayInSamples].second = (1.0 - reverberance) * values[i].second * decay + reverberance * reverbBuffer[i % delayInSamples].second;
    }
}
void AudioSignal::fadeOut(double duration) {
    double fadeSamples = duration * rate;
    double fadeIncrement = 1.0 / fadeSamples;

    for (size_t i = 0; i < fadeSamples && i < values.size(); ++i) {
        double fade = 1 - (fadeIncrement * i);
        values[i].second *= fade;
    }
}
void AudioSignal::fadeIn(double duration) {
    double fadeSamples = duration * rate;
    double fadeIncrement = 1.0 / fadeSamples;

    for (size_t i = 0; i < fadeSamples && i < values.size(); ++i) {
        double fade =  (fadeIncrement * i);
        values[i].second *= fade;
    }
}
void AudioSignal::adjustVolume(double factor) {
    for (long long i = 0; i < values.size(); ++i) {
        values[i].second = static_cast<int16_t>(values[i].second * factor);
    }
}
void AudioSignal::applyEcho(double delay, double decay) {
    double delayInSamples = static_cast<double>((delay*2 * rate));
    std::vector<std::pair<double, double>> delayedValues(values.size(), { 0.0, 0.0 });

    for (long long  i = 0; i < values.size(); ++i) {
        if (i >= delayInSamples) {
            delayedValues[i].first = values[i - delayInSamples].first * decay;
            delayedValues[i].second = values[i - delayInSamples].second * decay;
        }
        // Chỉ cộng âm thanh trễ với âm thanh gốc nếu chúng cùng có dữ liệu
        if (i <= values.size() - delay) {
            values[i].first += delayedValues[i].first;
            values[i].second += delayedValues[i].second;
        }
    }
}



void AudioSignal::timeShift(long long pandemic) {
    std::vector<std::pair<double, double>> shifted_values(values.size() + std::abs(pandemic));

    if (pandemic < 0) {
        for (long long i = 0; i < shifted_values.size(); i++)
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
        for (long long i = shifted_values.size() - 1; i >= 0; i--)
        {
            if (i >= pandemic + 1 ) {
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
    std::cout << "Time Shifting success!\n";
}

void AudioSignal::reverseTime() {
    std::vector<std::pair<double, double>> reversed_values = values;
    std::reverse(reversed_values.begin(), reversed_values.end());
    for (long long i = 0; i < reversed_values.size(); ++i) {
        reversed_values[i].first = -reversed_values[i].first;
    }

    values = reversed_values;

    std::cout << "Reverse Time success!\n";
}

void AudioSignal::downsample(int M)
{   
    rate /= M;
    std::vector<std::pair<double, double>> downsampled_values;

    // Lấy mẫu mỗi M mẫu
    long long j = 0;
    for (long long i = 0; i < values.size(); i += M) {
        std::pair<double, double> tmp;
        tmp.second = values[i].second;
        tmp.first = j;
        j++;
        downsampled_values.push_back(tmp);
    }
    values = downsampled_values;
    std::cout << "Downsample success!\n";
}

void AudioSignal::upsample(int L) {
    rate *= L;
    std::vector<std::pair<double, double>> upsampled_values;

    long long bufferupsample = values[0].first;
    std::pair<double, double> tmp;

    for (long long i = 0; i < values.size(); ++i) {
        if (i != 0) {
            bufferupsample++;
        }
        tmp.second = values[i].second;
        tmp.first = bufferupsample;
        upsampled_values.push_back(tmp);
        if (i == values.size() - 1) {
            break;
        }

        // noi suy
        for (long long j = 1; j < L; ++j) {

            bufferupsample++;
            upsampled_values.push_back({ bufferupsample, 0});
        }
    }

    values = upsampled_values;

    std::cout << "Upsample success!\n";
}

void writeToFile(std::ofstream &file, int value, int size) {
    file.write(reinterpret_cast<const char*> (&value), size);
}

void AudioSignal::writeWavFile(WavHeader refWavHeader) {

    if (values == std::vector<std::pair<double, double>>{}) {
        std::cout << "You have no data to save!\n";
        return;
    }

    std::cin.get();
    std::cout << "Enter the audio file name: ";
    std::string name;
    std::getline(std::cin, name);
    std::string filename = "../assets/" + name + ".wav";

    std::ofstream outFile(filename, std::ios::binary);

    if (!outFile.is_open()) {
        std::cerr << "Cannot open file to write data." << std::endl;
        return;
    }

    std::cout << refWavHeader.numChannels << std::endl;
    std::cout << refWavHeader.bitsPerSample << std::endl;
    //Header chunk
    outFile << "RIFF";
    outFile << "----";
    outFile << "WAVE";

    //Format chunk
    outFile << "fmt ";
    writeToFile(outFile, 16, 4); //Length of format data as listed above
    writeToFile(outFile, 1, 2); //Type of format (1 is PCM) - 2 byte integer
    writeToFile(outFile, refWavHeader.numChannels, 2); //Number of Channels - 2 byte integer
    writeToFile(outFile, rate, 4); //Sample Rate - 32 byte integer
    writeToFile(outFile, (rate * refWavHeader.bitsPerSample * refWavHeader.numChannels) / 8, 4); //Byte rate
    writeToFile(outFile, (refWavHeader.bitsPerSample * refWavHeader.numChannels) / 8, 2); //Block align (bytes/sample)
    writeToFile(outFile, refWavHeader.bitsPerSample, 2); //Bits per sample

    //Date chunk
    outFile << "data";
    outFile << "----";
    int preAudioPosition = outFile.tellp();

    for (const auto& sample : values) {
        int16_t sampleValue = static_cast<int16_t>(sample.second);
        writeToFile(outFile, sampleValue, 2);
    }

    int posAudioPosition = outFile.tellp();
    outFile.seekp(preAudioPosition - 4);
    writeToFile(outFile, posAudioPosition - preAudioPosition, 4);

    outFile.seekp(4, std::ios::beg);
    writeToFile(outFile, posAudioPosition - 8, 4);

    outFile.close();

    std::cout << "File WAV '" << filename << "' was recorded successfully." << std::endl;
}

void AudioSignal::plot1() const {
    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");
    
    gp << "set xlabel 'n'\nset ylabel 'x(n)'\n";  
    gp << "plot '-' with points pointtype 7 title 'Point', '-' with lines title 'Line'" << std::endl;

    for (long long i = 0; i < values.size(); i++) {

        gp << values[i].first << " " << values[i].second << std::endl;
    }
    gp << "e" << std::endl;

    for (long long i = 0; i < values.size(); i++)
    {
        gp << values[i].first << " " << values[i].second << std::endl;
        gp << values[i].first << " 0" << std::endl;
        gp << std::endl;
    }

    gp << "e" << std::endl;


    std::cout << "Press enter to out!";
    std::cin.get();
}

void AudioSignal::plot2() const {

    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");

    gp << "set xlabel 'n'\nset ylabel 'x(n)'\n";
    gp << "set style line 1 lc rgb 'blue' pt 7\n";
    gp << "plot '-' with lines ls 1 title 'Line'" << std::endl;

    for (long long i = 0; i < values.size(); i++)
    {
        gp << values[i].second << std::endl;
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
        std::cerr << "You can only add two signals at the same rate\n";
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

AudioSignal AudioSignal::operator*(const AudioSignal& other) const {
    std::vector<std::pair<double, double>> result;

    if (this->rate != other.rate) {
        std::cerr << "You can only multiply two signals at the same rate\n";
        return AudioSignal(result, 0.0);
    }

    AudioSignal x_AudioSignal(values, rate);
    std::vector<std::pair<double, double>> x = x_AudioSignal.getValues();
    AudioSignal h_AudioSignal = other;
    std::vector<std::pair<double, double>> h = h_AudioSignal.getValues();
    
    long long n = 0, k = 0, N = x.size(), M = h.size(), i = 0;
    
    std::vector<double> y(N + M - 1);
    result.resize( N + M - 1);

    for (n = 0; n < N + M - 1; n++) {
        for (k = 0; k <= std::min(n, M - 1); k++) {
            if (n - k < N && k < M) {
                y[n] += x[n - k].second * h[k].second;
            }
        }
    }

    for (auto tmp : y) {
        result[i].first = i;
        result[i].second = tmp;
        ++i;
    }
    return AudioSignal(result, rate);
}

AudioSignal AudioSignal::multiplyConstant(double constant) const {
    std::vector<std::pair<double, double>> result = this->values;
    for (long long i = 0; i < this->values.size(); i++) {
        result[i].second *= constant;
    }
    std::cout << "The calculation is completed!\n";
    return AudioSignal(result, rate);
}

void AudioSignal::LPF(AudioSignal &refAudioSignal, double fc, double fs, int N) {
    std::vector<std::pair<double, double>> h;
    h.resize(N);
    double omegac = (2 * M_PI * fc) / fs;

    for (int n = 0; n < N; n++) {
        h[n].first = n;

        double x = omegac * (n - (N - 1) / 2.0);

        if (x == 0) {
            h[n].second = omegac / M_PI;
        }
        else {
            h[n].second = (omegac / M_PI) * (sin(x) / x);
        }
    }

    AudioSignal filter(h, fs);
    refAudioSignal = refAudioSignal * filter;
}

void AudioSignal::HPF(AudioSignal &refAudioSignal, double fc, double fs, int N) {
    std::vector<std::pair<double, double>> h;
    h.resize(N);
    double omegac = (2 * M_PI * fc) / fs;

    for (int n = 0; n < N; n++) {
        h[n].first = n;

        double x = omegac * (n - (N - 1) / 2.0);

        if (x == 0) {
            h[n].second = 1 - omegac / M_PI;
        }
        else {
            h[n].second = -(omegac / M_PI) * (sin(x) / x);
        }
    }

    AudioSignal filter(h, fs);
    refAudioSignal = refAudioSignal * filter;
}

void AudioSignal::BPF(AudioSignal &refAudioSignal, double fc1, double fc2, double fs, int N) {
    std::vector<std::pair<double, double>> h;
    h.resize(N);
    double omegac1 = (2 * M_PI * fc1) / fs; // tần số thấp
    double omegac2 = (2 * M_PI * fc2) / fs; // tần số cao

    for (int n = 0; n < N; n++) {
        h[n].first = n; 

        double x = (n - (N - 1) / 2.0);

        double sinc1 = (omegac1 * x == 0) ? 1 : sin(omegac1 * x) / (omegac1 * x);
        double sinc2 = (omegac2 * x == 0) ? 1 : sin(omegac2 * x) / (omegac2 * x);

        h[n].second = ((omegac2 / M_PI) * sinc2) - ((omegac1 / M_PI) * sinc1);

    }

    AudioSignal filter(h, fs);
    refAudioSignal = refAudioSignal * filter;
}

void AudioSignal::BSF(AudioSignal &refAudioSignal, double fc1, double fc2, double fs, int N) {
    std::vector<std::pair<double, double>> h;
    h.resize(N);
    double omegac1 = (2 * M_PI * fc1) / fs; // tần số thấp
    double omegac2 = (2 * M_PI * fc2) / fs; // tần số cao

    for (int n = 0; n < N; n++) {
        h[n].first = n;

        double x = (n - (N - 1) / 2.0);

        double sinc1 = (omegac1 * x == 0) ? 1 : sin(omegac1 * x) / (omegac1 * x);
        double sinc2 = (omegac2 * x == 0) ? 1 : sin(omegac2 * x) / (omegac2 * x);

        if (x == 0) {
            h[n].second = 1 - (
                ((omegac2 / M_PI) * sinc2) - ((omegac1 / M_PI) * sinc1)
                );
        }
        else {
            h[n].second = -(((omegac2 / M_PI) * sinc2) - ((omegac1 / M_PI) * sinc1));
        }

    }

    AudioSignal filter(h, fs);
    refAudioSignal = refAudioSignal * filter;
}