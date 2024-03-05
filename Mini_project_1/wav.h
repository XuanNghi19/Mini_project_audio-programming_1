#pragma once
#include <string>
#include <vector>
#include <SFML/Audio.hpp>

struct WavHeader {
    char riff[4];
    unsigned long chunkSize;
    char wave[4];
    char fmt[4];
    unsigned long subchunk1Size;
    unsigned short audioFormat;
    unsigned short numOfChan;
    unsigned long samplesPerSec;
    unsigned long bytesPerSec;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    char subchunk2ID[4];
    unsigned long subchunk2Size;
};

class Wav
{
public:
    Wav(const std::string& filename);

    void read();
    bool write(const std::string& filename);
    std::vector<std::pair<double, double>> toSignals();

    void printData();
    void playWav(std::string ref_Filename = "");
    std::string changeRate(int rate);
private:
    std::string filename;
    std::vector<double> data;
    WavHeader header;
    sf::Sound sound;
};

