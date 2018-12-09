/*!
 * \file
 * Member function definitions of RegionProcess class. This file is part of Region module.
 *
 * \remarks
 *
 * \authors
 * Andras Wirth
 */

#include "RegionProcess.h"
#include "RegionUI.h"
#include "RegionIO.h"

RegionProcess::RegionProcess(double aThreshold, double aThreshold2, double aAlpha, int aMaxDimension):
	m_Region(aThreshold, aThreshold2, aAlpha),
	m_MaxDimension(aMaxDimension)
{
	;
}

void RegionProcess::Run( const cv::Mat& imgInO,
						 const cv::Point location,
						 std::string outputRegionPath,
						 std::string outputPerimeterPath)
{
	cv::Mat imgIn = imgInO;
	cv::Point pointO;

	if ( location.x == -1 && 
		 0 <= location.x && location.x < imgIn.cols &&
		 0 <= location.y && location.y < imgIn.rows )
	{
		pointO.x = location.x;
		pointO.y = location.y;
		RegionUI::DisplayImage(imgIn,"Input");
	}
	else
	{
		pointO = RegionUI::DisplayImageSelectPixel(imgIn,"InputSelector");
	}

	if (pointO.x != -1)
	{
		cv::Point point = pointO;
		double scale = 1;

		// Downsample for speed
		if (imgIn.cols > m_MaxDimension)
		{
			scale = 300. / imgIn.cols;			
			cv::resize(imgIn, imgIn, cv::Size(0, 0), scale, scale);
			point = cv::Point((int)(pointO.x * scale), (int)(pointO.y * scale));
		}
		
		// Region detection
		cv::Mat regionImg = m_Region.findRegion(imgIn, point);

		// Edge detection
		std::vector<std::pair<int, int>> edgePoints;
		cv::Mat perimeterImage = m_Region.findPerimeter(regionImg, edgePoints);

		// Smooth perimeter
		m_Region.smoothPerimeter(edgePoints);

		if (!outputRegionPath.empty())
			RegionIO::SaveVectorToImage(edgePoints, regionImg.cols, regionImg.rows, outputRegionPath);

		// Resizing back to original resolution
		if (scale < 1)
		{
			double scaleB = 1. / scale;
			cv::resize(regionImg, regionImg, cv::Size(0, 0), scaleB, scaleB);
			cv::resize(perimeterImage, perimeterImage, cv::Size(0, 0), scaleB, scaleB);
		}

		// Writing output

		if (!outputRegionPath.empty())
			RegionIO::SavePixels(regionImg, outputRegionPath);

		if (!outputPerimeterPath.empty())
			RegionIO::SavePixels(perimeterImage, outputPerimeterPath);

		// Displaying results
		RegionUI::DisplayImage(regionImg, "Region");
		RegionUI::DisplayImage(perimeterImage, "Perimeter");
	}
}