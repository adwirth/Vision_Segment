#include "Region.h"
#include <stack>

Region::Region():
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
	cv::Vec3d pAverage = src.at<cv::Vec3b>(seed);
	cv::Vec3d pA = pAverage;
	double norm = 1;
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
				double alpha = 0.0005;
				
				double delta2 = distanceAngle(p2, pA);
				double thr2 = 0.0005;

				double delta3 = distanceEuc(p1, p2);
				double thr3 = 0.05;
				if (dest.at<uchar>(estimating_point) == 0
					&& mask.at<uchar>(estimating_point) == 0
					&& ((1- alpha) * delta + alpha * delta2 ) < thr2 
					&& delta3 < thr3 )
				{
					mask.at<uchar>(estimating_point) = 1;
					point_stack.push(estimating_point);
					pAverage += cv::Vec3d(p2);
					int pS = point_stack.size() + 1;
					norm += 1. / sqrt((seed.x - estimating_point.x) * (seed.x - estimating_point.x) + (seed.y - estimating_point.y) * (seed.y - estimating_point.y));
					pA = cv::Vec3d(pAverage[0] / norm, pAverage[1] / norm, pAverage[2] / norm);

               }
            }
        }
    }
}

cv::Mat Region::FindRegion(const cv::Mat &imgIn, const cv::Point location)
{
	int minRegionArea = int(m_MinRegionAreaFactor * imgIn.cols * imgIn.rows);

	uchar padding = 255;  // use which number to pad in "dest"
	cv::Mat regionImg = cv::Mat::zeros(imgIn.rows, imgIn.cols, CV_8UC1);

	// "mask" records current region, always use "1" for padding
	cv::Mat mask = cv::Mat::zeros(imgIn.rows, imgIn.cols, CV_8UC1);

	if (regionImg.at<uchar>(location) == 0)
	{
		Grow(imgIn, regionImg, mask, location, m_Threshold);

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

cv::Mat Region::FindPerimeter(const cv::Mat &regionIn)
{
	cv::Mat perimeterImg;
	regionIn.copyTo(perimeterImg);

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
					cv::Point estimating_point = center + m_PointShift2D[i];
					if (estimating_point.x < 0
						|| estimating_point.x > regionIn.cols - 1
						|| estimating_point.y < 0
						|| estimating_point.y > regionIn.rows - 1)
					{
						edge = true;
						i = 8;
					}
					else
					{
						if (centerValue != regionIn.at<uchar>(estimating_point))
						{
							edge = true;
							i = 8;
						}
					}
				}
				if (!edge)
					perimeterImg.at<uchar>(center) = 0;
			}
		}
	}
	return perimeterImg;
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
	cv::Vec3d p1(p1b);
	cv::Vec3d p2(p2b);

	double mi = std::min(p1[0], std::min(p1[1], p1[2]));
	double ma = std::max(p1[0], std::max(p1[1], p1[2]));
	double L1 = (ma + mi) / 2;

	mi = std::min(p2[0], std::min(p2[1], p2[2]));
	ma = std::max(p2[0], std::max(p2[1], p2[2]));
	double L2 = (ma + mi) / 2;
	return abs(L2 - L1) / 255.0f;
}
