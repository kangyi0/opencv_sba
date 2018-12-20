#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void basic_test_constructor() {

	Mat M0(100, 200, CV_8UC1, Scalar(50));
	Mat M1(100, 200, CV_8UC1, Scalar(100));
	Mat M2(100, 200, CV_8UC1, Scalar(150));
	Mat M3(Size(200, 100), CV_8UC3, Scalar(0, 0, 255));
	Mat M4(Size(200, 100), CV_8UC3, Scalar(255, 0, 0));
	Mat M5(Size(200, 100), CV_8UC3, Scalar(255, 0, 255));

	imshow("M0", M0);
	imshow("M1", M1);
	imshow("M2", M2);
	imshow("M3", M3);
	imshow("M4", M4);
	imshow("M5", M5);
	waitKey(0);
}

void basic_test_subMat(char *name) {
	Mat img;
	img = imread(name, IMREAD_UNCHANGED);
	Mat dst;
	if (img.empty())
		return;
	imshow("org", img);

	Mat sub(img, Rect(30, 60, 200, 400));
	sub = Scalar(0, 255, 0);
	Mat sub2(img, Rect(Point(230, 60), Point(430, 460)));
	sub2 = Scalar(255, 255, 255);
	imshow("sub", sub);
	imshow("sub2", sub2);
	imshow("org+sub", img);
	waitKey(0);

}

void basic_test_split(char *name)
{
	Mat img = imread(name, IMREAD_COLOR);

	if (img.empty())
		return;

	resize(img, img, Size(300, 300.0 * img.rows / img.cols));
	imshow("org", img);

	vector<Mat> channel;
	split(img, channel);
	char wname[10];
	for (int i = 0; i < img.channels(); i++) {
		sprintf(wname, "ch%d", i);
		imshow(wname, channel[i]);
	}
	channel[0] = channel[0] / 2;
	channel[1] = channel[1] / 2;
	Mat mrg;
	merge(channel, mrg);
	imshow("mrg", mrg);
	waitKey(0);
}

void basic_test_deepCopy(char *name)
{
	Mat img = imread(name, IMREAD_COLOR);
	if (img.empty())
		return;

	resize(img, img, Size(300, 300.0 * img.rows / img.cols));
	imshow("org", img);

	Mat M1(img); // share the whole image area
	Mat M2, M3, M4;

	M2 = img;
	M3 = img.clone();
	img.copyTo(M4);

	M2 = Scalar(200, 100, 100);
	M3 = Scalar(0, 0, 255);
	M4 = Scalar(0, 255, 0);

	imshow("M1", M1);
	imshow("M2", M2);
	imshow("M3", M3);
	imshow("M4", M4);

	waitKey(0);
}

void basic_test_size()
{
	Mat M(10, 15, CV_8UC3);
	int imagesize = M.cols * M.rows;
	imagesize *= M.channels() * M.elemSize1();
	cout << "depth:" << M.elemSize1() << endl;
	std::cout << "total size = " << imagesize << " bits" << std::endl;
}

void basic_traversal(char *name) {
	Mat M = imread(name, 0);// IMREAD_UNCHANGED);
	if (M.empty())
		return;
	imshow("org", M);
	const int channels = M.channels();
	switch (channels) {
	case 1: {
		MatIterator_<uchar> it, end;
		for (it = M.begin<uchar>(), end = M.end<uchar>(); it != end; ++it)
			if (*it > 100)
				*it = 255;
		break;
	}
	case 3: {
		int B = 0, G = 0, R = 0, tot = 0;
		MatIterator_<Vec3b> it, end;
		for (it = M.begin<Vec3b>(), end = M.end<Vec3b>(); it != end; ++it) {
			if ((*it)[0] > 100) {
				B++;
				(*it)[0] = 255;
			}
			if ((*it)[1] > 100) {
				G++;
				(*it)[1] = 255;
			}
			if ((*it)[2] > 100) {
				R++;
				(*it)[2] = 255;
			}
			tot++;
		}
		cout << "B=" << (float)B / tot << " G=" << (float)G / tot
			<< " R=" << (float)R / tot << endl;
		break;
	}
	}
	imshow("img", M);
	waitKey(0);

}

void color_test(char *name) {
	Mat A;
	A = imread(name);
	if (A.empty()) {
		cout << "file read error" << endl;
		return;
	}

	imshow("org", A);
	Mat B, C;
	cvtColor(A, B, COLOR_BGR2GRAY);
	imshow("GRAY", B);
	cvtColor(A, B, COLOR_BGR2HSV);
	imshow("HSV", B);

	vector<Mat> channels;
	split(B, channels);
	imshow("H", channels[0]);
	imshow("S", channels[1]);
	imshow("V", channels[2]);
	waitKey(0);
}

void frame_diff()
{
	VideoCapture cap(2);
	Mat frame, prev, dst;
	cap >> prev;
	cvtColor(prev, prev, COLOR_BGR2GRAY);
	while (1) {
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		imshow("live", frame);
		absdiff(frame, prev, dst);
		threshold(dst, dst, 3, 255, THRESH_BINARY);
		imshow("diff", dst);
		prev = frame.clone();
		int key = waitKey(33);
		if (key == 27)
			break;
	}
}


void morph_testing(char * name)
{
	Mat img;
	img = imread(name, 0);
	Mat blurred, dst;
	if (img.empty())
		return;
	imshow("org", img);

	Mat mask;

	mask = getStructuringElement(MORPH_RECT, Size(11, 11));
	dilate(img, dst, mask);
	imshow("Dilate", dst);
	erode(img, dst, mask);
	imshow("erode", dst);

	morphologyEx(img, dst, MORPH_CLOSE, mask, Point(-1, -1), 1);
	imshow("open", dst);
	waitKey(0);
}
