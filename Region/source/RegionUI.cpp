/*!
 * \file
 * Member function definitions of RegionUI class. This file is part of Region module.
 *
 * \remarks
 *
 * \authors
 * Andras Wirth
 */

#include "RegionUI.h"

void RegionUI::DisplayImage(cv::Mat image, std::string windowName)
{
	cv::namedWindow(windowName, cv::WINDOW_NORMAL);
	cv::resizeWindow(windowName, image.cols * 512 / image.rows, 512);
	cv::imshow(windowName, image);
	cv::waitKey(0);
}

void onMouse(int evt, int x, int y, int flags, void* param)
{
	if (evt == CV_EVENT_LBUTTONDOWN)
	{
		cv::Point* ptPtr = (cv::Point*)param;
		ptPtr->x = x;
		ptPtr->y = y;

		cv::destroyAllWindows();
	}
}

cv::Point RegionUI::DisplayImageSelectPixel(cv::Mat image, std::string windowName)
{
	cv::Point point(-1, -1);

	cv::namedWindow(windowName, cv::WINDOW_NORMAL);
	cv::resizeWindow(windowName, image.cols * 512 / image.rows, 512);
	cv::imshow(windowName, image);
	cv::setMouseCallback(windowName, onMouse, (void*)&point);
	cv::waitKey(0);

	return point;
}
