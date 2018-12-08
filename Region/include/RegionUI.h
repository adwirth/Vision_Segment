#pragma once

#include <opencv2/opencv.hpp>

namespace RegionUI
{
    void DisplayImage(cv::Mat image, std::string windowName);

    cv::Point DisplayImageSelectPixel(cv::Mat image, std::string windowName);
};