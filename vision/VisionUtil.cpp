#include "VisionUtil.h"

VisionUtil::VisionUtil()
{	
}
VisionUtil::~VisionUtil()
{
}

CvMat* VisionUtil::CutGreenBackground(IplImage *img,CvMat *result){

	IplImage*   hsvImage = cvCreateImage(cvSize(img->width,img->height),8,3);
//	cvConvertImage(img,hsvImage,CV_BGR2HSV);
	//CvMat *result = cvCreateMat( img->height,img->width, CV_8U );

	for(int y=0;y<img->height;y++)
		for(int x=0;x<img->width;x++)		
		{

			uchar b = ((uchar*)(img->imageData + img->widthStep*y))[x*3];
			uchar g = ((uchar*)(img->imageData + img->widthStep*y))[x*3+1];
			uchar r = ((uchar*)(img->imageData + img->widthStep*y))[x*3+2];
			
			int color_difference = g-r;
			if(g-b<color_difference) color_difference = g-b;

			// RGB
			if( color_difference > 50 ) // R
				(result->data.ptr + result->step*y)[x] = 255; 	
			else
				(result->data.ptr + result->step*y)[x] = 0; 
			//// H
			//if( ( ((uchar*)(hsvImage->imageData + hsvImage->widthStep*y))[x*3] > 90 
			//	&& ((uchar*)(hsvImage->imageData + hsvImage->widthStep*y))[x*3] <= 140 )
			//	&& ((uchar*)(hsvImage->imageData + hsvImage->widthStep*y))[x*3+1] > 60 // S
			//	&& ((uchar*)(hsvImage->imageData + hsvImage->widthStep*y))[x*3+2] > 40 )// V 
			//	(result->data.ptr + result->step*y)[x] = 255; 	
			//else
			//	(result->data.ptr + result->step*y)[x] = 0; 
		}
	//cvNamedWindow("green");
	//cvShowImage("green",result);
	//cvSave("bgcut.xml",result);
	cvReleaseImage(&hsvImage);
	return result;
}