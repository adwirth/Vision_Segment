/*!
 * \file
 * Region class definition. This file is part of Region module.
 * Region class performs a region of the image and calculates its edge.
 *
 * \remarks
 * Original region grow algorithm by LogM.
 * https://github.com/imLogM/multi_seed_region_grow
 *
 * \authors
 * Andras Wirth
 */

#pragma once

#include <opencv2/opencv.hpp>

class Region
{
public:

    Region();


    cv::Mat FindRegion(const cv::Mat &imgIn, const cv::Point location);

    cv::Mat FindPerimeter(const cv::Mat &regionIn);

private:

    void Grow(const cv::Mat& src, cv::Mat& dest, cv::Mat& mask, cv::Point seed, int threshold);

    // parameters
    const int m_Threshold;
    // minimum area factor
    const double m_MinRegionAreaFactor;
    // offsets of the neighbours
    cv::Point m_PointShift2D[8];
double distanceAngle(const cv::Vec3b p1, const cv::Vec3b p2);
	cv::Point nearestNeighbor(const cv::Mat& img, const cv::Point p);
	double distanceEuc(const cv::Vec3b p1b, const cv::Vec3b p2b);
};