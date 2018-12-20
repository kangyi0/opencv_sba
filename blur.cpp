#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


void smoothing(char *name) {
	Mat img = imread(name, 0);
	Mat blurred, dst;
	if (img.empty())
		return;
	imshow("Output", img);
	char title[20];
	for (int i = 5; i < 20; i += 6) {
		blur(img, blurred, Size(i, i));
		sprintf(title, "mean %d", i);
		imshow(title, blurred);

		medianBlur(img, blurred, i);
		sprintf(title, "median %d", i);
		imshow(title, blurred);

		GaussianBlur(img, blurred, Size(i, i), 0, 0);
		sprintf(title, "Gaussian %d", i);
		imshow(title, blurred);

		bilateralFilter(img, blurred, i, i * 2, i / 2);
		sprintf(title, "bilateral %d", i);
		imshow(title, blurred);
	}
	waitKey(0);
}

void print_Gaussian(double size, double sigma) {

	Mat kernel = getGaussianKernel(size, sigma);
	int row = kernel.rows;
	int col = kernel.cols;
	if (kernel.isContinuous()) {
		col = col * row;
		row = 1;
	}

	cout << "Gaussian Kernel size=" << size << " sigma= " << endl;

	for (int i = 0; i < row; i++) {
		double *row_ptr = kernel.ptr<double>(i);
		for (int j = 0; j < col; j++)
			cout << row_ptr[j] << "  ";
		cout << endl;
	}
}

void GaussianTest(char *name) {

	print_Gaussian(7, 1.0);
	print_Gaussian(7, 2.0);
	print_Gaussian(7, 3.0);
	Mat img = imread(name, 0);
	Mat dst;
	if (img.empty())
		return;
	imshow("Output", img);

	GaussianBlur(img, dst, Size(7, 7), 1.0, 1.0);
	imshow("1.0", dst);
	GaussianBlur(img, dst, Size(7, 7), 2.0, 2.0);
	imshow("2.0", dst);
	GaussianBlur(img, dst, Size(7, 7), 3.0, 3.0);
	imshow("3.0", dst);
	GaussianBlur(img, dst, Size(7, 7), 4.0, 4.0);
	imshow("4.0", dst);

	waitKey(0);
}

void myfilter_test(char *name) {
	const int KSIZE = 7;
	Mat img = imread(name, 0);
	Mat dst;
	if (img.empty())
		return;
	imshow("Output", img);

	Mat kernel = Mat::ones(KSIZE, KSIZE, CV_32F);
	kernel = kernel / (float)(KSIZE*KSIZE);
	filter2D(img, dst, -1, kernel);
	imshow("Filter output", dst);

	waitKey(0);

}

void myfilter2_test(char *name) {
	const int KSIZE = 3;
	Mat img = imread(name, 0);
	Mat dst;
	if (img.empty())
		return;
	imshow("Output", img);

	Mat kernel(KSIZE, KSIZE, CV_32F);

	kernel = (Mat_<float>(3, 3) << 1.0, 2.0, 1.0, 2.0, 4.0, 2.0, 1.0, 2.0, 1.0);
	kernel = kernel / 16.0;
	cout << kernel;
	filter2D(img, dst, -1, kernel);
	imshow("Filter output", dst);

	waitKey(0);

}
