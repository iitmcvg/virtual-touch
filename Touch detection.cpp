// Touch detection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv\highgui.h>
#include "opencv2/imgproc/imgproc.hpp"

#define STORAGE_SIZE 10

using namespace cv;

int Hmin = 140, Hmax = 187, Smin = 80, Smax = 255, Vmin = 108, Vmax = 249;	//Pink
//int Hmin = 0, Hmax = 255, Smin = 0, Smax = 255, Vmin = 0, Vmax = 255;	    //All

Mat bgr, hsv, dst;

int i, j, n, k = 0;

Point CurPos;		// -1: Not in use;		-2: Not detected;


class Finger
{
public:
	Point pos;
	float vx, vy, t;
	Finger *previous, *next;

	Finger()
	{
		pos.x = pos.y = -1;
		vx = vy = 0;
		t = 1;
		previous = next = NULL;
	}
	void Init(Point pt, Finger *&prev)
	{
		pos = pt;
		previous = prev;
		prev->next = this;
		next = NULL;
		if (prev->pos.x != -1)
		{
			vx = pt.x - prev->pos.x;
			vy = pt.y - prev->pos.y;
		}
		else
		{
			vx = vy = 0;
		}
	}

	void Calculate(Point pt, Finger *&prev, Finger *&last)
	{
		pos = pt;
		previous = prev;
		prev->next = this;
		next = NULL;
		if (prev->pos.x != -1)
		{
			vx = (pt.x - prev->pos.x) / prev->t;
			vy = (pt.y - prev->pos.y) / prev->t;
		}
		else
		{
			vx = vy = 0;
		}


		last = last->next;
		delete last->previous;
		last->previous=NULL;
		
	}
}*cur,*last;



void FingerInit()
{
	Point pt(-1,-1);
	cur = new Finger;
	last = cur;
	for (int i = 0; i < STORAGE_SIZE - 1; i++)
	{
		Finger *temp = new Finger;
		temp->Init(pt, cur);
		//cur->next = temp;
		cur = temp;
	}
}

void DetectFInger()
{

	cvtColor(bgr, hsv, CV_BGR2HSV);
	inRange(hsv, Scalar(Hmin, Smin, Vmin), Scalar(Hmax, Smax, Vmax), dst);
	erode(dst, dst, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(dst, dst, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	k++;
	std::cout << "\n" << k;


	n = 0;
	for (i = 0; i < dst.rows; i++)
	{
		for (j = 0; j < dst.cols; j++)
			if (dst.at<uchar>(i, j) == 255)
			{
				n++; CurPos.x += j; CurPos.y += i;
			}
	}
	if (n >= 10)
	{
		CurPos.x = (float)CurPos.x / n;
		CurPos.y = (float)CurPos.y / n;
		circle(bgr, CurPos, 15, Scalar(0, 255, 255), 2);
	}
	else
	{
		CurPos.x = -2; CurPos.y = -2;
	}
}

int main()
{
	FingerInit();
	VideoCapture cam(0);
	if (!cam.isOpened())
		return -1;
	while (1)
	{
		cam >> bgr;
		DetectFInger();
		if (CurPos.y == -2)
		{
			if (cur->t > 10)
				cur->t = 1;
			else
				cur->t++;
		}
		else
		{
			Finger *temp = new Finger;
			temp->Calculate(CurPos, cur, last);
			//cur->next = temp;
			cur = temp;
		}
		std::cout << " x=" << cur->pos.x << " y=" << cur->pos.y <<" vy=" << cur->vy << " t=" << cur->t;




		imshow("Input", bgr);
		imshow("Output", dst);

		if (waitKey(30) > 0)
			break;
	}


    return 0;
}

