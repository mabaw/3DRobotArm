#ifndef _CUBE_H_
#define _CUBE_H_

#include <cv.h>
#include <highgui.h>
#include <ctype.h>
#include <stdio.h>

#include <iostream>
#include <vector>
#include <string>

#include "Calibrate.h"
#include "VisionUtil.h"
#include "yMath\yMath.h"
#include "math.h"
#include "..\ArmConfig.h"

using namespace std;

class Cube {
	private:

	

	public:		
		Cube();
		~Cube();
		bool Initialize(float _sideWidth,float _size,CvPoint3D32f _origin,Calibrate *_calibrate,IplImage *_image,ArmConfig *_armConfig);		
		void ProjectBack(CvMat *rotaion,CvMat *translation,CvMat *cutBg,IplImage *img);
		void LoadCamera();
		void Execute();
		void Test(IplImage **img1,int size);
		void ResetCube(float _sideWidth,float _size,CvPoint3D32f _origin);

		int getStatus(int index);

		bool FindRealPosition(CvPoint3D32f point,CvPoint3D32f center,float angleZ,CvPoint3D32f *realPoint,int *index);
		CvPoint3D32f *pointList;
		int	*status; // 0 unknown ||  1 hasObject  || -1 dont have object
		int n_points;
		int numside;
		CvMat *cameraMatrix_inv;
		IplImage  *image;


		CvPoint3D32f origin; // front left of the cube
		float sideWidth;
		float size;
		CvPoint3D32f center;
		Calibrate *calibrate;
		ArmConfig  *armConfig;
		CvMat *intrinsic_matrix;
		CvMat *distortion_coeffs;
		CvMat *rotation,*translation;	
		CvMat *cutBgImg;

		CvMat *rotation_normal;
		CvMat *cameraMatrix;

		IplImage *draw,*white;
		VisionUtil visionUtil;


		CvMat *object_points;
		CvMat *image_points;
		int ccc;

		FILE *fAngle;
		bool useFile;
		float	angle[80];
};

#endif // _CUBE_H_