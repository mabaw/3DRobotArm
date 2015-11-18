#include <stdio.h>
#include "ChessboardPlate.h"
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

ChessboardPlate::ChessboardPlate()
{	
}
ChessboardPlate::~ChessboardPlate()
{
}

void ChessboardPlate::FindChessboard(IplImage *img){
	cvNamedWindow("img");
	int num_points = 5;
	CvPoint2D32f* corners = new CvPoint2D32f[num_points];
	CvSize pattern_size = cvSize(3,2);
	int corner_count;


	int found = cvFindChessboardCorners(img, pattern_size, corners, &corner_count);		
	cvFindCornerSubPix(img, corners, corner_count, cvSize(11,11), cvSize(-1,-1),
								cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));
	cvDrawChessboardCorners(img,pattern_size,corners,corner_count,found);
	cvShowImage("img",img);
	cvReleaseImage(&img);

}
