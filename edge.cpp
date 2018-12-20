#include <opencv2/opencv.hpp>
using namespace cv;

Mat get_HorizDerivKernel() {
	Mat kernel = (Mat_<float>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
	kernel = kernel / 3.0;
	return kernel;
}

Mat get_VertDerivKernel() {
	Mat kernel = (Mat_<float>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
	kernel = kernel / 3.0;
	return kernel;
}

Mat get_HorizSobelKernel() {
	Mat kernel = (Mat_<float>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
	//kernel = kernel / 3.0;
	return kernel;
}

Mat get_VertSobelKernel() {
	Mat kernel = (Mat_<float>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
	//kernel = kernel / 3.0;
	return kernel;
}

void edges(char*name)
{
	Mat img = imread(name, 0);

	//Mat horiz_kernel = get_HorizDerivKernel();
	//Mat vert_kernel = get_VertDerivKernel();
	Mat horiz_kernel = get_HorizSobelKernel();
	Mat vert_kernel = get_VertSobelKernel();
	Mat deriv_X, deriv_Y;

	std::cout << horiz_kernel << std::endl;
	std::cout << vert_kernel << std::endl;

	filter2D(img, deriv_X, CV_16S, horiz_kernel);
	filter2D(img, deriv_Y, CV_16S, vert_kernel);


	convertScaleAbs(deriv_X, deriv_X); // 16S -> 8U
	convertScaleAbs(deriv_Y, deriv_Y); // 16S -> 8U

	imshow("Horiz_Derivative", deriv_X);
	imshow("Vertical_Derivative", deriv_Y);
	waitKey(0);
}



void Sobel_edges(char*name)
{
	Mat img = imread(name, 0);
	Mat deriv_X, deriv_Y;
	imshow("org", img);
	Sobel(img, deriv_X, CV_16S, 1,0);
	Sobel(img, deriv_Y, CV_16S, 0,1);

	convertScaleAbs(deriv_X, deriv_X); // 16S -> 8U
	convertScaleAbs(deriv_Y, deriv_Y); // 16S -> 8U

	imshow("Horiz_Derivative", deriv_X);
	imshow("Vertical_Derivative", deriv_Y);
	waitKey(0);
}

void Scharr_edges(char*name)
{
	Mat img = imread(name, 0);
	Mat deriv_X, deriv_Y;
	imshow("org", img);
	Scharr(img, deriv_X, CV_16S, 1, 0);
	Scharr(img, deriv_Y, CV_16S, 0, 1);

	convertScaleAbs(deriv_X, deriv_X); // 16S -> 8U
	convertScaleAbs(deriv_Y, deriv_Y); // 16S -> 8U

	imshow("Horiz_Derivative", deriv_X);
	imshow("Vertical_Derivative", deriv_Y);
	waitKey(0);
}

Mat getGradientMag(Mat &dev_X, Mat &dev_Y) {
	Mat mag(dev_X.rows, dev_X.cols, CV_16S);
	for(int i =0; i < dev_X.rows; i++)
		for (int j = 0; j < dev_X.cols; j++) {
			mag.at<short>(i, j) = abs(dev_X.at<short>(i, j)) + abs(dev_Y.at<short>(i, j));
		}
	return mag;
}

void thresholdGradientMag(Mat &mag, Mat &edges, int threshold)
{
	edges = Mat::zeros(mag.size(), CV_8U);
	for (int i = 0; i < mag.rows; i++)
		for (int j = 0; j < mag.cols; j++)
			if (mag.at<short>(i, j) > threshold) 
				edges.at<uchar>(i, j) = 255;
}

void Sobel_Magnitudes(char *name)
{
	Mat img = imread(name, 0);
	Mat dev_X, dev_Y, edges;
	imshow("org", img);

	Sobel(img, dev_X, CV_16S, 1, 0);
	Sobel(img, dev_Y, CV_16S, 0, 1);

	Mat gradient_mag = getGradientMag(dev_X, dev_Y);
	thresholdGradientMag(gradient_mag, edges, 100);
	imshow("edge", edges);
	waitKey(0);
}

void Canny_edges(char*name)
{
	Mat img = imread(name, 0);
	if (img.empty())
		return;
	Mat blurred,dst;
	GaussianBlur(img, img, Size(7, 7), 10, 10);
	//bilateralFilter(img, blurred, 10, 20, 5);
	//img = blurred;
	imshow("img", img);
	Canny(img, dst, 80, 100, 3, false);
	imshow("Canny", dst);
	waitKey(0);
}

static Mat input_img, edge_img;
static char *win_name = "Canny";
static int lowThreshold, highThreshold;

void CannyThreshold(int, void*)
{
	Canny(input_img, edge_img, lowThreshold, highThreshold, 3);
	imshow(win_name, edge_img);
}

void Canny_test_thresholds(char *name)
{
	input_img = imread(name, 0);
	
	edge_img.create(input_img.size(), input_img.type());

	namedWindow(win_name, CV_WINDOW_AUTOSIZE);
	createTrackbar("Threshold1", win_name, &lowThreshold, 200, CannyThreshold);
	createTrackbar("Threshold2", win_name, &highThreshold, 255, CannyThreshold);
	waitKey(0);
}

void noisy_Canny(char *name)
{
	Mat dst;
	Mat img = imread(name, IMREAD_GRAYSCALE);
	imshow("input", img);
	Canny(img, dst, 70, 180);
	imshow("Canny", dst);
	Mat gaussian_noise = img.clone();
	randn(gaussian_noise, 0,20);
	dst = img + gaussian_noise;
	imshow("N", dst);
	Mat edge;
	Canny(dst, edge, 70, 180);
	imshow("Noise Canny", edge);
	int key = waitKey(0);
	if (key == ' ')
		imwrite("noisy.tif", dst);
}

void Laplacian_Test(char *name)
{
	Mat dst;
	Mat img = imread(name, IMREAD_GRAYSCALE);

	Laplacian(img, dst, CV_16S);
	convertScaleAbs(dst, dst);

	imshow("Laplacian", dst);
	waitKey(0);
	
}