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

AudioSignal audioSignal({}, 0);
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

void saveFile() {
    char s = '1';
    std::cout << "Do you want to save this signal to file?\n";
    std::cout << "y(Yes) / n(No)\n";
    std::cout << "select: ";
    std::cin >> s;
    if (s == 'y') {
        audioSignal.writeWavFile();
    }
}

bool openWavFile(){

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

        Wav tmp_Wav(filePath);
        audioSignal = AudioSignal(tmp_Wav.toSignals(), tmp_Wav.getSampleRate());

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
        s == '1' ? audioSignal.plot1() : audioSignal.plot2();
    }
}

void timeShifting() {

    if (!save) {
        openWavFile();
    }

    long long pandemic;
    std::cout << "Choose m = ";
    std::cin >> pandemic;
    audioSignal.timeShift(pandemic);

    saveSignal();
    plot();
    saveFile();
}

void reversaling() {

}

void signalSum() {

}

void signalMultiply() {

}

void signalMultiplyConstant() {

}

void Downsampling() {

}

void Upsampling() {

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
        std::cout << "              4. Multiply two discrete signals\n";
        std::cout << "              5. Multiply the constant by the discrete signal\n";
        std::cout << "              6. Downsampling\n";
        std::cout << "              7. Upsampling\n";
        std::cout << "              8. Quit\n";
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
            flag = false;
            break;
        }
    }

    return 0;
}
