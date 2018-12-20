#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

vector<uchar> getLUT() {
	vector<uchar> Table(256, 0);
	for (int i = 0; i < 256; ++i)
		Table[i] = (uchar)(255 - i);
	return Table;
}

void process_ImageNegative(Mat &M) {
	vector<uchar> Table = getLUT();
	for (int i = 0; i < M.rows; ++i)
		for (int j = 0; j < M.cols; ++j)
			M.at<uchar>(i, j) = Table[M.at<uchar>(i, j)];
}

void negative_test(char *name) {
	Mat img = imread(name, IMREAD_GRAYSCALE);
	Mat dst = img.clone();
	process_ImageNegative(dst);
	imshow("input img", img);
	imshow("processed img", dst);
	waitKey(0);
}