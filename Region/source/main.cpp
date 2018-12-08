#include "cmdparser.hpp"
#include "Region.h"

#include <stdio.h>
#include <opencv2/opencv.hpp>

void show_mat(const cv::Mat &image, std::string const &win_name);

void show_mat_mouse_click(cv::Point& point, const cv::Mat &image, std::string const &win_name);

cv::Mat &invert_mat(cv::Mat &mat);

cv::Mat& invert_mat_pointer(cv::Mat &mat);

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

int main(int argc, char *argv[])
{
	cli::Parser parser(argc, argv);
	configure_parser(parser);
	parser.run_and_exit_if_error();


	auto inputPath = parser.get<std::string>("i");
	auto outputPath = parser.get<std::string>("o");

	cv::Mat image;
	image = cv::imread(inputPath, 1);

	if (!image.data)
	{
		printf("No image data (warning: OpenCV recognize files by extensions)\n");
		return -1;
	}

	cv::Point pt;
	show_mat_mouse_click(pt, image, "Input");

	cv::Mat output = Region().findregion(image, pt);

	show_mat(output, "Output");

	imwrite(outputPath, output);

	return 0;
}

cv::Mat& invert_mat(cv::Mat &mat) {
	// accept only char type matrices
	CV_Assert(mat.depth() == CV_8U);

	const int channels = mat.channels();
	switch (channels) {
	case 1: {
		//            gray scale image
		cv::MatIterator_<uchar> it, end;
		for (it = mat.begin<uchar>(), end = mat.end<uchar>(); it != end; ++it)
			*it = ~*it;
		break;
	}
	case 3: {
		//            RGB image
		cv::MatIterator_<cv::Vec3b> it, end;
		for (it = mat.begin<cv::Vec3b>(), end = mat.end<cv::Vec3b>(); it != end; ++it) {
			(*it)[0] = ~(*it)[0];
			(*it)[1] = ~(*it)[1];
			(*it)[2] = ~(*it)[2];
		}
	}
	}

	return mat;
}

cv::Mat& invert_mat_pointer(cv::Mat &mat) {
	int channels = mat.channels();

	int nRows = mat.rows;
	int nCols = mat.cols;

	int i, j;
	switch (channels) {
	case 1: {
		//            gray scale image
		uchar *p;
		for (i = 0; i < nRows; ++i) {
			p = mat.ptr<uchar>(i);
			for (j = 0; j < nCols; ++j) {
				p[j] = ~p[j];
			}
		}
	}
	case 3: {
		//            RGB image
		cv::Vec3b *p;
		for (i = 0; i < nRows; ++i) {
			p = mat.ptr<cv::Vec3b>(i);
			for (j = 0; j < nCols; ++j) {
				p[j][0] = ~p[j][0];
				p[j][1] = ~p[j][1];
				p[j][2] = ~p[j][2];
			}
		}
	}
	}

	return mat;
}



void show_mat(const cv::Mat &image, std::string const &win_name)
{
	cv::namedWindow(win_name, cv::WINDOW_AUTOSIZE);
	imshow(win_name, image);

	cv::waitKey(0);
}

void show_mat_mouse_click(cv::Point& point, const cv::Mat &image, std::string const &win_name)
{
	cv::namedWindow(win_name, cv::WINDOW_AUTOSIZE);
	cv::setMouseCallback(win_name, onMouse, (void*)&point);
	imshow(win_name, image);

	cv::waitKey(0);
}
