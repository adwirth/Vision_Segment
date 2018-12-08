#pragma once

#include <stack>
#include <opencv2/opencv.hpp>

class Region
{
public:

    enum ImageType 
    {
        RegionImage,
        PerimeterImage
    };

    Region(const cv::Mat &imgIn);

    void UpdateInputImage(const cv::Mat &imgIn);

    void FindRegion(const cv::Point location);

    void DisplayImage();

    cv::Point DisplayImageSelectPixel();
    
    void DisplayPixels(ImageType type);

    void SavePixels(ImageType type, std::string outputPath);

private:

    void Grow(const cv::Mat& src, cv::Mat& dest, cv::Mat& mask, cv::Point seed, int threshold);

    // parameters
    const int m_Threshold;
    // minimum area factor
    const double m_MinRegionAreaFactor;
    // offsets of the neighbours
    cv::Point m_PointShift2D[8];

    // The input image 
    cv::Mat m_SourceImg;
    // The region image 
    cv::Mat m_RegionImg;
    // The perimeter image 
    cv::Mat m_PerimeterImg;

	double Region::distanceAngle(const cv::Vec3b p1, const cv::Vec3b p2);

};