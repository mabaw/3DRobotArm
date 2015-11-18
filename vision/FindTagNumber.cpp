#include "FindTagNumber.h"


FindTagNumber::FindTagNumber()
{	
}
FindTagNumber::~FindTagNumber()
{
}

bool FindTagNumber::Initialize(){
	sthreshold	=	50;
	hlower		=	50;
	hupper		=	320;

	cvNamedWindow("red only");

	return true;
}



bool FindTagNumber::Execute(IplImage *image){
	//	convert and get only red pixel
	IplImage*	redPixel = cvCreateImage(cvSize(image->width,image->height),8,1);
	IplImage*   hsvImage = cvCreateImage(cvSize(image->width,image->height),8,3);
	cvConvertImage(image,hsvImage,CV_BGR2HSV);


	for(int y=0;y<image->height;y++)
		for(int x=0;x<image->width;x++)		
		{
			// H
			if( ( ((uchar*)(hsvImage->imageData + hsvImage->widthStep*y))[x*3] < hlower 
				|| ((uchar*)(hsvImage->imageData + hsvImage->widthStep*y))[x*3] >= hupper )
				&& ((uchar*)(hsvImage->imageData + hsvImage->widthStep*y))[x*3+1] > sthreshold 
				&& ((uchar*)(hsvImage->imageData + hsvImage->widthStep*y))[x*3+2] > 70 )// S 
				((uchar*)(redPixel->imageData + redPixel->widthStep*y))[x] = 0;			
			else
				((uchar*)(redPixel->imageData + redPixel->widthStep*y))[x] = 255;	
		}

	cvShowImage("red only",redPixel);

	cvReleaseImage(&redPixel);
	cvReleaseImage(&hsvImage);
	
	return true;
}
