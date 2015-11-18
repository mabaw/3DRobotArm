#ifndef _VISION_UTIL_H_
#define _VISION_UTIL_H_

#include <cv.h>
#include <highgui.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <string>

class VisionUtil {
	private:
	
	public:		
		

		VisionUtil();
		~VisionUtil();
		CvMat* CutGreenBackground(IplImage *img,CvMat *result);

};

#endif // _VISION_UTIL_H_