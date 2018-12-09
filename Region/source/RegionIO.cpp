/*!
 * \file
 * Member function definitions of RegionIO class. This file is part of Region module.
  *
 * \remarks
 *
 * \authors
 * Andras Wirth
 */

#include "RegionIO.h"

cv::Mat RegionIO::LoadPixels(std::string inputPath)
{
	cv::Mat image;
	image = cv::imread(inputPath, 1);

	// Image load check
	if (!image.data)
	{
		std::string errorMSG( inputPath + " file corrupt.\n No image data (warning: OpenCV recognize files by extensions)\n");
		throw std::exception(errorMSG.c_str());
	}
	return image;
}

void RegionIO::SavePixels(const cv::Mat& image, std::string outputPath)
{
	if (!image.data || image.empty())
	{
		throw std::exception("No image data\n");
	}
	imwrite(outputPath, image);
}

void RegionIO::SaveVectorToText(const std::vector<std::pair<int, int>>& edgePoints, std::string outputPath)
{
}

void RegionIO::SaveVectorToImage(const std::vector<std::pair<int, int>>& edgePoints, int cols, int rows, std::string outputPath)
{
	cv::Mat image(rows, cols, CV_8UC3, cv::Scalar(0, 0, 0));
	for (auto it = edgePoints.begin(); it != edgePoints.end(); ++it)
	{
		image.at<cv::Vec3b>(cv::Point((*it).first, (*it).second)) = cv::Vec3b(255,255,255);
	}
	imwrite(outputPath, image);
}
