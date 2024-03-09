#include<iostream>
#include "Wav.h"
#include <cstdlib>
#include <fstream>
#include<SFML/Audio.hpp>
#include<sndfile.hh>
#include<sndfile.h>
#include"gnuplot-iostream.h"


Wav::Wav(const std::string& filename) : filename(filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Couldn't open file: " << filename << std::endl;
        return;
    }

    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    std::cout << "sampleRate    : " << header.sampleRate << std::endl;

    if (header.bitsPerSample != 16) {
        std::cerr << "Error: Only 16-bit audio is supported." << std::endl;
        return;
    }

    uint32_t numSamples = header.subchunk2Size / (header.bitsPerSample / 8);
    this->data.reserve(numSamples);

    int16_t sample;
    while (file.read(reinterpret_cast<char*>(&sample), sizeof(int16_t))) {
        this->data.push_back(static_cast<double>(sample));
    }
}

Wav::Wav() {}

void Wav::playWav(std::string ref_Filename) {
    
    sf::SoundBuffer sb;
    std::cout << "Load file...................\n";
    if (!sb.loadFromFile(ref_Filename == "" ? this->filename : ref_Filename)) {
        std::cerr << "Failed to load the sound file." << std::endl;
        return;
    }

    sound.setBuffer(sb);
    std::cout << "Play sound!\n";
    sound.play();
    std::cout << "Press 'q' to quit." << std::endl;
    char input;
    while (true) {
        std::cin >> input;
        if (input == 'q') {
            sound.stop();
            break;
        }
    }
    
    return;
}
    
void Wav::printData() {
    for (auto x : data) {
        std::cout << x << " ";
    }
}

std::vector<std::pair<double, double>> Wav::toSignals() {
    std::vector<std::pair<double, double>> signals;
    for (long long i = 0; i < data.size(); i++) {
        std::pair<double, double> tmp;
        tmp.first = (double)i;
        tmp.second = data[i];
        signals.push_back(tmp);
    }

    return signals;
}

uint32_t Wav::getSampleRate() {
    return header.sampleRate;
}

WavHeader Wav::getWavHeader() {
    return header;
}