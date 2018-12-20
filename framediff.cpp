#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

void frame_MOG2(char * name) {

	VideoCapture cap(2);
	bool stop = false;
	if (!cap.isOpened()) {
		cout << "Error Device /File Cannot Open" << endl;
	}
	Mat frame, fgMOG2;
	Ptr<BackgroundSubtractorMOG2> pMOG2;
	pMOG2 = createBackgroundSubtractorMOG2(2, 16.0, false);
	//pMOG2->setVarThresholdGen(10);
	//pMOG2->setVarThreshold(30);
	//pMOG2->setNMixtures(10);

	while (1) {
		if (!stop) {
			cap >> frame;
			if (frame.empty())
				break;
			//update the background model
			pMOG2->apply(frame, fgMOG2, 0.1);
		}
		
		threshold(fgMOG2, fgMOG2, 50, 255, THRESH_BINARY);

		//get the frame number and write it on the current frame
		/*stringstream ss;
		rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
			cv::Scalar(255, 255, 255), -1);
		ss << cap.get(CAP_PROP_POS_FRAMES);
		string frameNumberString = ss.str();
		putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
			FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0)); */
		//show the current frame and the fg masks
		imshow("Frame", frame);
		imshow("FG MOG2", fgMOG2);
		//get the input from the keyboard
		int keyboard = waitKey(30);
		if (keyboard == 27)
			break;
		else if (keyboard == ' ')
			stop = !stop;
	}
	cap.release();
	destroyAllWindows();
}