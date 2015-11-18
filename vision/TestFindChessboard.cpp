#include <stdio.h>
#include "TestFindChessboard.h"
#include <iostream>

#include "cv.h"
#include "cxmisc.h"
#include "highgui.h"
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>
using namespace std;

TestFindChessboard::TestFindChessboard()
{	
}
TestFindChessboard::~TestFindChessboard()
{
}

void TestFindChessboard::FindChessboard(){
	IplImage* img1 = cvLoadImage( "1.jpg", 0 );	 
	IplImage* img2 = cvLoadImage( "2.jpg", 0 );
	IplImage* img3 = cvLoadImage( "3.jpg", 0 );

	cvNamedWindow("c1");
	cvNamedWindow("c2");
	cvNamedWindow("c3");
	cvResizeWindow("c1",300,300);
	int num_points = 15;
	CvPoint2D32f* corners = new CvPoint2D32f[num_points];
	CvSize pattern_size = cvSize(5,3);
	int corner_count;


	int found = cvFindChessboardCorners(img1, pattern_size, corners, &corner_count);		
	cvFindCornerSubPix(img1, corners, corner_count, cvSize(11,11), cvSize(-1,-1),
								cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));
	cvDrawChessboardCorners(img1,pattern_size,corners,corner_count,found);
	

	cvShowImage("c1",img1);

	cvWaitKey(0);

	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	cvReleaseImage(&img3);

}
