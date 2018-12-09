/*!
 * \file
 * Member function definitions of Region class. This file is part of Region module.
 *
 * \remarks
 * Original region grow algorithm by LogM.
 * https://github.com/imLogM/multi_seed_region_grow
 *
 * \authors
 * Andras Wirth
 */

#include "Region.h"
#include "Bezier.h"
#include <stack>

Region::Region( double aThreshold, double aThreshold2, double aAlpha ):
	m_Threshold(aThreshold),
	m_Threshold2(aThreshold2),
	m_Alpha(aAlpha),
	m_MinRegionAreaFactor(0.01)
{
	// Neighboring pixel coordinates
	m_PointShift2D[0] = cv::Point(1, 0);
	m_PointShift2D[1] = cv::Point(1, -1);
	m_PointShift2D[2] = cv::Point(0, -1);
	m_PointShift2D[3] = cv::Point(-1, -1);
	m_PointShift2D[4] = cv::Point(-1, 0);
	m_PointShift2D[5] = cv::Point(-1, 1);
	m_PointShift2D[6] = cv::Point(0, 1);
	m_PointShift2D[7] = cv::Point(1, 1);
}

bool Region::isPixelInImage(const cv::Mat& src, cv::Point pixel)
{
	return 0 <= pixel.x && pixel.x < src.cols && 0 <= pixel.y && pixel.y < src.rows;
}

void Region::grow(const cv::Mat& src, cv::Mat& dest, cv::Mat& mask, cv::Point seed)
{
	std::stack<cv::Point> point_stack;
	point_stack.push(seed);
	double norm = 1;

	// Repeating while new pixels added to region
	while (!point_stack.empty())
	{
		cv::Point center = point_stack.top();
		mask.at<uchar>(center) = 1;
		point_stack.pop();

		// Iterating through neighboring pixels
		for (int i = 0; i<8; ++i)
		{
			cv::Point estimatingPoint = center + m_PointShift2D[i];

			// Check if inside the image
			if ( !isPixelInImage( src, estimatingPoint) )
			{
				// estimating_point should not out of the range in image
				continue;
			}
			else 
			{
				// Getting pixel values
				cv::Vec3b p1 = src.at<cv::Vec3b>(center);
				cv::Vec3b p2 = src.at<cv::Vec3b>(estimatingPoint);
				cv::Vec3b pavg = avgColor(src, mask, center, 15);

				// Calculating distance measures
				double delta = distanceAngle(p1, p2);			
				double delta2 = distanceAngle(pavg, p2);
				double delta3 = distanceEuc(p1, p2);
				double l = 1 - pow(abs( lightness(p1) - 0.5 ) * 2, 2);

				// Adding matching pixels to mask and stack
				if (dest.at<uchar>(estimatingPoint) == 0
					&& mask.at<uchar>(estimatingPoint) == 0
					&& (l * ((1- m_Alpha) * delta + m_Alpha * delta2 - m_Threshold)  + (1 - l) * ( delta3 - m_Threshold2) ) < 0. )
				{
					mask.at<uchar>(estimatingPoint) = 1;
					point_stack.push(estimatingPoint);
				}
			}
		}
	}	
}

cv::Vec3b Region::avgColor(const cv::Mat &imgIn, const cv::Mat& mask, const cv::Point location, int kernel)
{
	int limit = (kernel - 1) / 2;
	double norm = 0;
	cv::Vec3d pAverage = 0;

	// iterationg through the kernel
	for (int i = -limit; i <= limit; ++i)
	{
		for (int j = -limit; j <= limit; ++j)
		{
			cv::Point estimatingPoint = location + cv::Point(i, j);

			// Check if inside the image
			if ( isPixelInImage(imgIn, estimatingPoint) && (mask.at<uchar>(estimatingPoint) == 1) )
			{
				// Weighted summation of pixel values
				double w = 1. / (1. + pow((location.x - estimatingPoint.x) * (location.x - estimatingPoint.x) + (location.y - estimatingPoint.y) * (location.y - estimatingPoint.y),0.3));
				pAverage += cv::Vec3d(imgIn.at<cv::Vec3b>(location)[0] * w, imgIn.at<cv::Vec3b>(location)[1] * w, imgIn.at<cv::Vec3b>(location)[2] * w );
				norm += w;
			}
		}
	}

	// Returning the weighted average of pixel values
	return cv::Vec3d(pAverage[0] / norm, pAverage[1] / norm, pAverage[2] / norm);
}

cv::Mat Region::findRegion(const cv::Mat &imgIn, const cv::Point location)
{
	int minRegionArea = int(m_MinRegionAreaFactor * imgIn.cols * imgIn.rows);

	uchar padding = 255;  // use which number to pad in "dest"
	cv::Mat regionImg = cv::Mat::zeros(imgIn.rows, imgIn.cols, CV_8UC1);

	// "mask" records current region, always use "1" for padding
	cv::Mat mask = cv::Mat::zeros(imgIn.rows, imgIn.cols, CV_8UC1);

	if (regionImg.at<uchar>(location) == 0)
	{
		grow(imgIn, regionImg, mask, location);

		int mask_area = (int)cv::sum(mask).val[0];  // calculate area of the region that we get in "seed grow"
		if (mask_area > minRegionArea)
		{
			regionImg = regionImg + mask * padding;   // record new region to "dest"
		}
		else
		{
			regionImg = regionImg + mask * 255;   // record as "ignored"
		}
	}

	return regionImg;
}

cv::Mat Region::findPerimeter(const cv::Mat &regionIn, std::vector<std::pair<int, int>>& edgePoints)
{
	cv::Mat perimeterImg;
	regionIn.copyTo(perimeterImg);
	edgePoints.clear();

	// Iterating through the image
	for (int rowIndex = 0; rowIndex < regionIn.rows; rowIndex++)
	{
		for (int columnIndex = 0; columnIndex < regionIn.cols; columnIndex++)
		{
			cv::Point center(columnIndex, rowIndex);
			uchar centerValue = regionIn.at<uchar>(center);
			if (centerValue != 0)
			{
				bool edge = false;
				for (int i = 0; i < 8; ++i)
				{
					cv::Point estimatingPoint = center + m_PointShift2D[i];

					// Checking if inside the image
					if ( !isPixelInImage(regionIn, estimatingPoint) )
					{
						edge = true;
						break;
					}
					else
					{
						if (centerValue != regionIn.at<uchar>(estimatingPoint))
						{
							edge = true;
							break;
						}
					}
				}
				if (!edge)
				{
					// If not on edge, setting value to zero
					perimeterImg.at<uchar>(center) = 0;
				}
				else
				{
					// If on edge, value remains 1, and coordinates are added edge vector
					edgePoints.push_back(std::pair<int,int>(columnIndex, rowIndex));
				}
			}
		}
	}

	std::vector<std::pair<int, int>> edgePointsClean;
	for (int c = 0; c < edgePoints.size(); ++c)
	{
		std::pair<int, int> center = edgePoints[c];
		std::vector<std::pair<int, int>> neighbors = neighborPoints(edgePoints, center);
		if (neighbors.size() > 1)
		{
			edgePointsClean.push_back(center);
		}
	}

	std::vector<bool> checkedPoints(edgePoints.size(), false);
	edgePoints.clear();
	
	std::pair<int, int> center = edgePointsClean[0];
	
	std::vector<std::pair<int, int>> prox;
	prox.push_back(std::pair<int, int>(1, 0));
	prox.push_back(std::pair<int, int>(0, 1));
	prox.push_back(std::pair<int, int>(-1, 0));
	prox.push_back(std::pair<int, int>(0, -1));

	bool cont = true;
	while (cont)
	{
		int c = (int)std::distance(edgePointsClean.begin(), std::find(edgePointsClean.begin(), edgePointsClean.end(), center));
		edgePoints.push_back(edgePointsClean[c]);
		checkedPoints[c] = true;
		std::vector<std::pair<int, int>> neighbors = neighborPoints(edgePointsClean, center);

		for (int i = 0; i < 4; ++i)
		{
			std::pair<int, int> current(center.first + prox[i].first, center.second + prox[i].second);
			int p = (int)std::distance(edgePointsClean.begin(), std::find(edgePointsClean.begin(), edgePointsClean.end(), current));
			if (p < edgePointsClean.size() && !checkedPoints[p])
			{
				center = current;
				break;
			}
			if (i == 3)
			{
				cont = false;
			}
		}
	}

	return perimeterImg;
}

void Region::smoothPerimeter(std::vector<std::pair<int, int>>& edgePoints)
{
	Curve* curve = new Bezier();

	for (auto it = edgePoints.begin(); it != edgePoints.end(); ++it)
	{
		curve->add_way_point(Vector(it->first, it->second, 0));
	}

	edgePoints.clear();

	//std::cout << "nodes: " << curve->node_count() << std::endl;
	//std::cout << "total length: " << curve->total_length() << std::endl;
		
	for (int i = 0; i < curve->node_count(); ++i) {
		//std::cout << "node #" << i << ": " << curve->node(i).toString() << " (length so far: " << curve->length_from_starting_point(i) << ")" << std::endl;
		edgePoints.push_back(std::pair<int, int>((int)curve->node(i).x, (int)curve->node(i).y));
	}
	delete curve;
}

double Region::distanceAngle(const cv::Vec3b p1b, const cv::Vec3b p2b)
{
	cv::Vec3d p1(p1b);
	cv::Vec3d p2(p2b);
	double c = (p1[0] * p2[0] + p1[1] * p2[1] + p1[2] * p2[2]) / (std::sqrt(p1[0] * p1[0] + p1[1] * p1[1] + p1[2] * p1[2]) * std::sqrt(p2[0] * p2[0] + p2[1] * p2[1] + p2[2] * p2[2]));
	return 1. - c * c;
}

double Region::distanceEuc(const cv::Vec3b p1b, const cv::Vec3b p2b)
{
	cv::Vec3d p1(p1b[0] / 255., p1b[1] / 255., p1b[2] / 255.);
	cv::Vec3d p2(p2b[0] / 255., p2b[1] / 255., p2b[2] / 255.);
	return std::sqrt( (p1[0] - p2[0]) * (p1[0] - p2[0]) + (p1[1] - p2[1]) * (p1[1] - p2[1]) + (p1[2] - p2[2]) * (p1[2] - p2[2]));
}

double Region::lightness(const cv::Vec3b pb)
{
	cv::Vec3d p1(pb);

	double mi = std::min(p1[0], std::min(p1[1], p1[2]));
	double ma = std::max(p1[0], std::max(p1[1], p1[2]));
	return (ma + mi) / 2. / 255.;
}

std::vector<std::pair<int, int>> Region::neighborPoints(const std::vector<std::pair<int, int>>& edgePoints, std::pair<int, int> center)
{
	std::vector<std::pair<int, int>> neighbors;

	for (auto it = edgePoints.begin(); it != edgePoints.end(); ++it)
	{
		int x = it->first;
		int y = it->second;
		if ((abs(x - center.first) == 1 && abs(y - center.second) == 0) ||
			(abs(x - center.first) == 0 && abs(y - center.second) == 1))
		{
			neighbors.push_back(*it);
		}
	}
	return neighbors;
}
