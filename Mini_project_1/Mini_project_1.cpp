// test_gnuplot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

bool openWavFile(std::string& filePath){

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
            std::cout << "Lỗi: Không thể mở file " << filePath << std::endl;
            return false;
        }

        return true;
    }
    else
    {
        std::cout << "Hủy bỏ chọn file" << std::endl;
        return false;
    }
}

void timeShifting() {
    std::string filePath;
    openWavFile(filePath);
    std::cin.get();
    Wav tmp_Wav(filePath);
    AudioSignal tmp_AudioSignal(tmp_Wav.toSignals(), 44200);
    tmp_AudioSignal.timeShift(1000);
    tmp_AudioSignal.plot();
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
        case 8:
            flag = false;
            break;
        }
    }

    return 0;
}
