#include "cmdparser.hpp"
#include "Region.h"

#include <stdio.h>
#include <opencv2/opencv.hpp>

void configure_parser(cli::Parser& parser) 
{
    parser.set_required<std::string>("i", "output", "data", "Strings are naturally included.");
    parser.set_required<std::string>("o", "output", "data", "Strings are naturally included.");
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
    auto outputPath = parser.get<std::string>("o");

    cv::Mat image;
    image = cv::imread(inputPath, 1);

    // Image load
    if (!image.data)
    {
        printf("No image data (warning: OpenCV recognize files by extensions)\n");
        return -1;
    }

    Region region(image);

    cv::Point point = region.DisplayImageSelectPixel();

    region.FindRegion(point);

    region.DisplayPixels(Region::ImageType::RegionImage);

    region.SavePixels(Region::ImageType::RegionImage,outputPath);

    return 0;
}

