#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


void show_hist(Mat & img, char *wname)
{
	Mat hist;
	float ranges[] = { 0, 255 };
	const float * histRange = { ranges };
	const int histSize[] = { 256 };
	bool uniform = true;
	bool accum = false;
	const int channels[] = { 0 };
	//const Mat img2 = img.clone();
	// compute histogram
	calcHist(&img, 1, channels, Mat(), hist, 1, histSize, &histRange, uniform, accum);

	double maxVal = 0;
	minMaxLoc(hist, 0, &maxVal, 0, 0);

	int scale = 2;
	Mat histImg = Mat::zeros(256 * scale, 256 * scale, CV_8UC3);

	for (int v = 0; v < 256; v++) {
		float binVal = hist.at<float>(v);
		int  num = cvRound(binVal * 255.0 / maxVal);

		rectangle(histImg, Point(v*scale, num*scale),
			Point((v + 1)*scale - 1, 0),
			Scalar(0, 200, 100), CV_FILLED);
	}

	flip(histImg, histImg, 0);

	imshow(wname, histImg);
}


void histogram_Test(char *name)
{
	Mat img = imread(name, 0);
	Mat dst;
	if (img.empty())
		return;
	imshow("org", img);
	show_hist(img, "input hist");

	equalizeHist(img, dst);
	imshow("eqlaized", dst);
	show_hist(dst, "dst hist");

	waitKey(0);
}

void sharpening(char *name) {
	Mat img = imread(name, 0);
	Mat blurred, dst;
	if (img.empty())
		return;

	blur(img, blurred, Size(7, 7));
	addWeighted(img, 2.0, blurred, -1.0, 0.0, dst);

	imshow("org", img);
	imshow("blurred", blurred);
	imshow("shapen", dst);
	waitKey(0);
}

// Revised 12/12/2018
#define ROW 3
#define COL 4
#define SUB_WIDTH 300
void change_contrast_brightness(char *name)
{
	Mat img = imread(name, IMREAD_GRAYSCALE);
	Mat blurred, dst;
	if (img.empty())
		return;

	float a = 1;
	float b = 0;
	float ratio = img.rows / img.cols;
	resize(img, img, Size(SUB_WIDTH, SUB_WIDTH*ratio));

	Mat big(ROW* (SUB_WIDTH * ratio), SUB_WIDTH* COL, CV_8UC1, Scalar(0));

	imshow("org", img);

	char title[30];
	for (int i = 0; i < COL; i++)
		for (int j = 0; j < ROW; j++) {
			Mat ROI(big, Rect(Point(i * SUB_WIDTH, j * SUB_WIDTH*ratio), Size(SUB_WIDTH, SUB_WIDTH*ratio)));
			a = 1.0 + (i + 0.5);
			b = j * 60;
			ROI = a * img + b;
			sprintf(title, "a=%2.1f,b=%2.1f", a, b);
			putText(ROI, title, Point(25,15), FONT_HERSHEY_PLAIN, 1.0, Scalar(0), 2);
		}
	imshow("contrast", big);
	waitKey(0);
}
