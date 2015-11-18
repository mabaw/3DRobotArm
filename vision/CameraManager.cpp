
#include "CameraManager.h"


CameraManager::CameraManager()
{	
	image = cvCreateImage(cvSize(640,480),8,3);
}
CameraManager::~CameraManager()
{
	cvReleaseCapture(&capture);
}

bool CameraManager::Initialize(){
	capture = cvCaptureFromCAM(0); // capture from video device #0
	//For NSC : Clean up
	//cvNamedWindow("capture");
	return true;
}

bool CameraManager::GrabImage(){
	if(!cvGrabFrame(capture)){              // capture a frame 
	 // printf("Could not grab a frame\n");
	  return false;
	}
	//if(image)
	//	cvReleaseImage(&image);
	image=cvRetrieveFrame(capture);           // retrieve the captured frame
	//For NSC : Clean up
	//cvShowImage("capture",image);
	return true;
}


IplImage* CameraManager::getCurrentImage(){
	IplImage *cloneImg = cvCloneImage(image);
	return cloneImg;
}