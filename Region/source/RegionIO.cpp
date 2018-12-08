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
    if( !image.data || image.empty() )
    {
        throw std::exception("No image data\n");
    }
    imwrite(outputPath, image);
}
