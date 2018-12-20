#include <opencv2/opencv.hpp>
#include "myopencv.h"

using namespace std;
using namespace cv;

void thresh_test(char *name)
{
	Mat img = imread(name, IMREAD_GRAYSCALE);
	if (img.empty())
		return;
	Mat dst;

	imshow("org", img);

	threshold(img, dst, 100, 255, THRESH_BINARY);
	imshow("BINARY", dst);
	threshold(img, dst, 100, 255, THRESH_BINARY_INV);
	imshow("INV", dst);
	threshold(img, dst, 100, 255, THRESH_TRUNC);
	imshow("TRUNC", dst);
	threshold(img, dst, 100, 255, THRESH_TOZERO);
	imshow("TOZERO", dst);
	waitKey(0);
}

void adaptive_threshold_test(char *name)
{
	Mat img = imread(name, IMREAD_GRAYSCALE);
	if (img.empty())
		return;
	Mat dst;
	imshow("org", img);

	adaptiveThreshold(img, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 19, 0);
	imshow("MEAN_C 19", dst);
	adaptiveThreshold(img, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 19, 0);
	imshow("GAUSSIAN 19", dst);
	adaptiveThreshold(img, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 49, 0);
	imshow("MEAN_C 49", dst);
	adaptiveThreshold(img, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 49, 0);
	imshow("GAUSSIAN 49", dst);
	adaptiveThreshold(img, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 199, 0);
	imshow("MEAN_C 199", dst);
	adaptiveThreshold(img, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 199, 0);
	imshow("GAUSSIAN 199", dst);
	waitKey(0);

}

void thresh_otsu(char *name) {
	Mat img = imread(name, IMREAD_GRAYSCALE);
	if (img.empty())
		return;
	Mat dst;
	imshow("org", img);

	blur(img, img, Size(5, 5));
	threshold(img, dst, 0, 255, THRESH_OTSU);
	imshow("thresh_OTSU", dst);
	show_hist(img, "img hist");
	waitKey(0);
}

void vignette(char *name)
{
	Mat img = imread(name, IMREAD_GRAYSCALE);
	if (img.empty())
		return;
	Mat dst;
	imshow("org", img);

	Mat kernel_x = getGaussianKernel(img.cols, (img.cols*0.6)/2); // column vector with x coord size
	Mat kernel_y = getGaussianKernel(img.rows, (img.rows*0.6)/2 ); // column vector with y coord size
	Mat kernel_x_transpose;

	transpose(kernel_x, kernel_x_transpose); // make row vector by transposing column vector of x size

	Mat kernel = kernel_y * kernel_x_transpose;
	normalize(kernel, kernel, 0, 1, NORM_MINMAX); // normalize kernel 

	img.convertTo(dst, CV_64F); // CV_8U -> CV_64F
	multiply(kernel, dst, dst); // dst = mask * dst
	convertScaleAbs(dst, dst); // CV_64F -> CV_8U
	imshow("kernel", kernel);
	imshow("dst", dst);
	waitKey(0);
}
