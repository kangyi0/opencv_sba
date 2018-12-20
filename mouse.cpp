#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

static char *win_name = "Display";
static char *msg_win = "msg";
Mat canvas;
//vector<Mat> Mptr(2);
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	Mat *Mptr = (Mat *) userdata;

	Mat bgr = Mptr[0];
	Mat hsv = Mptr[1];
	vector<Mat> bgrCh(3) , hsvCh(3);
	split(bgr, bgrCh);
	split(hsv, hsvCh);
	char msg[50] , msg2[50];

	sprintf(msg, "H=%3d, S=%3d V=%3d",
		hsvCh[0].at<uchar>(y, x), hsvCh[1].at<uchar>(y, x), hsvCh[2].at<uchar>(y, x));
	sprintf(msg2, "B=%3d, G=%3d R=%3d",
		bgrCh[0].at<uchar>(y, x), bgrCh[1].at<uchar>(y, x), bgrCh[2].at<uchar>(y, x));

	if (event == EVENT_LBUTTONDOWN)
	{
		std::cout << "Left Click at (" << x << ", " << y << ") ";
		cout << msg <<endl;
	}
	else if (event == EVENT_RBUTTONDOWN)
	{
		std::cout << "Right Click at (" << x << ", " << y << ") ";

		cout << msg2 << endl;
	}
	else {
		
	}

	canvas = Scalar(0, 0, 0);

	putText(canvas, msg, Point(10,20), 1.2, FONT_HERSHEY_PLAIN, Scalar(20, 255, 100), 2);
	putText(canvas, msg2, Point(10,40), 1.2, FONT_HERSHEY_PLAIN, Scalar(0, 100, 180), 2);
	imshow(msg_win, canvas);
}

void mouse_event(char *name)
{
	cv::Mat img = cv::imread(name,1);

	/// if fail to read the image
	if (img.empty()) {
		std::cout << "Error loading the image" << std::endl;
		return ;
	}
	Mat Marray[2];
	Marray[0] = img.clone();
	cvtColor(img, Marray[1], COLOR_BGR2HSV);
	cv::namedWindow(win_name, 1);

	/// set the callback function for any mouse event
	cv::setMouseCallback(win_name, CallBackFunc, Marray);

	cv::imshow(win_name, img);
	
	canvas = Mat::zeros(Size(400, 80), CV_8UC3);
	imshow(msg_win, canvas);

	waitKey(0);
	destroyAllWindows();
}