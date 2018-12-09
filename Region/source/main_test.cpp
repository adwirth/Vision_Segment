/*!
* \file
* Test Main function of the Region module.
*
* \remarks
*
* \authors
* Andras Wirth
*/

#include "cmdparser.hpp"
#include "RegionProcess.h"
#include "RegionIO.h"

#include <opencv2/opencv.hpp>
#include <chrono>

#define ASSERT_THROW( condition )									\
{																	\
	if( !( condition ) )											\
	{																\
		throw std::runtime_error( std::string( __FILE__ )			\
								+ std::string( ":" )				\
								+ std::to_string( __LINE__ )		\
								+ std::string( " in " )				\
								+ std::string( __PRETTY_FUNCTION__ )\
	);																\
	}																\
}

#define ASSERT_EQUAL( x, y )										\
{																	\
	if( ( x ) != ( y ) )											\
	{																\
		throw std::runtime_error( std::string( __FILE__ )			\
								+ std::string( ":" )				\
								+ std::to_string( __LINE__ )		\
								+ std::string( " in " )				\
								+ std::string( __PRETTY_FUNCTION__ )\
								+ std::string( ": " )				\
								+ std::to_string( ( x ) )			\
								+ std::string( " != " )				\
								+ std::to_string( ( y ) )			\
	);																\
	}																\
}

std::tuple<double,double,double,int> defaultParameters()
{
	return std::make_tuple(0.0008, 0.025, 0.5, 100);
}

std::string imageBaseFolder()
{
	return std::string("../../Images/");
}

std::string testResultsBaseFolder()
{
	return std::string("../../Tests/");
}

void defaultTest( std::string testName, std::string inputImagePath, std::string regionImagePath, std::string perimeterImagePath)
{
	std::cout << testName << " running" << std::endl;
	auto start = std::chrono::high_resolution_clock::now();

	try
	{
		cv::Mat image;
		try
		{
			image = RegionIO::LoadPixels(imageBaseFolder() + inputImagePath);
		}
		catch (std::exception e)
		{
			std::cout << "Unable to load image" << "\n" << e.what();
			exit(1);
		}

		double threshold1, threshold2, alpha;
		int maxdimension;

		std::tie(threshold1, threshold2, alpha, maxdimension) = defaultParameters();

		RegionProcess region(threshold1, threshold2, alpha, maxdimension, false);

		cv::Point point(25,25);

		region.Run(image, point, testResultsBaseFolder() + regionImagePath, testResultsBaseFolder() + perimeterImagePath);
	}
	catch (std::exception e)
	{
		std::cout << "Test failed." << std::endl << e.what() << std::endl;
		return;
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Test passed " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms." << std::endl;
	return;
}

void defaultTest1()
{
	defaultTest(std::string(__FUNCTION__), "test1.png", "region1.png", "perimeter1.png");
}

void defaultTest2()
{
	defaultTest(std::string(__FUNCTION__), "test2.png", "region2.png", "perimeter2.png");
}

void defaultTest3()
{
	defaultTest(std::string(__FUNCTION__), "test3.png", "region3.png", "perimeter3.png");
}

void defaultTest4()
{
	defaultTest(std::string(__FUNCTION__), "test4.jpg", "region4.jpg", "perimeter4.jpg");
}


void defaultTest5()
{
	defaultTest(std::string(__FUNCTION__), "yesthisfileiscorrupt.png", "corrupt.jpg", "corrupt.jpg");
}

int main_test(int argc, char *argv[])
{
	defaultTest1();
	defaultTest2();
	defaultTest3();
	defaultTest4();
	defaultTest5();

	return 0;
}

