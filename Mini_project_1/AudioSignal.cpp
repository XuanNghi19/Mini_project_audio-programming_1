#include<vector>
#include<numeric>
#include "AudioSignal.h"
#include "gnuplot-iostream.h"
#include "wav.h"

AudioSignal::AudioSignal(std::vector<std::pair<double, double>> values, double rate)
{
    this->values = values;
    this->rate = rate;
}


void AudioSignal::timeShift(int pandemic) {
    std::vector<std::pair<double, double>> shifted_values(values.size() + std::abs(pandemic));

    if (pandemic < 0) {
        for (int i = 0; i < shifted_values.size(); i++)
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
        for (int i = shifted_values.size() - 1; i >= 0; i--)
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
}

void AudioSignal::reverseTime() {
    // Tạo một bản sao của vector giá trị âm thanh
    std::vector<std::pair<double, double>> reversed_values = values;

    // Đảo ngược chỉ số thời gian
    for (int i = 0; i < reversed_values.size(); ++i) {
        reversed_values[i].first = -reversed_values[i].first;
        // Lưu ý rằng nếu chỉ số thời gian ban đầu là dương, thì sau khi đảo ngược nó sẽ trở thành âm và ngược lại.
    }

    // Gán lại giá trị đảo ngược thời gian cho vector giá trị âm thanh gốc
    values = reversed_values;

    // Hiển thị dữ liệu âm thanh đã đảo ngược thời gian
    plot1();
}

void AudioSignal::downsample(int M)
{   
    rate /= M;
    // Tạo một vector mới để lưu trữ dữ liệu đã giảm tần số lấy mẫu
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

    // Gán lại giá trị đã giảm tần số lấy mẫu cho vector giá trị âm thanh gốc
    values = downsampled_values;

    // Hiển thị dữ liệu âm thanh đã giảm tần số lấy mẫu
    plot1();
}


void AudioSignal::upsample(int L) {
    rate *= L;
    // Tạo một vector mới để lưu trữ dữ liệu đã tăng tần số lấy mẫu
    std::vector<std::pair<double, double>> upsampled_values;

    long long bufferupsample = values[0].first;
    std::pair<double, double> tmp;

    for (int i = 0; i < values.size(); ++i) {
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
        for (int j = 1; j < L; ++j) {

            bufferupsample++;
            upsampled_values.push_back({ bufferupsample, 0});
        }
    }

    values = upsampled_values;

    plot2();
}
void AudioSignal::writeWavFile() {


    uint16_t channels = 1; // mono
    uint32_t rate = this->rate; // Hz
    uint16_t bitsPerSample = 16; // bits/sample
    uint32_t dataSize = values.size() * channels * bitsPerSample / 8; // kích thước dữ liệu âm thanh
    WavHeader header(channels, rate, bitsPerSample, dataSize);

    // Tên file đầu ra
    std::string filename = "../assets/output.wav";
    // Mở một file để ghi dữ liệu âm thanh vào
    std::ofstream outFile(filename, std::ios::binary);

    if (!outFile.is_open()) {
        std::cerr << "Không thể mở file để ghi dữ liệu." << std::endl;
        return;
    }

    outFile.write(reinterpret_cast<const char*>(&header), sizeof(header));

    for (const auto& sample : values) {
        int16_t sampleValue = static_cast<int16_t>(sample.second);
        outFile.write(reinterpret_cast<const char*>(&sampleValue), sizeof(sampleValue));
    }

    outFile.close();

    std::cout << "File WAV '" << filename << "' đã được ghi thành công." << std::endl;
}

void AudioSignal::plot1() const {
    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");
    
    gp << "set xlabel 'n'\nset ylabel 'x(n)'\n";
    gp << "plot '-' with points pointtype 7 title 'Point', '-' with lines title 'Line'" << std::endl;

    for (int i = 0; i < values.size(); i++) {

        gp << values[i].first << " " << values[i].second << std::endl;
    }
    gp << "e" << std::endl;

    for (int i = 0; i < values.size(); i++)
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
    gp << "plot '-' with lines title 'Line'" << std::endl;

    for (int i = 0; i < values.size(); i++)
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
        std::cerr << "Chỉ có thể cộng hai tín hiệu cùng rate\n";
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

AudioSignal AudioSignal::multiply(double constant) const {
    std::vector<std::pair<double, double>> result = this->values;
    for (long long i = 0; i < this->values.size(); i++) {
        result[i].second *= constant;
    }
    return AudioSignal(result, rate);
}