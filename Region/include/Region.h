/*!
 * \file
 * Region class definition. This file is part of Region module.
 * Region class performs a region segmentation of the image and calculates its edge.
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
#include "Graph.h"
#include <vector>

 //! \brief Region segmentation class
class Region
{
public:

	//! \brief Constructor
	Region(double aThreshold, double aThreshold2, double aAlpha);

	//! \brief Region segmentation function
	cv::Mat findRegion(const cv::Mat &imgIn, const cv::Point location);

	//! \brief Perimeter detection function
	cv::Mat findPerimeter(const cv::Mat &regionIn, std::vector<std::pair<int, int>>& edgePoints);

	//! \brief Smooth perimeter function
	void smoothPerimeter(std::vector<std::pair<int, int>>& edgePoints);

private:

	// parameters
	const double m_Threshold;				//!< Threshold parameter for region growing
	const double m_Threshold2;				//!< Threshold parameter for region growing
	const double m_Alpha;					//!< Weight parameter for region growing
	const double m_MinRegionAreaFactor;		//!< Minimum area factor for edge detection
	cv::Point m_PointShift2D[8];			//!< Offsets of the neighbours

	//! \brief Region growing function
	void grow(const cv::Mat& src, cv::Mat& dest, cv::Mat& mask, cv::Point seed);

	//! \brief Determines whether coordinates are inside image border
	bool isPixelInImage(const cv::Mat& src, cv::Point pixel);

	//! \brief Angle based distance function
	double distanceAngle(const cv::Vec3b p1, const cv::Vec3b p2);

	//! \brief Euclidean distance function
	double distanceEuc(const cv::Vec3b p1b, const cv::Vec3b p2b);

	//! \brief Average color value inside kernel
	cv::Vec3b avgColor(const cv::Mat &imgIn, const cv::Mat& mask, const cv::Point location, int kernel);

	//! \brief Calculate lightness value of pixel
	double Region::lightness(const cv::Vec3b pb);

	//! \brief Build graph from edge vector
	Graph buildDAG(std::vector<std::pair<int, int>> edgePoints);
};