#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

void get_logTable(uchar lut[]);


void prepare_lut(unsigned char lut[], float fGamma, float C)
{
	cout << "Gamma Transform Table" << endl;
	float max = 0;
	for (int i = 0; i < 256; i++) {
		float v = pow((float)(i / 255.0), fGamma) * 255.0f * C;
		lut[i] = saturate_cast<uchar>(v);
		cout << i << ':' << v << ':' << (int)lut[i] << endl;
	}

}


void gamma_tranformTest(char *name) {

	Mat img, gray;
	unsigned char lut1[256], lut2[256], log_lut[256];
	gray = imread(name);

	if (gray.empty())
		return;

	prepare_lut(lut1, 0.2, 1);
	prepare_lut(lut2, 2.5, 1);
	get_logTable(log_lut);

	Mat table(1, 256, CV_8UC1, log_lut);
	Mat table2(1, 256, CV_8UC1, lut1);
	Mat table3(1, 256, CV_8UC1, lut2);

	Mat T = gray.clone();
	Mat T2 = gray.clone();
	Mat T3 = gray.clone();

	LUT(gray, table, T);
	LUT(gray, table2, T2);
	LUT(gray, table3, T3);

	imshow("org gray", gray);
	imshow("gamma=0.2", T2);
	imshow("gamma=2.5", T3);
	imshow("log", T);

	waitKey(0);
	destroyAllWindows();

}

void negativeTest(char *name)
{
	Mat img = imread(name);
	if (img.empty())
		return;
	Mat neg = Scalar(255, 255, 255) - img;
	imshow("neg", neg);
	imshow("org", img);
	waitKey(0);
	destroyAllWindows();

}


void draw_log_scale()
{
	uchar LUT[256];

	get_logTable(LUT);
	Mat bg(50, 6 * 256, CV_8UC1, Scalar(0));
	Mat bg2(50, 6 * 256, CV_8UC1);
	for (int i = 0; i < 256; i++) {
		uchar v = LUT[i];
		rectangle(bg, Rect(i * 6, 0, 6, 50), Scalar(v), -1);
		rectangle(bg2, Rect(i * 6, 0, 6, 50), Scalar(i), -1);
		cout << i << ":" << (int)v << endl;
	}
	imshow("logarithm", bg);
	imshow("linear", bg2);
	waitKey(0);
}


void get_logTable(uchar lut[])
{
	double rmax = 255;

	cout << "LOG transform Table " << endl;
	double C = 255.0 / log10(rmax + 1);
	for (int i = 0; i < 256; i++) {
		float v = C * std::log10((float)i + 1.0);
		lut[i] = saturate_cast<uchar>(v);
	}
}

void process_ImageLog(char *name) {
	Mat img = imread(name,0);
	uchar table[256];

	imshow("input", img);
	get_logTable(table);
	for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++)
			img.at<uchar>(i, j) = table[img.at<uchar>(i, j)];

	imshow("processed", img);
	waitKey(0);
}

void process_ImageLog_math(char *name) {
	Mat img = imread(name, 0);
	Mat dst;

	img.convertTo(dst, CV_32F); // 8U --> 32F
	dst = dst + 1;
	log(dst, dst);
	normalize(dst, dst, 0, 255, NORM_MINMAX);
	convertScaleAbs(dst, dst); // 32F --> 8U
	imshow("input", img);
	imshow("processed", dst);
	waitKey(0);
}


void process_Log_LUT(char *name)
{
	Mat img = imread(name, 0);
	Mat Table(1, 256, CV_8U), dst;

	uchar *p = Table.data;

	double C = 255.0 / log10(255 + 1);
	for (int i = 0; i < 256; i++)
		p[i] = saturate_cast<uchar>(C * log10(i + 1.0)); // option 1
	   // Table.at<uchar>(0,i) = saturate_cast<uchar>(C * log10(i+1.0)); // option 2 

	LUT(img, Table, dst);
	imshow("input", img);
	imshow("processed", dst);
	 waitKey(0);

}


void get_expTable(uchar tab[], double rmax)
{
	//double rmax = 255;
	const double BASE = 1.02;
		double C = 255.0 / (pow(BASE, rmax) -1);
	for (int i = 0; i < 256; i++) {
		double v = C * (pow(BASE, (double)i) - 1.0);
		tab[i] = saturate_cast<uchar>(v);
	}
}

void process_Image_exp(char *name) 
{
	Mat img = imread(name, IMREAD_GRAYSCALE);
	imshow("input", img);
	double maxval;
	minMaxLoc(img, NULL, &maxval);

	uchar table[256];
	get_expTable(table, maxval);
	//get_logTable(table);
	Mat table_mat(1, 256, CV_8UC1, table);

	LUT(img, table_mat, img);
	imshow("processed", img);
	waitKey(0);
}


void draw_exp_scale()
{
	uchar LUT[256];

	get_expTable(LUT,255);
	Mat bg(50, 6 * 256, CV_8UC1, Scalar(0));
	Mat bg2(50, 6 * 256, CV_8UC1);
	for (int i = 0; i < 256; i++) {
		uchar v = LUT[i];
		rectangle(bg, Rect(i * 6, 0, 6, 50), Scalar(v), -1);
		rectangle(bg2, Rect(i * 6, 0, 6, 50), Scalar(i), -1);
		cout << i << ":" << (int)v << endl;
	}
	imshow("logarithm", bg);
	imshow("linear", bg2);
	waitKey(0);
}


void get_gamma_LUT(uchar table[], double gamma)
{
	double C = 255.0 / pow(255, gamma);
	for (int i = 0; i < 256; i++)
		table[i] = saturate_cast<uchar>(C * pow(i, gamma)); // option 1
}

void process_Gamma_LUT(char *name)
{
	Mat img = imread(name, 0);
	imshow("input", img);

	Mat dst;
	uchar table[256];

	get_gamma_LUT(table, 2.0);
	Mat Table1(1,256, CV_8UC1, table);
	LUT(img, Table1, dst);
	imshow("processed (2.0)", dst);

	get_gamma_LUT(table, 0.5);
	Mat Table2(1, 256, CV_8UC1, table);
	LUT(img, Table2, dst);
	imshow("processed (0.5)", dst);
	
	waitKey(0);

}
