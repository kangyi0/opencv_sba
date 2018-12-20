#include <opencv2/opencv.hpp>
#include "myopencv.h"

using namespace cv;

void color_darken(char *name) {
	Mat img = imread(name, 1);
	if (img.empty())
		return;
	Mat hsv, enhanced;
	imshow("org", img);

	cvtColor(img, hsv, COLOR_BGR2HSV);
	std::vector<Mat> channels(3);
	split(hsv, channels);

	channels[2] = 0.7* channels[2];
	merge(channels, enhanced);

	cvtColor(enhanced, enhanced, COLOR_HSV2BGR);
	imshow("darken", enhanced);
	waitKey(0);

	destroyAllWindows();
	return;
}

void get_color_table(uchar tables[], int amt) {

	for (int i = 0; i <= 180; i++)
		tables[i] = (i+amt)%180;
	for (int i = 181; i < 256; i++)
		tables[i] = 0;
}

void test_color() {

	//Mat rgb(3, 2, CV_8UC3, Scalar(100, 200, 0));
	Mat rgb = (Mat_<Vec3b>(3, 1) << Vec3b(100, 200, 300), Vec3b(0, 255, 255), Vec3b(50, 30, 200));
	//rgb.at<Vec3b>(0, 1) = Scalar(255, 120, 55);
	//rgb.at<Vec3b>(1, 0) = Scalar(200, 200, 200);


	Mat hsv, inv_rgb, inv_hsv;

	cout << "rgb :" << rgb << endl;

	cvtColor(rgb, hsv, COLOR_BGR2HSV);
	cout << "hsv  before:" << hsv << endl;
//	cvtColor(hsv, rgb, COLOR_HSV2BGR);
//	cout << "rgb2 " << rgb << endl;
//	cvtColor(rgb, hsv, COLOR_BGR2HSV);
//	cout << "hsv2" << hsv << endl;

	inv_rgb = Scalar(255, 255, 255) - rgb;
	cout << "rgb after inv: " << inv_rgb << endl;
	cvtColor(inv_rgb, inv_hsv, COLOR_BGR2HSV);
	cout << "hsv(rgb after inv)" << inv_hsv << endl;

	cout << "===============================" << endl;
	vector<Mat>ch(3);

	split(hsv, ch);
	uchar table[256];
	get_color_table(table, 90);
	Mat mat_table(1, 256, CV_8U,table);
	LUT(ch[0], mat_table, ch[0]);
	ch[2] = 255 - ch[2];
	Mat inv_hsv2, inv_rgb2;
	merge(ch, inv_hsv2);
	cout << "hsv after inv: " << inv_hsv2 << endl;

	cvtColor(inv_hsv2, inv_rgb2, COLOR_HSV2BGR);
	cout << "rgb (hsv after inv):" << inv_rgb2 << endl;
}

void color_inverse_rgb(char *name) {
	Mat img = imread(name, 1);
	if (img.empty())
		return;

	resize(img, img, Size(0, 0), 0.5, 0.5);

	vector<Mat> ch1(3);
	Mat hsv, enhanced;
	imshow("org", img);
	cvtColor(img, hsv, COLOR_BGR2HSV);
	split(hsv, ch1);
	imshow("V before inverse in BGR", ch1[2]);
	//cout << "V mean before inverse in BGR " << mean(ch1[2]) << endl;
	//cout << "V before inverse " << endl  << ch1[2] << endl;

	img = Scalar(255, 255, 255) - img;
	imshow("inverse in BGR ", img);

	cvtColor(img, hsv, COLOR_BGR2HSV);
	vector<Mat> ch2(3);
	split(hsv, ch2);
	imshow("V after inverse in BGR", ch2[2]);
	//cout << "mean V after inverse in BGR = " << mean(ch2[2]) << endl;
	//cout << "V after inverse (in BGR) " << endl << ch2[2] << endl;

	//
	ch2[1] = 255;
	ch2[2] = 255;
	merge(ch2, img);
	cvtColor(img, img, COLOR_HSV2BGR);
	imshow("pure-inverse color in BGR", img);
}

void color_inverse(char *name) {
	Mat img = imread(name, 1);
	if (img.empty())
		return;

	resize(img, img, Size(0, 0), 0.5, 0.5);
	Mat hsv, blurr, enhanced;
	imshow("org", img);

	cvtColor(img, hsv, COLOR_BGR2HSV);

	std::vector<Mat> channels(3);
	split(hsv, channels);
	//cout << "mean V before inverse (HSV)= " << mean(channels[2]) << endl;
	imshow("before V", channels[2]);
	//cout << "before V (HSV)=" << endl << channels[2] << endl;

	unsigned char color_comp_tab[256];
	get_color_table(color_comp_tab,90);
	Mat mapping(1, 256, CV_8UC1, color_comp_tab);
	LUT(channels[0], mapping, channels[0]); // H
	channels[2] = 255 -channels[2]; // V

	imshow("after V", channels[2]);
	//cout << "after V (HSV)=" << endl << channels[2] << endl;
	//cout << "mean V after inverse (HSV)= " << mean(channels[2]) << endl;

	merge(channels, hsv);
	cvtColor(hsv, img, COLOR_HSV2BGR);
	imshow("inverse", img);

	color_inverse_rgb(name);

	vector<Mat> ch2;
	split(hsv, ch2);

	ch2[1] = 255;
	ch2[2] = 255;
	
	merge(ch2, img);
	cvtColor(img, img, COLOR_HSV2BGR);
	imshow("pure", img);
	int key = waitKey(0);
	if (key == 'w')
		imwrite("pure.jpg", img);
	destroyAllWindows();
	return;
}


void color_correction(char *name) {
	Mat img = imread(name, 1);
	if (img.empty())
		return;

	resize(img, img, Size(0, 0), 0.7, 0.7);
	uchar gamma_table[256];
	get_gamma_LUT(gamma_table,2.5);
	Mat gamma_mat(1, 256, CV_8UC1, gamma_table);
	
	vector<Mat> channels;
	split(img, channels);
	for (int i = 0; i < 3; i++) {
		LUT(channels[i], gamma_mat, channels[i]);
	}
	Mat res;
	merge(channels,res);
	
	imshow("src", img);
	imshow("res", res);

	waitKey(0);
	destroyAllWindows();
}

void color_enhance(char *name) {
	Mat img = imread(name, 1);
	if (img.empty())
		return;
	resize(img, img, Size(0, 0), 0.5, 0.5);
	Mat hsv, enhanced;
	imshow("org", img);

	cvtColor(img, hsv, COLOR_BGR2HSV);
	std::vector<Mat> channel(3);
	split(hsv, channel);
	equalizeHist(channel[2], channel[2]);
	merge(channel, enhanced);

	cvtColor(enhanced, enhanced, COLOR_HSV2BGR);
	imshow("enhanced", enhanced);
	
	waitKey(0);
	destroyAllWindows();

}


void color_smooth(char *name)
{
	Mat img = imread(name, 1);
	if (img.empty())
		return;
//	resize(img, img, Size(0, 0), 0.8, 0.8);
	Mat hsv, smooth;
	imshow("org", img);

	blur(img, smooth, Size(11, 11));
	imshow("smooth(RGB)", smooth);
	
	cvtColor(img, hsv, COLOR_BGR2HSV);
	std::vector<Mat> channel(3);
	split(hsv, channel);

	blur(channel[2], channel[2], Size(11, 11));

	merge(channel, hsv);
	cvtColor(hsv, img, COLOR_HSV2BGR);
	imshow("smoothing(V ch)", img);

	waitKey(0);

	destroyAllWindows();

}


void color_laplacian(char *name)
{
	Mat img = imread(name, 1);
	if (img.empty())
		return;

	Mat hsv, filtered, rgb_filtered;
	imshow("org", img);
	vector<Mat> rgb_ch(3);
	split(img, rgb_ch);

	for (int i = 0; i < 3; i++) {
		Laplacian(rgb_ch[i], filtered, CV_16S);
		convertScaleAbs(filtered, filtered);
		rgb_ch[i] = rgb_ch[i] + 1.5* filtered;
 	}
	merge(rgb_ch, rgb_filtered);
	imshow("Laplacian (RGB)", rgb_filtered);

	cvtColor(img, hsv, COLOR_BGR2HSV);
	std::vector<Mat> channel(3);
	split(hsv, channel);
	imshow("ch[2]", channel[2]);
	Laplacian(channel[2], filtered, CV_16S);
	convertScaleAbs(filtered, filtered);
	channel[2] = channel[2] + 1.5* filtered;
	imshow("Laplacian ch[2]", channel[2]);
	merge(channel, hsv);
	cvtColor(hsv, img, COLOR_HSV2BGR);
	imshow("Laplacian(HSV)", img);

	waitKey(0);

	destroyAllWindows();
}
char *winname = "color";
static int BlueRatio = 0;

static vector<Mat> img_channel(3);

void color_change(int, void*)
{
	Mat YCrCb, bgr;
	vector<Mat> newch(3);

	BlueRatio = 50 - BlueRatio;
	newch[0] = img_channel[0];
	newch[1] = img_channel[1] - BlueRatio;
	newch[2] = img_channel[2] + BlueRatio;
	merge(newch, YCrCb);
	cvtColor(YCrCb, bgr, COLOR_YCrCb2BGR);
	imshow(winname, bgr);
}

void color_ratio(char *name) {
	Mat input_img;
	input_img = imread(name, 1);
	if (input_img.empty())
		return;
	resize(input_img, input_img, Size(0, 0), 0.8, 0.8);
	Mat yuv;
	imshow("org", input_img);
	
	cvtColor(input_img, input_img, COLOR_BGR2YCrCb);
	split(input_img, img_channel);
	
	namedWindow(winname, CV_WINDOW_AUTOSIZE);
	createTrackbar("Blue", winname, &BlueRatio, 100, color_change);
	waitKey(0);
	destroyAllWindows();
}

void color_segment(char *name) 
{
	Mat input_img;
	input_img = imread(name, 1);
	if (input_img.empty())
		return;
	
	//resize(input_img, input_img, Size(0, 0), 0.8, 0.8);
	Mat hsv, maskH, maskS, mask,img;
	imshow("org", input_img);
	cvtColor(input_img, hsv, COLOR_BGR2HSV);
	vector<Mat>ch;

	split(hsv, ch);
	threshold(ch[1], maskS, 30, 255, THRESH_BINARY);
	imshow("mask", maskS);
	bitwise_and(ch[0], maskS, ch[0]);
	inRange(ch[0], Scalar(0), Scalar(30), maskH);
	bitwise_and(maskH, ch[0], ch[0]);
	imshow("masked H", ch[0]);
	bitwise_and(maskH, ch[1], ch[1]);
	//ch[1] = mask;
	merge(ch, img);
	cvtColor(img, img, COLOR_HSV2BGR);
	//cvtColor(mask, mask, COLOR_GRAY2BGR);
	//bitwise_and(mask, input_img,img);
	imshow("img", img);

	cvtColor(input_img, hsv, COLOR_BGR2HSV);
	split(hsv, ch);
	ch[1] = ch[2] = 255;
	merge(ch, hsv);
	cvtColor(hsv, img, COLOR_HSV2BGR);
	imshow("pure color", img);
	int w = waitKey(0);
	if (w == 'w')
		imwrite("face_color2.png", img);
}

void change_colortone(char *name) {
	Mat img = imread(name, IMREAD_UNCHANGED);
	Mat blurred, dst;
	if (img.empty())
		return;
	imshow("Output", img);
	waitKey(0);

}