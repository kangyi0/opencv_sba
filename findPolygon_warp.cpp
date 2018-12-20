#include "opencv.hpp"

/* WARPING POINT */
#define WARP_POINT0_X 15 
#define WARP_POINT0_Y 120
#define WARP_POINT1_X 460
#define WARP_POINT1_Y 120
#define WARP_POINT2_X 370
#define WARP_POINT2_Y 440
#define WARP_POINT3_X 100
#define WARP_POINT3_Y 440
#define TOP_POINT_Y 150
#define BOTTOM_POINT_Y 350

using namespace std;
using namespace cv;

/* get lambda matrix to perform warpPerspective transform */
void warping(Mat& src, Mat& dst)
{
	Mat lambda;
	Point2f play_point0(WARP_POINT0_X, WARP_POINT0_Y);
	Point2f play_point1(WARP_POINT1_X, WARP_POINT1_Y);
	Point2f play_point2(WARP_POINT2_X, WARP_POINT2_Y);
	Point2f play_point3(WARP_POINT3_X, WARP_POINT3_Y);

	float val_y0 = WARP_POINT0_Y + (WARP_POINT2_X - WARP_POINT3_X)*1.2;
	float val_y1 = WARP_POINT1_Y + (WARP_POINT2_X - WARP_POINT3_X)*1.2;

	// Input Quadilateral or Image plane coordinates
	// The 4 points that select quadilateral on the input , from top-left in clockwise order
	Point2f inputQuad[4];
	inputQuad[0] = play_point0;
	inputQuad[1] = play_point1;
	inputQuad[2] = play_point2;
	inputQuad[3] = play_point3;

	// Output Quadilateral or World plane coordinates
	// The 4 points that select quadilateral on the input , from top-left in clockwise order
	Point2f outputQuad[4];
	outputQuad[0] = Point(WARP_POINT3_X, WARP_POINT0_Y);
	outputQuad[1] = Point(WARP_POINT2_X, WARP_POINT1_Y);
	outputQuad[2] = Point(WARP_POINT2_X, val_y1);
	outputQuad[3] = Point(WARP_POINT3_X, val_y0);

	lambda = Mat::zeros(src.cols, src.rows, src.type());

	// Get the Perspective Transform Matrix i.e. lambda 
	lambda = getPerspectiveTransform(inputQuad, outputQuad);

	Mat warpFrame;
	warpPerspective(src, warpFrame, lambda, warpFrame.size());
	dst = warpFrame.clone();

}

double getDistance(const Point& p1, const Point& p2) {

	double distance;

	// 피타고라스의 정리
	// pow(x,2) x의 2승,  sqrt() 제곱근
	distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

	return distance;
}

int checkPoly(vector<Point>& poly)
{
	vector<float> corner;

	//calculate distances between vertices
	for (int i = 0; i < poly.size(); i++) {
		corner.push_back(getDistance(poly[i], poly[(i + 1) % poly.size()]));
	}
	sort(corner.begin(), corner.end());	//sort by ascending order

	vector<float>cornerRate;
	for (int i = 0; i < corner.size(); i++) {
		cornerRate.push_back(corner[i] / corner[0]);
	}

	float margin = 0.35;
	if (corner.size() == 3) {
		if (((cornerRate[0] >= (1 - margin)) && (cornerRate[0] <= (1 + margin))) &&
			((cornerRate[1] >= (1 - margin)) && (cornerRate[1] <= (1 + margin))) &&
			((cornerRate[2] >= (1.4 - margin)) && (cornerRate[2] <= (1.4 + margin))))
			return 0; 	//직각삼각형 - 1:1:1.4
		else return 1;
	}
	else if (corner.size() == 4) {
		if (((cornerRate[0] >= (1 - margin)) && (cornerRate[0] <= (1 + margin))) &&
			((cornerRate[1] >= (1 - margin)) && (cornerRate[1] <= (1 + margin))) &&
			((cornerRate[2] >= (1 - margin)) && (cornerRate[2] <= (1 + margin))) &&
			((cornerRate[3] >= (1 - margin)) && (cornerRate[3] <= (1 + margin))))
			return 0; //정사각형 - 1:1:1:1

		else if (((cornerRate[0] >= (1 - margin)) && (cornerRate[0] <= (1 + margin))) &&
			((cornerRate[1] >= (1 - margin)) && (cornerRate[1] <= (1 + margin))) &&
			((cornerRate[2] >= (1.4 - margin)) && (cornerRate[2] <= (1.4 + margin))) &&
			((cornerRate[3] >= (1.4 - margin)) && (cornerRate[3] <= (1.4 + margin))))
			return 0; //평행사변 - 1:1:1.4:1.4

		else return 1;
	}
	else return 1;
}


void countPolygon(char* name)
{
	int count = 0;
	Mat src = imread(name);
	Mat warp;
	warping(src, warp);
	imshow("warp", warp);

	//cut source frame into enough size to find object
	Mat canny;
	Canny(warp, canny, 10, 30);
	dilate(canny, canny, Mat(), Point(-1, -1), 3);

	//find contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(canny, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	if (contours.size() <= 0) {
		cout << "No materials" << endl;
		return;
	}

	//find block area
	double largest_area = 0;
	int largest_contour_index = -1;
	for (int i = 0; i < contours.size(); i++) {
		RotatedRect rect = minAreaRect(contours[i]);
		double sz = rect.size.width*rect.size.height;
		if (sz > largest_area && sz < (double)(src.cols*src.rows)*0.8) {
			largest_area = sz;
			largest_contour_index = i;
		}

	}
	Rect blockRect = boundingRect(Mat(contours[largest_contour_index]));

	Mat sub = Mat::zeros(Size(warp.cols, warp.rows), warp.type());
	Mat subRect(sub, blockRect);
	subRect.setTo(255);
	Mat roi;
	warp.copyTo(roi, sub);

	Mat gray;
	cvtColor(roi, gray, CV_BGR2GRAY);

	Mat cannyRoi;
	Canny(gray, cannyRoi, 10, 20);
	dilate(cannyRoi, cannyRoi, Mat(), Point(-1, -1), 1);
	imshow("Canny", cannyRoi);

	//find contours
	vector<vector<Point>> contours2;
	vector<Vec4i> hierarchy2;
	findContours(cannyRoi , contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	if (contours.size() <= 0) {
		cout << "No materials" << endl;
		waitKey(0);
		return;
	}

	Mat result = warp.clone();
	for (size_t i = 1; i < contours.size(); i++)
	{
		vector<Point> poly;
		approxPolyDP(contours[i], poly, 10, true);
		int err = 0;
		err = checkPoly(poly);
		if (err == 0) {
			count++;
			polylines(result, poly, true, Scalar(0, 0, 0), 2);
			RotatedRect area = minAreaRect(poly);
			if (poly.size() == 3) {
				putText(result, "T", area.center, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 0), 2);
			}
			else if (poly.size() == 4) {
				putText(result, "R", area.center, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 0), 2);
			}

		}
	}

	cout << "The number of polygons: " << count << endl;
	imshow("src", src);
	imshow("result", result);
	waitKey(0);
}



int main()
{
	countPolygon("Tangram/bird_warp.jpg");
}