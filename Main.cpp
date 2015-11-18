
#include "yUtils/dxKeyboard.h"
#include "ArmDialog.h"
#include "ArmConnector.h"
#include "vision/Calibrate.h"
#include "vision/CameraManager.h"
#include "vision/FindTagNumber.h"
#include "vision/Stereo.h"
#include "vision/Cube.h"

#include "Render/Mesh.h"

#include <iostream>

#include "cv.h"
#include "cxmisc.h"
#include "highgui.h"
#include "vision/VisionUtil.h"


ArmDialog armDlg;
ArmGuiData armDlgData;
bool isRunning;
int speed;
dxKeyboard		keyboard;
ArmConnector	armConnector;
ArmConfig		armConfig;

// Vision
CameraManager	cameraMan;
Calibrate		calibrate;
Surf			surf;
FindTagNumber	findTagNumber;
Stereo			stereo;
VisionUtil		visionUtil;
Cube			cube;

//Render
Mesh			mesh;

		


// test var
IplImage		*im1,*im2;
CvMat			*img1,*img2;
CvMat			*disp1,*disp2;
CvMat			*vdisp;

IplImage		*img[80];



bool Initialize()
{		
	armConnector.Initialize(L"COM7");
	cout << "1" << endl;
	cameraMan.Initialize();
	cout << "2" << endl;
	calibrate.Initialize(&cameraMan);
	cout << "3" << endl;
	//findTagNumber.Initialize();
	//surf.Initialize();
	armDlg.Initialize(&keyboard, &armDlgData,&armConnector,
		&armConfig,&calibrate,&surf,&cameraMan,&stereo,&cube,&mesh);
	cout << "4" << endl;
	//stereo.LoadReprojectImage();
	speed=10;
	isRunning	=	true;


	//cube.Initialize(0.0f,0.0f,0,calibrate,cameraMan.image); // temp init


	// test correspondence
	//im1 = cvLoadImage("im0.jpg",0);
	//im2 = cvLoadImage("im1.jpg",0);
	//
	//
	//CvSize imageSize = cvSize(im1->width,im1->height);
	//img1 = cvCreateMat(imageSize.height,imageSize.width,CV_8U);
	//img2 = cvCreateMat(imageSize.height,imageSize.width,CV_8U);
	//img1 = cvGetMat(im1,img1);
	//img2 = cvGetMat(im2,img2);
	//cvNamedWindow("img1");
	//cvNamedWindow("img2");
	//cvShowImage("img1",img1);
	//cvShowImage("img2",img2);
	//cvWaitKey(0);

	//disp1 = cvCreateMat( imageSize.height+1,
 //           imageSize.width+1, CV_16S );
	//disp2 = cvCreateMat( imageSize.height+1,
 //           imageSize.width+1, CV_16S );
	//vdisp = cvCreateMat( imageSize.height+1,
 //           imageSize.width+1, CV_8U );
	// end test




	// test cube
	//char buff[256];
	//for(int i=0;i<12;i++)
	//{
	//	sprintf(buff,"img/savepos/%d.bmp",i);
	//	img[i] = cvLoadImage(buff);
	//}
	CvPoint3D32f origin;
	origin.x = -3.0f;
	origin.y = -18.0f;
	origin.z = -15.0f;
	cout << "5" << endl;
	
	if(cameraMan.capture!=0)
		cameraMan.GrabImage();
	cout << "6" << endl;
	cube.Initialize(0.25f,15.0f,origin,&calibrate,cameraMan.image,&armConfig);
	cout << "7" << endl;
	//cube.LoadCamera();
	//calibrate.FindExtrinsic(cube.intrinsic_matrix,cube.distortion_coeffs,&rotation,&translation,img[0]);
	//cube.Test(img,12);

	//cvWaitKey(0);

	// end test cube
	

	return true;
}

void Process(){
	armDlg.Render();
	if(cameraMan.capture!=0)
		cameraMan.GrabImage();

	
	//findTagNumber.Execute(cameraMan.image);
	
	//stereo.SMProc(img1,img2,disp1,disp2,vdisp);
	//cvWaitKey(0);
	//stereo.SMCorrespondence(stereo.image1,stereo.image2,disp1,disp2);

	if(armDlgData.isScanning)
	{


	}
	
	if(armDlgData.isKeyMove&&!armConnector.IsArmMoving())
	{
		armConnector.WriteCommand(armConfig.ceta1,armConfig.ceta2,armConfig.ceta3,armConfig.ceta4,armConfig.ceta5,100);
	}


}

int _tmain(int argc, _TCHAR* argv[])
{
	MSG		msg;										// Windows Message Structure
	if( !Initialize() ) 
	{
		printf("Failed Initialize Program terminate !!!\n");
		return 0;
	}
	while( isRunning )										// Loop That Runs Until done = TRUE
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)					// Have We Received A Quit Message?
				isRunning = FALSE;						// If So done = TRUE
			else										// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);					// Translate The Message
				DispatchMessage(&msg);					// Dispatch The Message
			}
		}
		else											// If There Are No Messages
		{
			Process();
		}
	}
	keyboard.FreeDirectInput();
	return 0;
}