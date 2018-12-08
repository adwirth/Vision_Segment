#pragma once

#include <opencv2/opencv.hpp>

namespace RegionIO
{
    cv::Mat LoadPixels(std::string inputPath);

    void SavePixels(const cv::Mat& image, std::string outputPath);
};