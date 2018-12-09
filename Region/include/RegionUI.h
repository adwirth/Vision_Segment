/*!
 * \file
 * RegionUI class definition. This file is part of Region module.
 * The RegionUI class manages general user interaction and display functions.
 *
 * \remarks
 *
 * \authors
 * Andras Wirth
 */

#pragma once

#include <opencv2/opencv.hpp>

class RegionUI
{
public:
	RegionUI(bool enable = true);

	void DisplayImage(cv::Mat image, std::string windowName);

	cv::Point DisplayImageSelectPixel(cv::Mat image, std::string windowName);

private:
	bool m_Enabled;
};