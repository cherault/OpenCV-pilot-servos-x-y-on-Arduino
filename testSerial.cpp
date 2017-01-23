/*
 * testSerial.cpp
 *
 *  Created on: 18 janv. 2017
 *      Author: tux
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>

#define PORT "/dev/ttyUSB0" //define serial port (The same as Arduino)

using namespace std;
using namespace cv;

//global variables
int pan, tilt;
Mat frame;

void moveMouse(int, int, int, int, void*);

int main()
{
	int outMax = 180; //max servo range (0-180°)
	int servoPan, servoTilt;

	//open serial port in file
	fstream fd;
	fd.open(PORT);

	VideoCapture cap(0);

	while(true)
	{
		cap >> frame;

		setMouseCallback("Frame", moveMouse, NULL);

		Point pt1 = Point(0, frame.rows/2);
		Point pt2 = Point(frame.cols, frame.rows/2);
		Point pt3 = Point(frame.cols/2, 0);
		Point pt4 = Point(frame.cols/2, frame.rows);
		line(frame, pt1, pt2, Scalar(0,0,255),1);
		line(frame, pt3, pt4, Scalar(0,0,255),1);

		line(frame, Point(pan, 0), Point(pan, frame.rows), Scalar(0,255,0), 1);
		line(frame, Point(0, tilt), Point(frame.cols, tilt), Scalar(0,255,0), 1);
		circle(frame, Point(pan, tilt), 2, Scalar(0,255,0),2);

		//convert int to string
		stringstream x, y, p, t;
		x << pan;
		y << tilt;
		p << servoPan;
		t << servoTilt;

		//scaling the range for the servos 0-180°
		servoPan = (pan * outMax + 1)/(frame.cols + 1);
		servoTilt = (tilt * outMax + 1)/(frame.rows + 1);

		int servoRefX, servoRefY;

		//scaling the range for the servos 0-180°
		servoRefX = (outMax/2 - servoPan);
		servoRefY = (outMax/2 - servoTilt);

		stringstream refX, refY;
		refX << servoRefX;
		refY << servoRefY;

		//print infos on screen
		putText(frame, "Offset X : ", Point(10,120),1,1,Scalar(255,255,255),1);
		putText(frame, refX.str() , Point(100,120),1,1,Scalar(255,255,255),1);
		putText(frame, "Offset Y : ", Point(10,140),1,1,Scalar(255,255,255),1);
		putText(frame, refY.str() , Point(100,140),1,1,Scalar(255,255,255),1);

		line(frame, Point(frame.cols/2, frame.rows/2), Point(pan, tilt), Scalar(255,0,0),3);

		putText(frame, "X : ", Point(10,20),1,1,Scalar(255,255,255),1);
		putText(frame, x.str() , Point(40,20),1,1,Scalar(255,255,255),1);

		putText(frame, "Y : ", Point(10,40),1,1,Scalar(255,255,255),1);
		putText(frame, y.str() , Point(40,40),1,1,Scalar(255,255,255),1);

		putText(frame, "servo PAN : ", Point(10,60),1,1,Scalar(255,255,255),1);
		putText(frame, p.str() , Point(110,60),1,1,Scalar(255,255,255),1);

		putText(frame, "servo TILT : ", Point(10,80),1,1,Scalar(255,255,255),1);
		putText(frame, t.str() , Point(110,80),1,1,Scalar(255,255,255),1);

		/*sending values on serial port to Arduino
		 *  X value + "x"
		 *  Y value + "y"
		 */
		fstream fd;
		fd.open(PORT);
		fd << t.str() << "x" << endl;
		fd << p.str() << "y" << endl;
		fd.close();

		imshow("Frame", frame);
		waitKey(32);
	}
	return 0;
}

void moveMouse(int event, int x, int y, int flags, void* userdata)
{
	if(event == EVENT_MOUSEMOVE)
    {
		pan = x;
		tilt = y;
    }
}


