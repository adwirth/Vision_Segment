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

RegionProcess::RegionProcess():
    m_Region()
{
    ;
}

void RegionProcess::Run( const cv::Mat& imgIn,
                         const cv::Point location,
                         std::string outputRegionPath,
                         std::string outputPerimeterPath)
{
    cv::Point point;
    if ( location.x == -1 && 
         0 <= location.x && location.x < imgIn.cols &&
         0 <= location.y && location.y < imgIn.rows )
    {
        point.x = location.x;
        point.y = location.y;
        RegionUI::DisplayImage(imgIn,"Input");
    }
    else
    {
        point = RegionUI::DisplayImageSelectPixel(imgIn,"InputSelector");
    }

    if (point.x != -1)
    {
        cv::Mat regionImg = m_Region.FindRegion(imgIn, point);

        RegionUI::DisplayImage(regionImg, "Region");

        if (!outputRegionPath.empty())
            RegionIO::SavePixels(regionImg, outputRegionPath);

        cv::Mat perimeterImage = m_Region.FindPerimeter(regionImg);

        RegionUI::DisplayImage(perimeterImage, "Perimeter");
        
        if( !outputPerimeterPath.empty() )
            RegionIO::SavePixels(perimeterImage, outputPerimeterPath);
    }
}