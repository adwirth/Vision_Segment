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
#include <vector>

namespace RegionIO
{
	cv::Mat LoadPixels(std::string inputPath);

	void SavePixels(const cv::Mat& image, std::string outputPath);

	void SaveVectorToText(const std::vector<std::pair<int,int>>& edgePoints, std::string outputPath);
	void SaveVectorToImage(const std::vector<std::pair<int, int>>& edgePoints, int cols, int rows, std::string outputPath);
};