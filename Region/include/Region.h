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

    Region( double aThreshold, double aThreshold2, double aAlpha, int aMaxDimension);

    cv::Mat FindRegion(const cv::Mat &imgIn, const cv::Point location);

    cv::Mat FindPerimeter(const cv::Mat &regionIn);

private:    

    // parameters
    const double m_Threshold;
	const double m_Threshold2;
	const double m_Alpha;
	const double m_MaxDimension;
    // minimum area factor
    const double m_MinRegionAreaFactor;
    // offsets of the neighbours
    cv::Point m_PointShift2D[8];

	void Grow(const cv::Mat& src, cv::Mat& dest, cv::Mat& mask, cv::Point seed);
	double distanceAngle(const cv::Vec3b p1, const cv::Vec3b p2);	
	double distanceEuc(const cv::Vec3b p1b, const cv::Vec3b p2b);
	cv::Vec3b avgColor(const cv::Mat &imgIn, const cv::Mat& mask, const cv::Point location, int kernel);
	double Region::lightness(const cv::Vec3b pb);
};