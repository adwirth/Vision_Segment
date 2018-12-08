#pragma once

#include "Region.h"

class RegionProcess
{
public:

    RegionProcess();

    void Run(const cv::Mat& imgIn,
             const cv::Point location,
             std::string outputRegionPath,
             std::string outputPerimeterPath);

private:
    Region m_Region;
};