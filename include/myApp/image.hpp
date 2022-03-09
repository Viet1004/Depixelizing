#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <vector>
#include <tuple>
#include <iostream>
#include <opencv2/opencv.hpp>
class Image{
    cv::Mat mat_image;
    std::vector<std::vector<std::tuple<double, double, double>>> YUV_representation;
    std::tuple<double, double, double> convertTo_YUV(size_t i, size_t j);
    public:        
        Image(const std::string& file_name);
        ~Image();
        std::tuple<double, double, double> get_YUV(size_t i, size_t j);
        std::tuple<double, double, double> get_RGB(size_t i, size_t j);
        size_t get_height();
        size_t get_width();
};


#endif
