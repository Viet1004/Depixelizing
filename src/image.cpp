#include "../include/myApp/image.hpp"
#include <iostream>

Image::Image(const std::string& file_name){
    mat_image = cv::imread(file_name);
//    std::cout << mat_image.rows << " constructor Image " << mat_image.cols << std::endl;
    
    std::vector<std::tuple<double, double, double>> temp(mat_image.cols);
    YUV_representation.resize(mat_image.rows);
    fill(YUV_representation.begin(), YUV_representation.end(), temp);
    
    for (size_t i = 0; i < (size_t)mat_image.rows; i++){
        for(size_t j =0; j < (size_t)mat_image.cols; j++){
            YUV_representation[i][j] = convertTo_YUV(i,j);

        }
    }
}

size_t Image::get_height(){
    return (size_t)mat_image.rows;
}

size_t Image::get_width(){
    return (size_t)mat_image.cols;
}


std::tuple<double, double, double> Image::convertTo_YUV(size_t i, size_t j){
    double r = mat_image.at<cv::Vec3b>(i,j)[0];
    double g = mat_image.at<cv::Vec3b>(i,j)[1];
    double b = mat_image.at<cv::Vec3b>(i,j)[2]; 

    double y = 0.299 * r + 0.587 * g + 0.114 * b;
    double u = 0.492 * (b - y);
    double v = 0.877 * (r - y);
    return std::make_tuple(y,u,v);
}

std::tuple<double, double, double> Image::get_RGB(size_t i, size_t j){
    double b = mat_image.at<cv::Vec3b>(i,j)[0];
    double g = mat_image.at<cv::Vec3b>(i,j)[1];
    double r = mat_image.at<cv::Vec3b>(i,j)[2]; 
    return std::make_tuple(r,g,b);
}

std::tuple<double, double, double> Image::get_YUV(size_t i, size_t j){
    return YUV_representation[i][j];
}

Image::~Image(){
//    delete[] mat_image;
//    std::cout << "Inside destructor" ;
}