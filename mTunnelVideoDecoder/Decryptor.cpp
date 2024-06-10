#include "Decryptor.h"
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include <random>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <tchar.h>
#include <stdio.h>

#include "../Reed-Solomon//include//rs.hpp"



using namespace cv;


// Decode the QR code
bool Decryptor::decode(cv::Mat& crop) {
    double w_begin = w_pad - 1;
    double h_begin = h_pad; //- 1;
    double step = this->step + 0.006; //For block size 8: +0.01;

    std::vector<int> bits;
    double w = w_begin + step / 2;
    double h = h_begin + step / 2;



    for (int i = 0; i < h_num; i++) {
        auto color_value = crop.ptr<cv::Vec3b>(int(round(h)));
        for (int j = 0; j < w_num; j++) {
            for (int k = 0; k < 3; k++) {
                if (int(color_value[int(round(w))][k]) < 128) {
                    bits.push_back(0);
                }
                else {
                    bits.push_back(1);
                }
            }
            w += step;
        }
        h += step;
        w = w_begin + step / 2;
    }

    std::vector<uchar> Bytes;
    for (int i = 0; i <= bits.size() - 8; i+=8) {
        int temp = 0;
        for (int j = 0; j < 8; j++) {
            temp = temp + (bits[i + j] << (7 - j));    
        }
        Bytes.push_back(temp);
    }

    const int msglen = 100;
    const int ECC_LENGTH = 4;
    RS::ReedSolomon<msglen, ECC_LENGTH> rs;
    std::vector<uchar> decoded;

    int err_counter = 0;

    unsigned video_len = (Bytes[0] << 8) + Bytes[1]; // First two bytes are used to transmit the frame size
    std::cout << "video bytes: " << video_len << '\n';

    int rs_num = video_len / msglen + (video_len % msglen == 0 ? 0 : 1);
    for (int i = 0; i < rs_num; i++) {
        std::vector<uchar>::const_iterator begin = Bytes.begin() + 2 + i * (msglen+ ECC_LENGTH);
        std::vector<uchar>::const_iterator end = Bytes.begin() + 2 + (i + 1) * (msglen + ECC_LENGTH);
        std::vector<uchar> temp(begin, end);
        std::vector<uchar> repaired(msglen);
        rs.Decode(temp.data(), repaired.data());
        for (int j = 0; j < msglen; ++j) {
            if (temp[j] != repaired[j])
                err_counter += 1;
        }
        decoded.insert(decoded.end(), repaired.begin(), repaired.end());
    }
    std::cout << "Error Byte rate: " << double(err_counter) / video_len << '\n';


    std::cout << "figSize: " << decoded.size() << std::endl;
    crop = cv::imdecode(std::vector<uchar>(decoded.begin(), decoded.begin()+ decoded.size()), cv::IMREAD_COLOR);

    if (crop.empty()) {
        std::cout << "Image Corrupted" << std::endl;
        return false;
    }
    return true;
}


// Calculate the position of bounding vertice
std::vector<int> cal_vertex(std::vector<cv::Point> boundRect) {
    int x_max = 0;
    int x_min = 3840;
    int y_max = 0;
    int y_min = 3840;
    for (int i = 0; i < boundRect.size(); i++) {
        auto point = boundRect[i];
        x_min = point.x < x_min ? point.x : x_min;
        x_max = point.x > x_max ? point.x : x_max;
        y_min = point.y < y_min ? point.y : y_min;
        y_max = point.y > y_max ? point.y : y_max;
    }
    std::vector<int> result = {y_min , y_max, x_min , x_max };
    return result;
}


// Main loop, extract and decode the QR codes from the captured window
void start(std::string title, int widthClient, int heightClient) {
    std::string command = "ffmpeg -f gdigrab -framerate 10 -draw_mouse 0 -video_size " + std::to_string(widthClient) + 'x' + std::to_string(heightClient) + " -i title=\"" + title + "\" -color_range 2 -sample_fmt u8 -vcodec rawvideo -pix_fmt bgr24 -an -sn -f image2pipe -";
    //If the window does not support gdigrab, you have to capture the whole desktop
    // std::string command = "ffmpeg -f gdigrab -framerate 10 -draw_mouse 0" + std::string(" -i desktop -color_range 2 -sample_fmt u8 -vcodec rawvideo -pix_fmt bgr24 -an -sn -f image2pipe -");

    std::cout << command.c_str() << std::endl;
    FILE* fp = _popen(command.c_str(), "rb");
    if (!fp)
        std::cout << "Failed to pipe" << std::endl;

    unsigned size = heightClient * widthClient * 3;
    unsigned char* rawData = new unsigned char[size];

    std::vector<class Decryptor> decryptors;
    unsigned decryptor_index = 0;

    int capture_init = 10;
    while (capture_init-- > 0) {
        if(fread(rawData, 1, size, fp) == size){

        cv::Mat image = cv::Mat(cv::Size(widthClient, heightClient), CV_8UC3, rawData, cv::Mat::AUTO_STEP).clone();
        cv::Mat grey, dst;
        cv::cvtColor(image, grey, cv::COLOR_RGB2GRAY); 
        cv::threshold(grey, dst, 250, 255, cv::THRESH_BINARY);
        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(dst, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

        for (int i = 0; i < hierarchy.size(); i++) {
            auto outer_area = cv::contourArea(contours[i]);
            if (outer_area < 30000)
                continue;

            auto info = hierarchy[i];

            if (info[3] == -1) {  // [3] = -1->no parent; [2] >= 0->has children
                auto boundRect = contours[i];
                auto rect = cal_vertex(boundRect);

                // ratio -> 16:9
                float ratio = float(rect[3] - rect[2]) / (rect[1] - rect[0]);
                if (ratio > 16 / 8 || ratio < 1.6) {
                    std::cout << "Ratio not fit: "<< ratio << std::endl;
                    continue;
                }

                auto decryptor = class Decryptor(rect, widthClient, heightClient);
                decryptors.push_back(decryptor);
                std::cout << "Target detected" << std::endl;

                // For Demo, only detect one target/
                capture_init = -1;
                break;
            }
            }
        }
        
    }
    std::cout << "Start decoding!" << std::endl;

    while (1) {
        fread(rawData, 1, size, fp);
        cv::Mat image = cv::Mat(heightClient, widthClient, CV_8UC3, rawData, cv::Mat::AUTO_STEP).clone();
        
        for (int i = 0; i < decryptors.size(); i++) {
            auto decryptor = decryptors[i];
            auto bbox = decryptor.bbox;
            int bhei = bbox[1] - bbox[0];
            int bwid = bbox[3] - bbox[2];
            cv::Mat crop = image(Rect(bbox[2], bbox[0], bwid, bhei));
            if (decryptor.decode(crop)) {     
                Mat decoded(bhei, bwid, crop.type());    
                cv::resize(crop, decoded, decoded.size(), 0, 0, INTER_LINEAR);
                decoded.copyTo(image(Rect(bbox[2], bbox[0], bwid, bhei))); 
                imshow("Decrypted Window", image);
            }
            cv::waitKey(1);
        }
    }
    fflush(fp);
    fclose(fp);
}
