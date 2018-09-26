#include <fstream>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#define TAR 202021.250

void flowrite(cv::String addr, cv::Mat &flo) {
    cv::Size imgsize = flo.size();
    int high = imgsize.height;
    int width = imgsize.width;
    std::ofstream fout(addr,std::ios::binary);
    char * data = flo.ptr<char>(0);
    if (!fout) { return; }
    else {
        fout << "PIEH";
        fout.write((char*)&width, sizeof(int));
        fout.write((char*)&high, sizeof(int));
        fout.write(data, high * width * 2 * sizeof(float));
    }
    fout.close();
}
void floread(cv::String addr, cv::Mat &flo) {
    std::ifstream fin(addr, std::ios::binary);
    char buffer[sizeof(float)];
    fin.read(buffer, sizeof(float));
    float tar = ((float*)buffer)[0];
    if (tar != TAR) {
        fin.close();
        return;
    }
    fin.read(buffer, sizeof(int));
    int high = ((int*)buffer)[0];
    fin.read(buffer, sizeof(int));
    int width = ((int*)buffer)[0];
    flo = cv::Mat(cv::Size(high, width), CV_32FC2);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < high; j++) {
            if (!fin.eof()) {
                float * data = flo.ptr<float>(i, j);
                fin.read(buffer, sizeof(float));
                data[0] = ((float*)buffer)[0];
                fin.read(buffer, sizeof(float));
                data[1] = ((float*)buffer)[0];
            }
        }
    }

    fin.close();
}
