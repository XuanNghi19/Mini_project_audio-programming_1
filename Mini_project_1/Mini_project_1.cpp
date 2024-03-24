#include <iostream>
#include<vector>
#include<numeric>
#include<random>
#include<string>

#include"gnuplot-iostream.h"
#include"AudioSignal.h"
#include"Wav.h"
#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<Windows.h>
#include<sndfile.h>
#include<sndfile.hh>

AudioSignal audioSignal1({}, 0);
AudioSignal audioSignal2({}, 0);
Wav wav;
bool save = false;

void saveSignal() {
    char s = 'n';
    std::cout << "Do you want to save this audio signal for the next operation?\n";
    std::cout << "y(Yes) / n(No)\n";
    std::cout << "select: ";
    std::cin >> s;
    if (s == 'y') {
        save = true;
    }
    else {
        save = false;
    }
}

bool openWavFile(AudioSignal& refAudioSignal){

    std::string filePath;
    char fileNameBuffer[MAX_PATH] = "";

    // Tạo một cấu trúc để chứa thông tin về loại file
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = fileNameBuffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Audio Files (*.wav)\0*.wav\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Hiển thị hộp thoại chọn file
    if (GetOpenFileNameA(&ofn))
    {
        filePath = fileNameBuffer;
        sf::SoundBuffer buffer;

        // Nạp file .wav vào SoundBuffer
        if (!buffer.loadFromFile(filePath))
        {
            std::cout << "Error: Cannot open file " << filePath << std::endl;
            return false;
        }

        wav = Wav(filePath);
        refAudioSignal = AudioSignal(wav.toSignals(), wav.getSampleRate());

        return true;
    }
    else
    {
        std::cout << "Cancel file selection" << std::endl;
        return false;
    }
}

void plot() {
    char s = '1';
    std::cout << "Do you want to graph this signal?\n";
    std::cout << "y(Yes) / n(No)\n";
    std::cout << "select: ";
    std::cin >> s;
    if (s == 'y') {
        std::cout   << "draw type 1 chart (select 1)\n"
                    << "draw type 2 chart (select 2) \n";
        std::cout << "select: ";
        std::cin >> s;
        std::cin.get();
        s == '1' ? audioSignal1.plot1() : audioSignal1.plot2();
    }
}

void timeShifting() {

    if (!save) {
        openWavFile(audioSignal1);
    }

    long long pandemic;
    std::cout << "Choose m = ";
    std::cin >> pandemic;
    audioSignal1.timeShift(pandemic);

    saveSignal();
    plot();
}

void reversaling() {

    if (!save) {
        openWavFile(audioSignal1);
    }
    audioSignal1.reverseTime();
    saveSignal();
    plot();
}

void signalSum() {
    if (!save) {
        std::cout << "Select signal file 1\n";
        openWavFile(audioSignal1);
    }
    std::cout << "Select signal file 2\n";
    openWavFile(audioSignal2);

    audioSignal1 = audioSignal1 + audioSignal2;

    saveSignal();
    plot();
}

void signalMultiply() {
    if (!save) {
        std::cout << "Select signal file 1\n";
        openWavFile(audioSignal1);
    }
    std::cout << "Select signal file 2\n";
    openWavFile(audioSignal2);

    audioSignal1 = audioSignal1 * audioSignal2;

    saveSignal();
    plot();
}

void signalMultiplyConstant() {
    if (!save) {
        openWavFile(audioSignal1);
    }

    long long constant;
    std::cout << "Choose constant = ";
    std::cin >> constant;
    audioSignal1 = audioSignal1.multiplyConstant(constant);

    saveSignal();
    plot();
}

void Downsampling() {
    if (!save) {
        openWavFile(audioSignal1);
    }

    int M;
    std::cout << "Choose M = ";
    std::cin >> M;
    audioSignal1.downsample(M);

    saveSignal();
    plot();
}

void Upsampling() {
    if (!save) {
        openWavFile(audioSignal1);
    }

    int L;
    std::cout << "Choose L = ";
    std::cin >> L;
    audioSignal1.upsample(L);

    saveSignal();
    plot();
}

void drawDiagram() {
    openWavFile(audioSignal1);
    plot();
}

void playSound() {
    openWavFile(audioSignal1);
    wav.playWav("");
}


void lpf() {
    double fc;
    int N;
    std::cout << "Enter the cutting frequency(fc)";
    std::cin >> fc;
    std::cout << "Enter the steps of the filter(N)";
    std::cin >> N;
    AudioSignal::LPF(audioSignal1, fc, audioSignal1.getRate(), N);

    std::cout << "LPF Filtered!\n";
}

void hpf() {
    double fc;
    int N;
    std::cout << "Enter the cutting frequency(fc)";
    std::cin >> fc;
    std::cout << "Enter the steps of the filter(N)";
    std::cin >> N;
    AudioSignal::HPF(audioSignal1, fc, audioSignal1.getRate(), N);

    std::cout << "HPF Filtered!\n";
}

void bpf() {
    double fc1, fc2;
    int N;
    std::cout << "Enter 2 cutting frequency(fc1, fc2: )";
    std::cin >> fc1 >> fc2;
    std::cout << "Enter the steps of the filter(N): ";
    std::cin >> N;
    AudioSignal::BPF(audioSignal1, fc1, fc2, audioSignal1.getRate(), N);

    std::cout << "BPF Filtered!\n";
}

void bsf() {
    double fc1, fc2;
    int N;
    std::cout << "Enter 2 cutting frequency(fc1, fc2): ";
    std::cin >> fc1 >> fc2;
    std::cout << "Enter the steps of the filter(N): ";
    std::cin >> N;
    AudioSignal::BSF(audioSignal1, fc1, fc2, audioSignal1.getRate(), N);

    std::cout << "BSF Filtered!\n";
}

void filter() {
    if (!save) {
        openWavFile(audioSignal1);
    }

    std::cout << "Choose a filter\n";
    std::cout << "  1. Low pass filter\n";
    std::cout << "  2. Hight pass filter\n";
    std::cout << "  3. Band pass filter\n";
    std::cout << "  4. Band stop filter\n";
    std::cout << "  0. Quit\n";
    int sl = 0;
    std::cin >> sl;
    switch (sl)
    {
    case 1:
        lpf();
        break;
    case 2:
        hpf();
        break;
    case 3:
        bpf();
        break;
    case 4:
        bsf();
        break;
    }

    saveSignal();
    plot();
}

int main()
{  
    bool flag = true;
    while (flag) {
        int select;
        std::cout << "_____________MINI_PROJECT_SOUND_PROGRAMING_1_____________\n";
        std::cout << "              1. Time shifting\n";
        std::cout << "              2. Time reversal\n";
        std::cout << "              3. Sum two discrete signals\n";
        std::cout << "              4. Signal convolution\n";
        std::cout << "              5. Multiply the constant by the discrete signal\n";
        std::cout << "              6. Downsampling\n";
        std::cout << "              7. Upsampling\n";
        std::cout << "              8. Filter\n";
        std::cout << "              9. Effect\n";
        std::cout << "              10.Draw diagram\n";
        std::cout << "              11.Save file\n";
        std::cout << "              12.Open file\n";
        std::cout << "              0. Quit\n";
        std::cout << "__________________________________________________________\n";
        std::cout << "Select: ";
        std::cin >> select;
        
        switch (select)
        {
        case 1:
            timeShifting();
            break;
        case 2:
            reversaling();
            break;
        case 3:
            signalSum();
            break;
        case 4:
            signalMultiply();
            break;
        case 5:
            signalMultiplyConstant();
            break;
        case 6:
            Downsampling();
            break;
        case 7:
            Upsampling();
            break;
        case 8:
            filter();
            break;
        case 9:
            
            break;
        case 10:
            drawDiagram();
            break;
        case 11:
            audioSignal1.writeWavFile(wav.getWavHeader());
            break;
        case 12:
            openWavFile(audioSignal1);
            break;
        case 0:
            flag = false;
            break;
        }
    }

    return 0;
}
