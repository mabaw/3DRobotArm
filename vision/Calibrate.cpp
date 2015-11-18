#include <stdio.h>
#include "Calibrate.h"
#include <iostream>

using namespace std;

Calibrate::Calibrate()
{	
}
Calibrate::~Calibrate()
{
}

bool Calibrate::Initialize(CameraManager *_cameraMan){
	cameraMan = _cameraMan;

	num_points = 48;
	image_points = cvCreateMat(num_points, 2, CV_32FC1);
	image_points2 = cvCreateMat(num_points, 2, CV_32FC1);
	object_points = cvCreateMat(num_points, 3, CV_32FC1);
	corners = new CvPoint2D32f[num_points];

	image2 = cvCreateImage(cvGetSize(cameraMan->image),8,3);

	return true;
}

void Calibrate::ShowImageFromProjection(POINTINT* point,int numpoint,Matrix44f camaratip, mVector3f position,Matrix44f allMat){


	Matrix44f invert;
	Matrix44f::Invert(&invert, &allMat);

	mVector3f pos1;// = mVector3(0.0f,0.0f,0.0f);



	CvMat *intrinsic_matrix = (CvMat*)cvLoad("Intrinsics.xml");
	CvMat *distortion_coeffs = (CvMat*)cvLoad("Distortion.xml");

	int num_points = 5;
	numpoint = 5;

	CvMat *image_points2 = cvCreateMat(num_points, 2, CV_32FC1); // output ??
	CvMat *object_points = cvCreateMat(num_points, 3, CV_32FC1);
	CvMat *rotation = cvCreateMat(3, 3, CV_32FC1);
	CvMat *rodrigue = cvCreateMat(1, 3, CV_32FC1);
	CvMat *translation = cvCreateMat(1,3,CV_32FC1);

	//cout << "xxxxxxxxxxxxx" << endl;
	CV_MAT_ELEM(*translation, float, 0, 0) = 0;//camaratip.m14;
	CV_MAT_ELEM(*translation, float, 0, 1) = 0;//camaratip.m24;
	CV_MAT_ELEM(*translation, float, 0, 2) = 0;//camaratip.m34;

	//cout << "transaltion ... " << endl;
	//cout << CV_MAT_ELEM(*translation,float,0,0) << " ";
	//cout << CV_MAT_ELEM(*translation,float,0,1) << " ";
	//cout << CV_MAT_ELEM(*translation,float,0,2) << " " <<endl<<endl; 

	// red point #1
	pos1 = invert * mVector3f(0.0f,0.0f,0.0f);
	CV_MAT_ELEM(*object_points, float, 0, 0) = pos1.x;
	CV_MAT_ELEM(*object_points, float, 0, 1) = pos1.y;
	CV_MAT_ELEM(*object_points, float, 0, 2) = pos1.z;
	
	// red point #2
	CV_MAT_ELEM(*object_points, float, 1, 0) = 20.4f  - position.x;
	CV_MAT_ELEM(*object_points, float, 1, 1) = 0.0f	 - position.y;
	CV_MAT_ELEM(*object_points, float, 1, 2) = 0.0f	 - position.z;
	
	// red point #3
	
	CV_MAT_ELEM(*object_points, float, 2, 0) = 39.7f - position.x;
	CV_MAT_ELEM(*object_points, float, 2, 1) = 0.0f - position.y;
	CV_MAT_ELEM(*object_points, float, 2, 2) = 0.0f - position.z;

		
	// red point #4
	CV_MAT_ELEM(*object_points, float, 3, 0) = 39.7f  - position.x;
	CV_MAT_ELEM(*object_points, float, 3, 1) = 20.5f - position.y;
	CV_MAT_ELEM(*object_points, float, 3, 2) = 0.0f	 - position.z;

		
	// red point #5
	CV_MAT_ELEM(*object_points, float, 4, 0) = 39.7f  - position.x;
	CV_MAT_ELEM(*object_points, float, 4, 1) = 40.1f - position.y;
	CV_MAT_ELEM(*object_points, float, 4, 2) = 0.0f	 - position.z;

	//cvRodrigues2(rotation,rodrigue);
	
	CV_MAT_ELEM(*rodrigue,float,0,0) = 0;
	CV_MAT_ELEM(*rodrigue,float,0,1) = 0;
	CV_MAT_ELEM(*rodrigue,float,0,2) = 0;
  
	cvProjectPoints2(object_points,rodrigue,translation,intrinsic_matrix,distortion_coeffs,image_points2);
	

	//cout << "rodrigue >> " << endl;
	//cout << CV_MAT_ELEM(*rodrigue,float,0,0) <<endl; 
	//cout << CV_MAT_ELEM(*rodrigue,float,0,1) <<endl; 
	//cout << CV_MAT_ELEM(*rodrigue,float,0,2) <<endl; 

	//for(int i=0;i<numpoint;i++)
	//{
	//	cout << "(" << CV_MAT_ELEM(*image_points2,float,i,0) << ","; 
	//	cout << CV_MAT_ELEM(*image_points2,float,i,1) << ")"<< endl;
	//}

	IplImage* img = 0; 
	CvCapture* capture = cvCaptureFromCAM(0); // capture from video device #0
	//cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,1280);
	//cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,1024);
	if(!cvGrabFrame(capture)){              // capture a frame 
	  printf("Could not grab a frame\n\7");
	  exit(0);
	}
	img=cvRetrieveFrame(capture);           // retrieve the captured frame
	cout << "Image size = " << img->width << " x " << img->height << endl;
	cout << " num point = " << numpoint << endl;
	//for(int i=0;i<numpoint;i++)
	//{
	int i=0;
		int x = CV_MAT_ELEM(*image_points2,float,i,0) ;
		int y = CV_MAT_ELEM(*image_points2,float,i,1);
		cout << "x = " << x << " : y = " << y << endl; 

		if(x>=0&&x<1280&&y>=0&&y<1024){
			cout << "draw" << i << "," << (((float)x/1280.0f)*640.0f) << "," << (((float)y/1024.0f)*480.0f) << endl;
			CvScalar color=cvScalar(0xff,0x00,0x00);
			//if(i==1)
			//	color=cvScalar(0x00,0xff,0x00);
			//else if(i==2)
			//	color=cvScalar(0x00,0x00,0xff);
			//else if(i==3)
			//	color=cvScalar(0xff,0xff,0x00);
			//else if(i==4)
				color=cvScalar(0xff,0x00,0xff);
			//cvDrawCircle(img,cvPoint(x,y),3,color,3);	
			cvDrawCircle(img,cvPoint(((float)x/1280.0f)*img->width,((float)y/1024.0f)*img->height),3,color,3);	
		}
	//}
	//For NSC : Clean up
	//cvShowImage("capture",img);
	cvReleaseCapture(&capture);
	return;
}



void Calibrate::TestCalibrate(){
	int corner_count;
	int num_points = 24;
	IplImage* gray_image;
	CvMat *image_points = cvCreateMat(num_points, 2, CV_32FC1);
	CvMat *image_points2 = cvCreateMat(num_points, 2, CV_32FC1);
	CvMat *object_points = cvCreateMat(num_points, 3, CV_32FC1);
	CvMat *rotation = cvCreateMat(3, 1, CV_32FC1);
	CvMat *translation = cvCreateMat(3, 1, CV_32FC1);
	CvPoint2D32f* corners = new CvPoint2D32f[num_points];
	CvSize pattern_size = cvSize(6,4);
	
	cvNamedWindow("img");
	IplImage* img = 0; 
	CvMat *intrinsic_matrix = (CvMat*)cvLoad("IntrinsicsLive.xml");
	CvMat *distortion_coeffs = (CvMat*)cvLoad("DistortionLive.xml");

	cameraMan->GrabImage();
	img = cameraMan->image;
	cvShowImage("capture",img);

	while(true)
	{
		int found = cvFindChessboardCorners(img, pattern_size, corners, &corner_count);		
		if(found!=0)
		{
			cout << "found." << endl;
			gray_image = cvCreateImage(cvGetSize(img), 8, 1);
			cvCvtColor(img, gray_image, CV_BGR2GRAY);
			cvFindCornerSubPix(gray_image, corners, corner_count, cvSize(11,11), cvSize(-1,-1),
								cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));
			//cvDrawChessboardCorners(img,pattern_size,corners,corner_count,found);

			for (int j = 0, k = 0; k < num_points; j++,k++) {
				//cvDrawCircle(img,cvPoint(corners[k].x,corners[k].y),3,cvScalar(255,0,0,0),3);
				CV_MAT_ELEM(*image_points, float, j, 0) = corners[k].x;
				CV_MAT_ELEM(*image_points, float, j, 1) = corners[k].y;
				CV_MAT_ELEM(*object_points, float, j, 0) = (k%pattern_size.width)*0.73;
				CV_MAT_ELEM(*object_points, float, j, 1) = (k/pattern_size.width)*0.73;
				CV_MAT_ELEM(*object_points, float, j, 2) = 0.0f;
			}
			cvFindExtrinsicCameraParams2(object_points,image_points,intrinsic_matrix,distortion_coeffs,rotation,translation);
			cvProjectPoints2(object_points,rotation,translation,intrinsic_matrix,distortion_coeffs,image_points2);

			for(int i=0;i<num_points;i++)
			{
				int x = CV_MAT_ELEM(*image_points2, float, i, 0);
				int y = CV_MAT_ELEM(*image_points2, float, i, 1);
			//	cout << x << " " << y << endl;
				cvDrawCircle(img,cvPoint(x,y),2,cvScalar(0x0,0x00,0xff),3);

			}
			cvShowImage("img",img);
			break;

		}
		else
			cout << "not found." << endl;	
		cameraMan->GrabImage();
		img = cameraMan->image;	
		cvShowImage("capture",img);
		cvWaitKey(5);

	}
}

bool Calibrate::CalibrateCamera(){
		char* images[12] = { "img_01.jpg", "img_02.jpg", "img_03.jpg",
		"img_04.jpg", "img_05.jpg", "img_06.jpg", "img_07.jpg",
		"img_08.jpg", "img_09.jpg", "img_10.jpg", "img_11.jpg","img_12.jpg" };



	int images_len = 12;
	int num_points = 48;
	CvSize pattern_size = cvSize(6,8);

	CvMat *image_points = cvCreateMat(images_len*num_points, 2, CV_32FC1);
	CvMat *object_points = cvCreateMat(images_len*num_points, 3, CV_32FC1);
	CvMat *point_counts = cvCreateMat(images_len,1,CV_32SC1);
	int corner_count;
	CvPoint2D32f* corners = new CvPoint2D32f[num_points];
	IplImage* img;
	IplImage* img2;
	IplImage* gray_image;


	cvNamedWindow("img2");
	for (int i = 0; i < images_len; i++) {
		
		img = cvLoadImage(images[i]);
		img2 = cvCreateImage(cvSize(480, 360), 8, 3);
		gray_image = cvCreateImage(cvGetSize(img), 8, 1);
		int found = cvFindChessboardCorners(img, pattern_size, corners, &corner_count);		
		cvDrawChessboardCorners(img,pattern_size,corners,corner_count,found);

		cvResize(img, img2);
		cout << corner_count << endl;
		cvResizeWindow("img2",480,360);
		cvShowImage("img2",img2);
		
		if(found!=0)
		{
			cout << "found." << endl;
			cvDrawChessboardCorners(img,pattern_size,corners,corner_count,found);
			cvCvtColor(img, gray_image, CV_BGR2GRAY);
			cvFindCornerSubPix(gray_image, corners, corner_count, cvSize(11,11), cvSize(-1,-1),
								cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));
				
			
			for (int j = i*num_points, k = 0; k < num_points; j++,k++) {
				//cvDrawCircle(img,cvPoint(corners[k].x,corners[k].y),3,cvScalar(255,0,0,0),3);
				CV_MAT_ELEM(*image_points, float, j, 0) = corners[k].x;
				CV_MAT_ELEM(*image_points, float, j, 1) = corners[k].y;
				CV_MAT_ELEM(*object_points, float, j, 0) = (k/pattern_size.width)*0.73;
				CV_MAT_ELEM(*object_points, float, j, 1) = (k%pattern_size.width)*0.73;
				CV_MAT_ELEM(*object_points, float, j, 2) = 0.0f;
			}
			cvShowImage("img",img);
			cvWaitKey(0);
			cout << i << endl;
			CV_MAT_ELEM(*point_counts, int, i, 0) = num_points;
		}
		else
			cout << "not found." << endl;
		//cvWaitKey(0);
	}
	CvMat *intrinsic_matrix = cvCreateMat(3,3,CV_32FC1);
	CvMat *distortion_coeffs = cvCreateMat(4,1,CV_32FC1);
	CV_MAT_ELEM(*intrinsic_matrix, float, 0, 0) = 1.0f;
	CV_MAT_ELEM(*intrinsic_matrix, float, 1, 1) = 1.0f;
	cvCalibrateCamera2(object_points, image_points, point_counts, 
						cvGetSize(img), intrinsic_matrix, distortion_coeffs,
						NULL, NULL, 0);								
	cvSave("Intrinsics.xml", intrinsic_matrix);
	cvSave("Distortion.xml", distortion_coeffs);
	cvWaitKey(0);
	



	return true;
}


bool Calibrate::CalibrateLiveCamera(int numImg){
	int num_points = 24;
	CvSize pattern_size = cvSize(6,4);

	CvMat *image_points = cvCreateMat(numImg*num_points, 2, CV_32FC1);
	CvMat *object_points = cvCreateMat(numImg*num_points, 3, CV_32FC1);
	CvMat *point_counts = cvCreateMat(numImg,1,CV_32SC1);
	int corner_count;
	CvPoint2D32f* corners = new CvPoint2D32f[num_points];
	IplImage* img;
	IplImage* img2;
	IplImage* gray_image;
	CvSize imageSize = cvSize(cameraMan->image->width,cameraMan->image->height);

	
	cvNamedWindow("img2");
	int i=-1; // number of good chessboard got
	while(i<numImg-1) {
		cameraMan->GrabImage();
		//cvWaitKey(10);
		img = cvCloneImage(cameraMan->image);
		img2 = cvCreateImage(cvSize(480, 360), 8, 3);
		cvResize(img, img2);
		cvResizeWindow("img2",480,360);
		cvShowImage("img2",img2);
		cvWaitKey(5);
		int found = cvFindChessboardCorners(img, pattern_size, corners, &corner_count);	\
		cout << corner_count << endl;
		if(found ==0) 
		{
			cvReleaseImage(&img);
			cvReleaseImage(&img2);
			continue;
		}
		gray_image = cvCreateImage(cvGetSize(img), 8, 1);
		i++;
		if(found!=0)
		{
			cvCvtColor(img, gray_image, CV_BGR2GRAY);
			cvFindCornerSubPix(gray_image, corners, corner_count, cvSize(11,11), cvSize(-1,-1),
								cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));
			cvDrawChessboardCorners(img,pattern_size,corners,corner_count,found);	
		
			for (int j = i*num_points, k = 0; k < num_points; j++,k++) {
				CV_MAT_ELEM(*image_points, float, j, 0) = corners[k].x;
				CV_MAT_ELEM(*image_points, float, j, 1) = corners[k].y;
				CV_MAT_ELEM(*object_points, float, j, 0) = (k%pattern_size.width)*0.73;
				CV_MAT_ELEM(*object_points, float, j, 1) = (k/pattern_size.width)*0.73;
				CV_MAT_ELEM(*object_points, float, j, 2) = 0.0f;
			}
			cvShowImage("img",img);
			cvWaitKey(0);
			CV_MAT_ELEM(*point_counts, int, i, 0) = num_points;
		}
		cvReleaseImage(&img);
		cvReleaseImage(&img2);
		cvReleaseImage(&gray_image);
	}
	CvMat *intrinsic_matrix = cvCreateMat(3,3,CV_32FC1);
	CvMat *distortion_coeffs = cvCreateMat(4,1,CV_32FC1);
	CV_MAT_ELEM(*intrinsic_matrix, float, 0, 0) = 1.0f;
	CV_MAT_ELEM(*intrinsic_matrix, float, 1, 1) = 1.0f;
	cvCalibrateCamera2(object_points, image_points, point_counts, 
						imageSize, intrinsic_matrix, distortion_coeffs,
						NULL, NULL, 0);								
	cvSave("IntrinsicsLive.xml", intrinsic_matrix);
	cvSave("DistortionLive.xml", distortion_coeffs);
	return true;
}

bool Calibrate::FindExtrinsicFromFile(CvMat *rotation,CvMat *translation){
	int corner_count;
	int num_points = 48;
	IplImage* gray_image;
	CvMat *image_points = cvCreateMat(num_points, 2, CV_32FC1);
	CvMat *image_points2 = cvCreateMat(num_points, 2, CV_32FC1);
	CvMat *object_points = cvCreateMat(num_points, 3, CV_32FC1);
	rotation = cvCreateMat(3, 1, CV_32FC1);
	translation = cvCreateMat(3, 1, CV_32FC1);
	CvPoint2D32f* corners = new CvPoint2D32f[num_points];
	CvSize pattern_size = cvSize(6,8);
	//For NSC : Clean up
	//cvNamedWindow("img");
	IplImage* img = 0; 

	cameraMan->GrabImage();
	img = cameraMan->getCurrentImage();

	CvMat *intrinsic_matrix = (CvMat*)cvLoad("IntrinsicsLive.xml");
	CvMat *distortion_coeffs = (CvMat*)cvLoad("DistortionLive.xml");

	int found = cvFindChessboardCorners(img, pattern_size, corners, &corner_count);		
	if(found!=0)
		{			
			gray_image = cvCreateImage(cvGetSize(img), 8, 1);
			cvCvtColor(img, gray_image, CV_BGR2GRAY);
			cvFindCornerSubPix(gray_image, corners, corner_count, cvSize(11,11), cvSize(-1,-1),
								cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));				
			cvDrawChessboardCorners(img,pattern_size,corners,corner_count,found);
			for (int j = 0, k = 0; k < num_points; j++,k++) {
				//cvDrawCircle(img,cvPoint(corners[k].x,corners[k].y),3,cvScalar(255,0,0,0),3);
				CV_MAT_ELEM(*image_points, float, j, 0) = corners[k].x;
				CV_MAT_ELEM(*image_points, float, j, 1) = corners[k].y;
				CV_MAT_ELEM(*object_points, float, j, 0) = (k/pattern_size.width)*0.73;
				CV_MAT_ELEM(*object_points, float, j, 1) = (k%pattern_size.width)*0.73;
				CV_MAT_ELEM(*object_points, float, j, 2) = 0.0f;
			}
			cvFindExtrinsicCameraParams2(object_points,image_points,intrinsic_matrix,distortion_coeffs,rotation,translation);
			cvShowImage("img",img);
			cvWaitKey(10);

		}
		else
			cout << "not found." << endl;
	cvReleaseImage(&img);
	return true;
}
bool Calibrate::FindExtrinsic(CvMat *intrinsic_matrix,CvMat *distortion_coeffs,CvMat *rotation,CvMat *translation,IplImage *image){
	int corner_count;
	CvSize pattern_size = cvSize(8,6);
	int found;
	//For NSC : Clean up
	//cvNamedWindow("img");
	//cvShowImage("img",image);
	//cvWaitKey(0);
	found = cvFindChessboardCorners(image, pattern_size, corners, &corner_count);		
	if(found!=0)
		{			
			gray_image = cvCreateImage(cvGetSize(image), 8, 1);
			cvCvtColor(image, gray_image, CV_BGR2GRAY);
			cvFindCornerSubPix(gray_image, corners, corner_count, cvSize(11,11), cvSize(-1,-1),
								cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));	
			cvCopyImage(image,image2);
			cvDrawChessboardCorners(image2,pattern_size,corners,corner_count,found);
			cvShowImage("img",image2);
			for (int j = 0, k = 0; k < num_points; j++,k++) {
				//cvDrawCircle(img,cvPoint(corners[k].x,corners[k].y),3,cvScalar(255,0,0,0),3);
				CV_MAT_ELEM(*image_points, float, j, 0) = corners[k].x;
				CV_MAT_ELEM(*image_points, float, j, 1) = corners[k].y;
				CV_MAT_ELEM(*object_points, float, j, 0) = (k%pattern_size.width)*0.9;
				CV_MAT_ELEM(*object_points, float, j, 1) = (k/pattern_size.width)*0.9;
				CV_MAT_ELEM(*object_points, float, j, 2) = 0.0f;
			}
			cvFindExtrinsicCameraParams2(object_points,image_points,intrinsic_matrix,distortion_coeffs,rotation,translation);
			//cvShowImage("img",image);
			//cvWaitKey(0);

		}
		else{
			cout << corner_count << " not found." << endl;			
			return false;
		}
	return true;
}