
#include <opencv.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

void main4()
{
	VideoCapture cap (1);
	Mat frame, gray, edge, prev, diff, thresh;
	bool stop = false;
	while(1){
		if(stop == false){
		   cap >> frame;
		   resize(frame, frame, Size(frame.size()));
		   if(frame.empty())
			   break;
		   cvtColor(frame,gray, CV_BGR2GRAY);
		   if(prev.empty())
			   gray.copyTo(prev);
		   absdiff(gray,prev,diff);
		   prev = gray.clone();
		   threshold(diff,thresh,100,255,THRESH_BINARY);
		   imshow("diff",diff);
		   imshow("org",gray);
		   imshow("thresh",thresh);
		}

		int key = waitKey(33);
        if(key == ' ')
			stop = !stop;
		else if (key == 27)
			break;
   }
}



void main3()
{
	Mat A;
	Mat B,C;

	A = imread("web.JPG");
	imshow("org",A);

	for (int i=1; i < 20; i += 2){
   
	   blur(A,B,Size(i,i));
	   bilateralFilter(A,C,i, i*2, i/2);
	   char name[100];
       sprintf(name, "burred %d", i);
	   imshow(name,B);
	   sprintf(name, "median %d", i);
	   imshow(name,C);  
     }
	 waitKey(0);
}

void main5()
{
	Mat A;
	Mat B;

	A = imread("flower1.jpg", 1);

	Size sz = A.size();
	sz = sz/8;
	resize(A, A, sz);

	//B = A + Scalar(100,100,100);
	
	vector<Mat> F(3);
	split(A, F);
	
	Mat gray;
	Mat gray3;
	Mat HSV;

	cvtColor(A, gray, COLOR_BGR2GRAY);
	cvtColor(gray, gray3, COLOR_GRAY2BGR);
	cvtColor(A, HSV, COLOR_BGR2HSV);
	Mat mask,mask1,  mask2, mask3;
	inRange(HSV, Scalar(100, 80, 80), Scalar(180, 255, 255), mask1);
	inRange(HSV, Scalar(0, 80, 80), Scalar(20, 255, 255), mask2);
	mask = mask1 | mask2;
	Mat morph_mask = getStructuringElement(MORPH_ELLIPSE,Size(5,5));
	morphologyEx(mask,mask,MORPH_OPEN,morph_mask);
	morphologyEx(mask,mask,MORPH_CLOSE,morph_mask);
	//inRange(HSV, Scalar(10, 80, 80), Scalar(30, 255, 200), mask);
	//imshow("mask1",mask1);
	//imshow("mask2",mask2);
	//imshow("mask", mask);
	//F[0] = F[0] & mask | (gray & ~mask);
	//F[1] = F[1] & mask | (gray & ~mask);
	//F[2] = F[2] & mask | (gray & ~mask);
	//merge(F, B);
	cvtColor(mask, mask3, COLOR_GRAY2BGR);
	B = (A & mask3) | (gray3 & ~mask3);
	
	

	String text = "test";
	putText(B,text,Point(20,30),FONT_HERSHEY_PLAIN,3.0, Scalar(0,255,100),3);


	char name[20];
	sprintf(name,"test%d",100);
	text = (String)name;
	putText(mask,text, Point(20,50),FONT_HERSHEY_SIMPLEX, 2.0, Scalar(255,255,255));
	line(B,Point(0,0),Point(B.cols, B.rows),Scalar(200,50,0),3);
	imshow("mask",mask);
	imshow("B",B);
    waitKey(0);
}