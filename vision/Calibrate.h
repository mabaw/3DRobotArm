#ifndef _CALI_H_
#define _CALI_H_



#include "yMath\yMath.h"
#include "cv.h"
#include "cxmisc.h"
#include "highgui.h"
#include "CameraManager.h"
#define M_PI 3.14159265358979323846


struct POINTINT {
	int x;
	int y;
	int z;
};

class Calibrate {
	private :
		CameraManager *cameraMan;
		IplImage *collectImg[20];



		IplImage* gray_image,*image2;
		CvMat *image_points; 
		CvMat *image_points2;
		CvMat *object_points;
		CvPoint2D32f* corners;
		int num_points;


	public:		
		

		Calibrate();
		~Calibrate();
		bool Initialize(CameraManager *_cameraMan);
		void TestCalibrate();
		bool CalibrateCamera();
		bool CalibrateLiveCamera(int numImg);
		bool FindExtrinsicFromFile(CvMat *rotation,CvMat *tramslation);
		bool FindExtrinsic(CvMat *intrinsic_matrix,CvMat *distortion_coeffs,CvMat *rotation,CvMat *translation,IplImage *image);
		void ShowImageFromProjection(POINTINT* point,int numpoint,Matrix44f camaratip,mVector3f position,Matrix44f allMat);

};

#endif // _ARMCAL_H_