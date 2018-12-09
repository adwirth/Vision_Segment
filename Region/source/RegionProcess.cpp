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

RegionProcess::RegionProcess(double aThreshold, double aThreshold2, double aAlpha, int aMaxDimension, bool enableUI):
	m_Region(aThreshold, aThreshold2, aAlpha),
	m_MaxDimension(aMaxDimension),
	m_RegionUI(enableUI)
{
}

std::tuple<cv::Mat, std::vector<std::pair<int, int>>, cv::Mat> RegionProcess::RunCore(const cv::Mat& imgInO, const cv::Point location)
{
	cv::Mat imgIn;
	imgInO.copyTo( imgIn );

	cv::Point pointO;
	if (0 <= location.x && location.x < imgIn.cols &&
		0 <= location.y && location.y < imgIn.rows)
	{
		pointO.x = location.x;
		pointO.y = location.y;
		m_RegionUI.DisplayImage(imgIn, "Input");
	}
	else
	{
		pointO = m_RegionUI.DisplayImageSelectPixel(imgIn, "InputSelector");
	}

	if( pointO.x == -1 )
	{
		throw std::exception("The selected index is invalid. Please enable UI or choose a correct pixel");
	}

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

	// Resizing back to original resolution
	if (scale < 1)
	{
		double scaleB = 1. / scale;
		cv::resize(regionImg, regionImg, cv::Size(0, 0), scaleB, scaleB);
		cv::resize(perimeterImage, perimeterImage, cv::Size(0, 0), scaleB, scaleB);
	}

	// Displaying results
	m_RegionUI.DisplayImage(regionImg, "Region");
	m_RegionUI.DisplayImage(perimeterImage, "Perimeter");

	return std::make_tuple(regionImg, edgePoints, perimeterImage);
}

void RegionProcess::Run( const cv::Mat& imgInO,
						 const cv::Point location,
						 std::string outputRegionPath,
						 std::string outputPerimeterPath)
{
	cv::Mat regionImg;
	cv::Mat perimeterImage;
	// Edge detection
	std::vector<std::pair<int, int>> edgePoints;

	std::tie(regionImg, edgePoints, perimeterImage) = RunCore(imgInO, location);

	if (!outputRegionPath.empty())
		RegionIO::SavePixels(regionImg, outputRegionPath);

	if (!outputPerimeterPath.empty())
		RegionIO::SavePixels(perimeterImage, outputPerimeterPath);

	if (!outputPerimeterPath.empty())
		RegionIO::SaveVectorToImage(edgePoints, perimeterImage.cols, perimeterImage.rows, outputPerimeterPath);

}