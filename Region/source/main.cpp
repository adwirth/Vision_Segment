/*!
 * \file
 * Main function of the Region module.
 *
 * \remarks
 *
 * \authors
 * Andras Wirth
 */

#include "cmdparser.hpp"
#include "RegionProcess.h"
#include "RegionIO.h"

#include <stdio.h>
#include <opencv2/opencv.hpp>

void configure_parser(cli::Parser& parser) 
{
	parser.set_required<std::string>("i", "input", "", "Path");
	parser.set_optional<std::string>("r", "region", "", "Path");
	parser.set_optional<std::string>("p", "perimeter", "", "Path");
	parser.set_optional<bool>("ui", "uiswitch", true, "Enable user interface.");
	parser.set_optional<std::vector<int>>("l", "values", std::vector<int>{}, "The input pixel for region searching");
	parser.set_optional<double>("t1", "threshold1", 0.0008, "Threshold1");
	parser.set_optional<double>("t2", "threshold2", 0.025, "Threshold2");
	parser.set_optional<double>("al", "alpha", 0.5, "Alpha");
	parser.set_optional<int>("dim", "maxdimension", 100, "Downscale image to this dimension");
}

int main(int argc, char *argv[])
{
	cli::Parser parser(argc, argv);
	configure_parser(parser);
	parser.run_and_exit_if_error();


	auto inputPath  = parser.get<std::string>("i");
	auto outputRegionPath = parser.get<std::string>("r");
	auto outputPerimeterPath = parser.get<std::string>("p");
	std::vector<int> location = parser.get<std::vector<int>>("l");
	bool uiEnable = parser.get<bool>("ui");
	double threshold1 = parser.get<double>("t1");
	double threshold2 = parser.get<double>("t2");
	double alpha = parser.get<double>("al");
	int maxdimension = parser.get<int>("dim");
	

	cv::Mat image;
	try
	{
		image = RegionIO::LoadPixels(inputPath);
	}
	catch (std::exception e)
	{
		std::cout << "Unable to load image" << "\n" << e.what();
		exit(1);
	}
	
	RegionProcess region(threshold1, threshold2, alpha, maxdimension, uiEnable);

	cv::Point point;
	if (location.size() == 2)
	{
		point.x = location[0];
		point.y = location[1];
	}

	region.Run(image, point, outputRegionPath, outputPerimeterPath);

	return 0;
}

