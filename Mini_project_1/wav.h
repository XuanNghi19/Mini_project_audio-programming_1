#pragma once
#include <string>
#include <vector>
#include <SFML/Audio.hpp>

struct WavHeader {
    // Chunk RIFF
    char chunkId[4];        // Chunk ID: "RIFF"
    uint32_t chunkSize;     // Chunk size
    char format[4];         // Format: "WAVE"

    // Subchunk 1 (fmt)
    char subchunk1Id[4];    // Subchunk 1 ID: "fmt "
    uint32_t subchunk1Size; // Subchunk 1 size: 16
    uint16_t audioFormat;   // Audio format (PCM = 1)
    uint16_t numChannels;   // Number of channels (mono = 1, stereo = 2)
    uint32_t sampleRate;    // Sample rate (Hz)
    uint32_t byteRate;      // Byte rate (bytes/second)
    uint16_t blockAlign;    // Block align (bytes/sample)
    uint16_t bitsPerSample; // Bits per sample

    // Subchunk 2 (data)
    char subchunk2Id[4];    // Subchunk 2 ID: "data"
    uint32_t subchunk2Size; // Subchunk 2 size (data size)

    // Hàm khởi tạo
    WavHeader(uint16_t channels, uint32_t rate, uint16_t bitsPerSample, uint32_t dataSize) {
        numChannels = channels;
        sampleRate = rate;
        this->bitsPerSample = bitsPerSample;
        byteRate = sampleRate * numChannels * bitsPerSample / 8;
        blockAlign = numChannels * bitsPerSample / 8;
        chunkSize = 36 + dataSize; // 36 = kích thước header cố định
        subchunk2Size = dataSize;
    }
    WavHeader() {}
};

class Wav
{
public:
    Wav(const std::string& filename);

    std::vector<std::pair<double, double>> toSignals();

    void printData();
    void playWav(std::string ref_Filename = "");
private:
    std::string filename;
    std::vector<double> data;
    WavHeader header;
    sf::Sound sound;
};

