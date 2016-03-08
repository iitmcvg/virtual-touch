// Touch detection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv\highgui.h>
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

int Hmin = 140, Hmax = 187, Smin = 80, Smax = 255, Vmin = 108, Vmax = 249;	//Pink
//int Hmin = 0, Hmax = 255, Smin = 0, Smax = 255, Vmin = 0, Vmax = 255;	

Mat bgr, hsv, dst;

int i, j, n,k=0;
Point pt;


int main()
{
	VideoCapture cam(0);
	if (!cam.isOpened())
		return -1;
	while (1)
	{
		cam >> bgr;
		cvtColor(bgr, hsv, CV_BGR2HSV);
		inRange(hsv, Scalar(Hmin, Smin, Vmin), Scalar(Hmax, Smax, Vmax), dst);
		erode(dst, dst, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(dst, dst, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		k++;
		std::cout << k << "\n";


		n = 0;
		for (i = 0; i < dst.rows; i++)
		{
			for (j = 0; j < dst.cols; j++)
				if (dst.at<uchar>(i, j) == 255)
				{
					n++; pt.x += j; pt.y += i;
				}
		}
		if (n >= 10)
		{
			pt.x = (float)pt.x / n;
			pt.y = (float)pt.y / n;
			circle(bgr, pt, 15, Scalar(0, 255, 255),2);
		}
		else
		{
			pt.x = -1; pt.y = -1;
		}

		imshow("Input", bgr);
		imshow("Output", dst);

		if (waitKey(30) > 0)
			break;
	}


    return 0;
}

