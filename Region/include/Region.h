#pragma once

#include <stack>
#include <opencv2/opencv.hpp>

class Region
{
public:

    Region();

    cv::Mat findregion(const cv::Mat &imgIn, const cv::Point location);

private:

    void grow(const cv::Mat& src, cv::Mat& dest, cv::Mat& mask, cv::Point seed, int threshold);

    // parameters
    const int threshold;
    const uchar max_region_num;
    const double min_region_area_factor;
    cv::Point PointShift2D[8];
};