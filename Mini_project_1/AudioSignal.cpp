#include<vector>
#include<numeric>
#include "AudioSignal.h"
#include "gnuplot-iostream.h"

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
    // Tạo một vector mới để lưu trữ dữ liệu đã tăng tần số lấy mẫu
    std::vector<std::pair<double, double>> upsampled_values;

    // Tính toán dữ liệu tăng tần số lấy mẫu với nội suy
    for (int i = 0; i < values.size(); ++i) {
        // Thêm mẫu gốc vào vector kết quả
        upsampled_values.push_back(values[i]);

        // Nếu không phải mẫu cuối cùng và không phải mẫu mà không thể nội suy được
        if (i < values.size() - 1 && (values[i + 1].first - values[i].first) > L) {
            // Nội suy L − 1 mẫu giữa các mẫu gốc
            for (int j = 1; j < L; ++j) {
                int interpolated_time = values[i].first + j * (values[i + 1].first - values[i].first) / L;
                upsampled_values.push_back(std::make_pair(interpolated_time, 0)); // Nội suy với giá trị mặc định là 0
            }
        }
    }

    // Gán lại giá trị đã tăng tần số lấy mẫu cho vector giá trị âm thanh gốc
    values = upsampled_values;

    // Hiển thị dữ liệu âm thanh đã tăng tần số lấy mẫu
    plot1();
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