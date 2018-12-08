#include "cmdparser.hpp"
#include "RegionProcess.h"
#include "RegionIO.h"

#include <stdio.h>
#include <opencv2/opencv.hpp>

void configure_parser(cli::Parser& parser) 
{
    parser.set_required<std::string>("i", "output", "", "Strings are naturally included.");
    parser.set_required<std::string>("r", "output", "", "Strings are naturally included.");
    parser.set_optional<std::string>("p", "output", "", "Strings are naturally included.");
    parser.set_optional<std::vector<int>>("l", "values", std::vector<int>{}, "The input pixel for region searching");
    //parser.set_optional<int>("n", "number", 8, "Integers in all forms, e.g., unsigned int, long long, ..., are possible. Hexadecimal and Ocatl numbers parsed as well");
    //parser.set_optional<cli::NumericalBase<int, 10>>("t", "temp", 0, "integer parsing restricted only to numerical base 10");
    //parser.set_optional<double>("b", "beta", 11.0, "Also floating point values are possible.");
    //parser.set_optional<bool>("a", "all", false, "Boolean arguments are simply switched when encountered, i.e. false to true if provided.");
    //parser.set_required<std::vector<short>>("v", "values", "By using a vector it is possible to receive a multitude of inputs.");
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

    cv::Mat image;
    image = RegionIO::LoadPixels(inputPath);

    RegionProcess region;

    cv::Point point;
    if (location.size() == 2)
    {
        point.x = location[0];
        point.y = location[1];
    }

    region.Run(image, point, outputRegionPath, outputPerimeterPath);

    return 0;
}

