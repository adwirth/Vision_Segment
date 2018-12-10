/*!
 * \file
 * RegionProcess class definition. This file is part of Region module.
 * The RegionProcess class executes the region segmentation and edge detection tasks.
 *
 * \remarks
 *
 * \authors
 * Andras Wirth
 */


#pragma once

#include "Region.h"
#include "RegionUI.h"

class RegionProcess
{
public:

	RegionProcess(double aThreshold, double aThreshold2, double aAlpha, int aMaxDimension, int edgeAverageKernel, bool enableUI = true, bool aSmoothEdge = true, bool aMedian = true);

	std::tuple<cv::Mat, std::vector<std::pair<int, int>>, cv::Mat> RunCore(const cv::Mat& imgIn, const cv::Point location);

	void Run(const cv::Mat& imgIn,
			 const cv::Point location,
			 std::string outputRegionPath,
			 std::string outputPerimeterPath);

private:
	Region m_Region;
	const double m_MaxDimension;			//!< Maximum image dimension
	RegionUI m_RegionUI;
	bool m_SmoothEdge;
	bool m_Median;
	const int m_EdgeAverageKernel;			//!< Kernel size for edge point average filter 
};