#ifndef _CAMERA_MANAGER_H_
#define _CAMERA_MANAGER_H_


#include <stdio.h>
#include "yMath\yMath.h"
#include "cv.h"
#include "cxmisc.h"
#include "highgui.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>

using namespace std;
class CameraManager {
	public:		
		
		IplImage *image;
		CvCapture	*capture;


		CameraManager();
		~CameraManager();
		bool Initialize(); 
		bool GrabImage();
		IplImage* getCurrentImage();
		
};

#endif // _TEST_FIND_CHESS_H_