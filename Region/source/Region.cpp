#include "Region.h"

Region::Region():
    threshold(200),
    max_region_num(100),
    min_region_area_factor(0.01)
{
    PointShift2D[0] = cv::Point(1, 0);
    PointShift2D[1] = cv::Point(1, -1);
    PointShift2D[2] = cv::Point(0, -1);
    PointShift2D[3] = cv::Point(-1, -1);
    PointShift2D[4] = cv::Point(-1, 0);
    PointShift2D[5] = cv::Point(-1, 1);
    PointShift2D[6] = cv::Point(0, 1);
    PointShift2D[7] = cv::Point(1, 1);
}

void Region::grow(const cv::Mat& src, cv::Mat& dest, cv::Mat& mask, cv::Point seed, int threshold)
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

        for (int i = 0; i<8; ++i) {
            cv::Point estimating_point = center + PointShift2D[i];
            if (estimating_point.x < 0
                || estimating_point.x > src.cols - 1
                || estimating_point.y < 0
                || estimating_point.y > src.rows - 1) {
                // estimating_point should not out of the range in image
                continue;
            }
            else {
                //                uchar delta = (uchar)abs(src.at<uchar>(center) - src.at<uchar>(estimating_point));
                // delta = (R-R')^2 + (G-G')^2 + (B-B')^2
                int delta = int(pow(src.at<cv::Vec3b>(center)[0] - src.at<cv::Vec3b>(estimating_point)[0], 2)
                    + pow(src.at<cv::Vec3b>(center)[1] - src.at<cv::Vec3b>(estimating_point)[1], 2)
                    + pow(src.at<cv::Vec3b>(center)[2] - src.at<cv::Vec3b>(estimating_point)[2], 2));
                if (dest.at<uchar>(estimating_point) == 0
                    && mask.at<uchar>(estimating_point) == 0
                    && delta < threshold) {
                    mask.at<uchar>(estimating_point) = 1;
                    point_stack.push(estimating_point);
                }
            }
        }
    }
}

cv::Mat Region::findregion(const cv::Mat &imgIn, const cv::Point location)
{
    int min_region_area = int(min_region_area_factor * imgIn.cols * imgIn.rows);

    uchar padding = 255;  // use which number to pad in "dest"
    cv::Mat dest = cv::Mat::zeros(imgIn.rows, imgIn.cols, CV_8UC1);

    // "mask" records current region, always use "1" for padding
    cv::Mat mask = cv::Mat::zeros(imgIn.rows, imgIn.cols, CV_8UC1);

    if (dest.at<uchar>(location) == 0)
    {
        grow(imgIn, dest, mask, location, threshold);
    
        int mask_area = (int)cv::sum(mask).val[0];  // calculate area of the region that we get in "seed grow"
        if (mask_area > min_region_area)
        {
            dest = dest + mask * padding;   // record new region to "dest"
        }
        else
        {
            dest = dest + mask * 255;   // record as "ignored"
        }
        mask = mask - mask;     // zero mask, ready for next "seed grow"
    }

    return dest;
}
