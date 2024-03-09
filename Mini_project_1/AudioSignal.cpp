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

AudioSignal::AudioSignal(std::vector<std::pair<double, double>> values, double rate)
{
    this->values = values;
    this->rate = rate;
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

//void AudioSignal::plot1() const {
//    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");
//    
//    gp << "set xlabel 'n'\nset ylabel 'x(n)'\n";
//    gp << "plot '-' with points pointtype 7 title 'Point', '-' with lines title 'Line'" << std::endl;
//
//    for (long long i = 0; i < values.size(); i++) {
//
//        gp << values[i].first << " " << values[i].second << std::endl;
//    }
//    gp << "e" << std::endl;
//
//    for (long long i = 0; i < values.size(); i++)
//    {
//        gp << values[i].first << " " << values[i].second << std::endl;
//        gp << values[i].first << " 0" << std::endl;
//        gp << std::endl;
//    }
//
//    gp << "e" << std::endl;
//
//
//    std::cout << "Press enter to out!";
//    std::cin.get();
//}

#include <iostream>
#include <vector>
#include <utility>
#include <process.h>

void AudioSignal::plot1() const {
    std::vector<char> cmdline;
    cmdline.reserve(256);
    cmdline.push_back('"');
    cmdline.insert(cmdline.end(), "C:\\Program Files\\gnuplot\\bin\\gnuplot.exe", "C:\\Program Files\\gnuplot\\bin\\gnuplot.exe" + strlen("C:\\Program Files\\gnuplot\\bin\\gnuplot.exe"));
    cmdline.push_back('"');
    cmdline.push_back(' ');
    cmdline.insert(cmdline.end(), "-persist", "-persist" + strlen("-persist"));
    cmdline.push_back('\0');

    FILE* gnuplot_pipe = _popen(&cmdline[0], "wb");
    if (!gnuplot_pipe) {
        std::cerr << "Error opening Gnuplot pipe" << std::endl;
        return;
    }

    fprintf(gnuplot_pipe, "set xlabel 'n'\nset ylabel 'x(n)'\n");
    fprintf(gnuplot_pipe, "plot '-' binary%lu%lu%lu%lu ", sizeof(double), sizeof(double), 2 * sizeof(double), values.size());

    fwrite(&values[0], sizeof(std::pair<double, double>), values.size(), gnuplot_pipe);
    fprintf(gnuplot_pipe, "e\n");

    std::cout << "Press enter to exit!";
    std::cin.get();
    _pclose(gnuplot_pipe);
}

void AudioSignal::plot2() const {

    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");

    gp << "set xlabel 'n'\nset ylabel 'x(n)'\n";
    gp << "plot '-' with lines title 'Line'" << std::endl;

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

    std::cout << "The calculation is completed!\n";
    return AudioSignal(result, rate);
}

AudioSignal AudioSignal::operator*(const AudioSignal& other) const {
    std::vector<std::pair<double, double>> result;

    if (this->rate != other.rate) {
        std::cerr << "You can only multiply two signals at the same rate\n";
        return AudioSignal(result, 0.0);
    }

    long long i = 0, j = 0;

    while (i < values.size() || j < other.values.size()) {
        if (i < values.size() && j < other.values.size()) {
            if (values[i].first == other.values[j].first) {
                result.push_back({ values[i].first, values[i].second * other.values[j].second });
                i++;
                j++;
            }
            else if (values[i].first < other.values[j].first) {
                result.push_back({ values[i].first, 0 });
                i++;
            }
            else {
                result.push_back({ other.values[j].first, 0 });
                j++;
            }
        }
        else if (i < values.size()) {
            result.push_back({ values[i].first, 0 });
            i++;
        }
        else {
            result.push_back({ other.values[j].first, 0 });
            j++;
        }
    }
    std::cout << "The calculation is completed!\n";
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