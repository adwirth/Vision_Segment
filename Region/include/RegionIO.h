/*!
 * \file
 * RegionIO class definition. This file is part of Region module.
 * The RegionIO class performs data IO tasks.
 *
 * \remarks
 *
 * \authors
 * Andras Wirth
 */

#pragma once

#include <opencv2/opencv.hpp>

namespace RegionIO
{
    cv::Mat LoadPixels(std::string inputPath);

    void SavePixels(const cv::Mat& image, std::string outputPath);
};