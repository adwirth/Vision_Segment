#include "Region.h"

Region::Region(const cv::Mat &imgIn):
    m_SourceImg(imgIn),
    m_Threshold(200),
    m_MinRegionAreaFactor(0.01)
{
    m_PointShift2D[0] = cv::Point(1, 0);
    m_PointShift2D[1] = cv::Point(1, -1);
    m_PointShift2D[2] = cv::Point(0, -1);
    m_PointShift2D[3] = cv::Point(-1, -1);
    m_PointShift2D[4] = cv::Point(-1, 0);
    m_PointShift2D[5] = cv::Point(-1, 1);
    m_PointShift2D[6] = cv::Point(0, 1);
    m_PointShift2D[7] = cv::Point(1, 1);
}

void Region::Grow(const cv::Mat& src, cv::Mat& dest, cv::Mat& mask, cv::Point seed, int threshold)
{
    /* apply "seed grow" in a given seed
    * Params:
    *   src: source image
    *   dest: a matrix records which pixels are determined/undtermined/ignored
    *   mask: a matrix records the region found in current "seed grow"
    */
    std::stack<cv::Point> point_stack;
    point_stack.push(seed);

    while (!point_stack.empty())
    {
        cv::Point center = point_stack.top();
        mask.at<uchar>(center) = 1;
        point_stack.pop();

        for (int i = 0; i<8; ++i)
        {
            cv::Point estimating_point = center + m_PointShift2D[i];
            if (estimating_point.x < 0
                || estimating_point.x > src.cols - 1
                || estimating_point.y < 0
                || estimating_point.y > src.rows - 1)
            {
                // estimating_point should not out of the range in image
                continue;
            }
            else {
				cv::Vec3b p1 = src.at<cv::Vec3b>(center);
				cv::Vec3b p2 = src.at<cv::Vec3b>(estimating_point);

				double delta = distanceAngle(p1, p2);
				double thr = 0.0004;
				if (dest.at<uchar>(estimating_point) == 0
					&& mask.at<uchar>(estimating_point) == 0
					&& delta < thr) {
					mask.at<uchar>(estimating_point) = 1;
					point_stack.push(estimating_point);

               }
            }
        }
    }
}

void Region::UpdateInputImage(const cv::Mat &imgIn)
{
    m_SourceImg = imgIn;
    m_RegionImg.deallocate();
    m_PerimeterImg.deallocate();
}

void Region::FindRegion(const cv::Point location)
{
    int minRegionArea = int( m_MinRegionAreaFactor * m_SourceImg.cols * m_SourceImg.rows);

    uchar padding = 255;  // use which number to pad in "dest"
    m_RegionImg = cv::Mat::zeros(m_SourceImg.rows, m_SourceImg.cols, CV_8UC1);

    // "mask" records current region, always use "1" for padding
    cv::Mat mask = cv::Mat::zeros(m_SourceImg.rows, m_SourceImg.cols, CV_8UC1);

    if (m_RegionImg.at<uchar>(location) == 0)
    {
        Grow(m_SourceImg, m_RegionImg, mask, location, m_Threshold);
    
        int mask_area = (int)cv::sum(mask).val[0];  // calculate area of the region that we get in "seed grow"
        if (mask_area > minRegionArea)
        {
            m_RegionImg = m_RegionImg + mask * padding;   // record new region to "dest"
        }
        else
        {
            m_RegionImg = m_RegionImg + mask * 255;   // record as "ignored"
        }
    }
}

double Region::distanceAngle(const cv::Vec3b p1b, const cv::Vec3b p2b)
{
	cv::Vec3d p1(p1b);
	cv::Vec3d p2(p2b);
	double c = (p1[0] * p2[0] + p1[1] * p2[1] + p1[2] * p2[2]) / (std::sqrt(p1[0] * p1[0] + p1[1] * p1[1] + p1[2] * p1[2]) * std::sqrt(p2[0] * p2[0] + p2[1] * p2[1] + p2[2] * p2[2]));
	return 1. - c * c;

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

cv::Point Region::DisplayImageSelectPixel()
{
    cv::Point point;
    std::string win_name("InputSelector");
    cv::namedWindow(win_name, cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback(win_name, onMouse, (void*)&point);
    imshow(win_name, m_SourceImg);

    cv::waitKey(0);

    return point;
}

void Region::DisplayImage()
{
    std::string win_name("Input");
    cv::namedWindow(win_name, cv::WINDOW_AUTOSIZE);
    imshow(win_name, m_SourceImg);

    cv::waitKey(0);
}

void Region::DisplayPixels(ImageType type)
{
    switch (type)
    {
    case ImageType::RegionImage:
        imshow("RegionImage", m_RegionImg);
        break;
    case ImageType::PerimeterImage:
        imshow("PerimeterImage", m_PerimeterImg);
        break;
    }
    cv::waitKey(0);
}

void Region::SavePixels(ImageType type, std::string outputPath)
{
    switch (type)
    {
    case ImageType::RegionImage:
        imwrite(outputPath, m_RegionImg);
        break;
    case ImageType::PerimeterImage:
        imwrite(outputPath, m_PerimeterImg);
        break;
    }
}
