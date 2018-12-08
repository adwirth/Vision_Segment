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
	m_Region(aThreshold, aThreshold2, aAlpha, aMaxDimension)
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
		if (imgIn.cols > 300 || imgIn.rows > 300)
		{
			scale = 300. / imgIn.cols;
			cv::resize(imgIn, imgIn, cv::Size(0, 0), scale, scale); // resize for speed
			point = cv::Point((int)(pointO.x * scale), (int)(pointO.y * scale));
		}

		cv::Mat regionImg = m_Region.FindRegion(imgIn, point);
		cv::Mat perimeterImage = m_Region.FindPerimeter(regionImg);

		if (scale < 1)
		{
			double scaleB = 1. / scale;
			cv::resize(regionImg, regionImg, cv::Size(0, 0), scaleB, scaleB);
			cv::resize(perimeterImage, perimeterImage, cv::Size(0, 0), scaleB, scaleB);
		}
		if (!outputRegionPath.empty())
			RegionIO::SavePixels(regionImg, outputRegionPath);

		if (!outputPerimeterPath.empty())
			RegionIO::SavePixels(perimeterImage, outputPerimeterPath);

		RegionUI::DisplayImage(regionImg, "Region");
		RegionUI::DisplayImage(perimeterImage, "Perimeter");
	}
}