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

class RegionProcess
{
public:

	RegionProcess(double aThreshold, double aThreshold2, double aAlpha, int aMaxDimension);

    void Run(const cv::Mat& imgIn,
             const cv::Point location,
             std::string outputRegionPath,
             std::string outputPerimeterPath);

private:
    Region m_Region;
};