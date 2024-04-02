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
#include <chrono>

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
            std::cout << "Lỗi: Không thể mở file " << filePath << std::endl;
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
    char s = 'y';
    std::cout << "Do you want to graph this signal?\n";
    std::cout << "y(Yes) / n(No)\n";
    std::cout << "select: ";
    std::cin >> s;
    if (s == 'y') {
        std::cin.get();
        audioSignal1.plot2();
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
    std::cout << "Enter the cutting frequency(fc): ";
    std::cin >> fc;
    std::cout << "Enter the steps of the filter(N): ";
    std::cin >> N;
    AudioSignal::LPF(audioSignal1, fc, audioSignal1.getRate(), N);

    std::cout << "LPF Filtered!\n";
}

void hpf() {
    double fc;
    int N;
    std::cout << "Enter the cutting frequency(fc): ";
    std::cin >> fc;
    std::cout << "Enter the steps of the filter(N): ";
    std::cin >> N;
    AudioSignal::HPF(audioSignal1, fc, audioSignal1.getRate(), N);

    std::cout << "HPF Filtered!\n";
}

void bpf() {
    double fc1, fc2;
    int N;
    std::cout << "Enter 2 cutting frequency(fc1, fc2): ";
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
    std::cout << "Choose a filter\n";
    std::cout << "  1. Low pass filter\n";
    std::cout << "  2. Hight pass filter\n";
    std::cout << "  3. Band pass filter\n";
    std::cout << "  4. Band stop filter\n";
    std::cout << "  0. Quit\n";
    int sl = 0;
    std::cout << "Select: ";
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
    case 0:
        return;
    }

    saveSignal();
    plot();
}

void echo() {
    if (!save) {
        openWavFile(audioSignal1);
    }
    double delay, decay;
    std::cout << "Enter the delay period: ";
    std::cin >> delay;
    std::cout << "Enter the volume decay coefficient: ";
    std::cin >> decay;
    audioSignal1.applyEcho(delay, decay);
}

void amplify() {
    if (!save) {
        openWavFile(audioSignal1);
    }
    double factor;
    std::cout << "Enter the volume up and down coefficient: ";
    std::cin >> factor;
    audioSignal1.adjustVolume(factor);
}

void fadeIn() {
    if (!save) {
        openWavFile(audioSignal1);
    }
    double duration;
    std::cout << "Enter the time fade in: ";
    std::cin >> duration;
    audioSignal1.fadeIn(duration);
}

void fadeOut() {
    if (!save) {
        openWavFile(audioSignal1);
    }
    double duration;
    std::cout << "Enter the time fade out: ";
    std::cin >> duration;
    audioSignal1.fadeOut(duration);
}

void reverb() {
    if (!save) {
        openWavFile(audioSignal1);
    }
    double decay, mix, delayInMs, wet, reverberance;
    std::cout << "Enter the volume decay coefficient: ";
    std::cin >> decay;
    std::cout << "Enter the ratio between the original signal and the Reverb signal: ";
    std::cin >> mix;
    std::cout << "Enter the delay time(ms): ";
    std::cin >> delayInMs;
    std::cout << "Enter the ratio of the Reverb signal to be added to the original signal: ";
    std::cin >> wet;
    std::cout << "Enter the coefficient showing the level of the reverb effect: ";
    std::cin >> reverberance;
    audioSignal1.applyReverb(decay, mix, delayInMs, wet, reverberance);
}

void flanging() {
    if (!save) {
        openWavFile(audioSignal1);
    }
    double A, r0, f, fs;
    std::cout << "Enter the amplifier coefficient (0 -> 1): ";
    std::cin >> A;
    std::cout << "Enter the maximum latency: ";
    std::cin >> r0;
    std::cout << "Enter the frequency of the lag control: ";
    std::cin >> f;

    audioSignal1.applyFlangingEffect(A, r0, f);
}


void effect() {

    std::cout << "Choose a effect\n";
    std::cout << "  1. Echo\n";
    std::cout << "  2. Amplify\n";
    std::cout << "  3. Fade in\n";
    std::cout << "  4. Fade out\n";
    std::cout << "  5. Reverb\n";
    std::cout << "  6. Flanging\n";
    std::cout << "  0. Quit\n";
    int sl = 0;
    std::cout << "Select: ";
    std::cin >> sl;
    switch (sl)
    {
    case 1:
        echo();
        break;
    case 2:
        amplify();
        break;
    case 3:
        fadeIn();
        break;
    case 4:
        fadeOut();
        break;
    case 5:
        reverb();
        break;
    case 6:
        flanging();
        break;
    case 0:
        return;
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
        std::cout << "              11.Play sound\n";
        std::cout << "              12.Open file\n";
        std::cout << "              13.Save file\n";
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
            effect();
            break;
        case 10:
            drawDiagram();
            break;
        case 11:
            playSound();
            break;
        case 12:
            openWavFile(audioSignal1);
            break;
        case 13:
            audioSignal1.writeWavFile(wav.getWavHeader());
            break;
        case 0:
            flag = false;
            break;
        }
    }

    return 0;
}
