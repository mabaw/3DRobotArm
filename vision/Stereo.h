#ifndef _STER_H_
#define _STER_H_

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


class Stereo {
	public:		
		

		IplImage *image1;
		IplImage *image2;
		CvMat	 *pointCloud;
		CvMat	 *pointRefine;

		Stereo();
		~Stereo();
		bool Initialize();
		bool Execute();
		
		void collectImage1(IplImage* _image); 
		void collectImage2(IplImage* _image); 
		void takeNewImage();
		void copyImage();
		void findMatchPoint(IplImage *image1,IplImage *image2);
		void calibrate();
		void reproject();
		void LoadReprojectImage();
		void LoadDisparity();
		void SMCorrespondence(CvMat *img1,CvMat *img2, CvMat *disp1, CvMat *disp2);
		void SMProc(CvMat* img1r,CvMat* img2r,CvMat* dispLeft,CvMat* dispRight,CvMat* vdisp);
		void BMProc(CvMat* img1r,CvMat* img2r,CvMat* disp,CvMat* vdisp);
		void GCProc(CvMat* img1r,CvMat* img2r,CvMat* dispLeft,CvMat* dispRight,CvMat* vdisp);

		//void mergePoint();

		private :
			double error[700][700];
			int path[700][700];
			int countskip[700][700];


};

#endif // _STER_H_