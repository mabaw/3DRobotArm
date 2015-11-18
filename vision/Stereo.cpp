#include "Stereo.h"


Stereo::Stereo()
{	
}
Stereo::~Stereo()
{
}

void Stereo::collectImage1(IplImage* _image) {
	cout << "collect1 " <<endl;
	cvReleaseImage(&image1);
	image1 = cvCloneImage(_image);
	//image1 = _image;
}

void Stereo::collectImage2(IplImage* _image) {
		cout << "collect2 " <<endl;		
	cvReleaseImage(&image2);
	image2 = cvCloneImage(_image);
}

void Stereo::LoadReprojectImage()
{
	pointCloud = (CvMat*)cvLoad("save/reprojectImage.xml");
	cvNamedWindow("reproject");
	cvShowImage("reproject",pointCloud);
}

void Stereo::LoadDisparity()
{
	

}

bool Stereo::Execute(){
	cout << "Stereo::Execute()" << endl;

	cvNamedWindow("img1");
	cvNamedWindow("img2");

	cvShowImage("img1",image1);
	cvSaveImage("img1.jpg",image1);
	cvShowImage("img2",image2);
	cvSaveImage("img2.jpg",image2);
	//cvWaitKey(0);
	int nx = 8, ny = 6;
    int displayCorners = 1;
    int showUndistorted = 1;
    bool isVerticalStereo = false;//OpenCV can handle left-right
                                      //or up-down camera arrangements
    const int maxScale = 1;
    const float squareSize = 1.f; //Set this to your actual square size


	int i, j, lr, nframes, n, N = 0;
    vector<string> imageNames[2];
    vector<CvPoint3D32f> objectPoints;
    vector<CvPoint2D32f> points[2];
    vector<int> npoints;
    vector<uchar> active[2];
    vector<CvPoint2D32f> temp;
    CvSize imageSize = {0,0};
    // ARRAY AND VECTOR STORAGE:
    double M1[3][3], M2[3][3], D1[5], D2[5];
    double R[3][3], T[3], E[3][3], F[3][3],Q[4][4];
    CvMat _M1 = cvMat(3, 3, CV_64F, M1 );
    CvMat _M2 = cvMat(3, 3, CV_64F, M2 );
    CvMat _D1 = cvMat(1, 5, CV_64F, D1 );
    CvMat _D2 = cvMat(1, 5, CV_64F, D2 );
    CvMat _R = cvMat(3, 3, CV_64F, R );
    CvMat _T = cvMat(3, 1, CV_64F, T );
    CvMat _E = cvMat(3, 3, CV_64F, E );
    CvMat _F = cvMat(3, 3, CV_64F, F );
	CvMat _Q = cvMat(4, 4, CV_64F, Q);

	int result;
	int count;

	cout << "Stereo::Execute() : declare variables" << endl;

	if( displayCorners )
        cvNamedWindow( "corners", 1 );
	n = nx*ny;
	temp.resize(n);

	IplImage *img;
	for(i=0;i<2;i++)
    {
	lr = i % 2;
	if(i==0) img = image1;
	else img = image2;
	imageSize = cvGetSize(img);
	vector<CvPoint2D32f>& pts = points[lr];
	//FIND CHESSBOARDS AND CORNERS THEREIN:
        for( int s = 1; s <= maxScale; s++ )
        {
            IplImage* timg = img;
            if( s > 1 )
            {
                timg = cvCreateImage(cvSize(img->width*s,img->height*s),
                    img->depth, img->nChannels );
                cvResize( img, timg, CV_INTER_CUBIC );
            }
            result = cvFindChessboardCorners( timg, cvSize(nx, ny),
                &temp[0], &count,
                CV_CALIB_CB_ADAPTIVE_THRESH |
                CV_CALIB_CB_NORMALIZE_IMAGE);

			cout << "count = " << count << endl;
            if( timg != img )
                cvReleaseImage( &timg );
            if( result || s == maxScale )
                for( j = 0; j < count; j++ )
            {
                temp[j].x /= s;
                temp[j].y /= s;
            }
			if( result == 0 ){
				cout << "Can't find chessboard" << endl;            
				return false;//break;
			}
        }
		if( displayCorners )
        {
			IplImage* cimg = cvCreateImage( cvSize(img->width,img->height), 8, 3 );
            //cvCvtColor( img, cimg, CV_GRAY2BGR );
			cimg  = cvCloneImage(img);
            cvDrawChessboardCorners( cimg, cvSize(nx, ny), &temp[0],
                count, result );
            cvShowImage( "corners", cimg );
            cvReleaseImage( &cimg );
            int c = cvWaitKey(1000);
            if( c == 27 || c == 'q' || c == 'Q' ) //Allow ESC to quit
                exit(-1);
        } else
            putchar('.');
		N = pts.size();
        pts.resize(N + n, cvPoint2D32f(0,0));
        active[lr].push_back((uchar)result);

		if( result )
        {
         //Calibration will suffer without subpixel interpolation
			IplImage* grayImg = cvCreateImage( cvSize(img->width,img->height), 8, 1);
			cvConvertImage(img,grayImg,CV_BGR2GRAY);
            cvFindCornerSubPix( grayImg, &temp[0], count,
                cvSize(11, 11), cvSize(-1,-1),
                cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,30, 0.1) );
		
            copy( temp.begin(), temp.end(), pts.begin() + N );
			
        }
     //   cvReleaseImage( &img );
	}

	printf("\n");
	// HARVEST CHESSBOARD 3D OBJECT POINT LIST:
		nframes = active[0].size();//Number of good chessboads found		
		objectPoints.resize(nframes*n);
		for( i = 0; i < ny; i++ )
			for( j = 0; j < nx; j++ )
			objectPoints[i*nx + j] =
			cvPoint3D32f(i*squareSize, j*squareSize, 0);
		for( i = 1; i < nframes; i++ )
			copy( objectPoints.begin(), objectPoints.begin() + n,
			objectPoints.begin() + i*n );
		npoints.resize(nframes,n);
		N = nframes*n;
		CvMat _objectPoints = cvMat(1, N, CV_32FC3, &objectPoints[0] );
		CvMat _imagePoints1 = cvMat(1, N, CV_32FC2, &points[0][0] );
		CvMat _imagePoints2 = cvMat(1, N, CV_32FC2, &points[1][0] );
		CvMat _npoints = cvMat(1, npoints.size(), CV_32S, &npoints[0] );
		cvSetIdentity(&_M1);
		cvSetIdentity(&_M2);
		cvZero(&_D1);
		cvZero(&_D2);

		// CALIBRATE THE STEREO CAMERAS
			printf("Running stereo calibration ...");
			fflush(stdout);
			cvStereoCalibrate( &_objectPoints, &_imagePoints1,
				&_imagePoints2, &_npoints,
				&_M1, &_D1, &_M2, &_D2,
				imageSize, &_R, &_T, &_E, &_F,
				cvTermCriteria(CV_TERMCRIT_ITER+
				CV_TERMCRIT_EPS, 100, 1e-5),
				CV_CALIB_FIX_ASPECT_RATIO +
				CV_CALIB_ZERO_TANGENT_DIST +
				CV_CALIB_SAME_FOCAL_LENGTH );
			printf(" done\n");

			//COMPUTE AND DISPLAY RECTIFICATION
    if( showUndistorted )
    {
        CvMat* mx1 = cvCreateMat( imageSize.height,
            imageSize.width, CV_32F );
        CvMat* my1 = cvCreateMat( imageSize.height,
            imageSize.width, CV_32F );
        CvMat* mx2 = cvCreateMat( imageSize.height,

            imageSize.width, CV_32F );
        CvMat* my2 = cvCreateMat( imageSize.height,
            imageSize.width, CV_32F );
        CvMat* img1r = cvCreateMat( imageSize.height,
            imageSize.width, CV_8U );
        CvMat* img2r = cvCreateMat( imageSize.height,
            imageSize.width, CV_8U );
		CvMat* disp = cvCreateMat( imageSize.height,
            imageSize.width, CV_16S );
        CvMat* dispLeft = cvCreateMat( imageSize.height,
            imageSize.width, CV_16S );
		CvMat* dispRight = cvCreateMat( imageSize.height,
            imageSize.width, CV_16S );
        CvMat* vdisp = cvCreateMat( imageSize.height,
            imageSize.width, CV_8U );

        CvMat* pair;
        double R1[3][3], R2[3][3], P1[3][4], P2[3][4];
        CvMat _R1 = cvMat(3, 3, CV_64F, R1);
        CvMat _R2 = cvMat(3, 3, CV_64F, R2);

		CvMat _P1 = cvMat(3, 4, CV_64F, P1);
        CvMat _P2 = cvMat(3, 4, CV_64F, P2);

        //cvStereoRectify( &_M1, &_M2, &_D1, &_D2, imageSize,
        //        &_R, &_T,
        //        &_R1, &_R2, &_P1, &_P2, 0,
        //        0/*CV_CALIB_ZERO_DISPARITY*/ );
        cvStereoRectify( &_M1, &_M2, &_D1, &_D2, imageSize,
                &_R, &_T,&_R1, &_R2, &_P1, &_P2, &_Q, 0 );
            isVerticalStereo = false;// fabs(P2[1][3]) > fabs(P2[0][3]);
		//Precompute maps for cvRemap()
            cvInitUndistortRectifyMap(&_M1,&_D1,&_R1,&_P1,mx1,my1);
            cvInitUndistortRectifyMap(&_M2,&_D2,&_R2,&_P2,mx2,my2);

		cvNamedWindow( "rectified", 1 );

	// RECTIFY THE IMAGES AND FIND DISPARITY MAPS
        if( !isVerticalStereo )
            pair = cvCreateMat( imageSize.height, imageSize.width*2,
            CV_8UC3 );
        else
            pair = cvCreateMat( imageSize.height*2, imageSize.width,
            CV_8UC3 );

		
		CvMat part;
		IplImage *gray1 = cvCreateImage(imageSize,8,1);
		IplImage *gray2 = cvCreateImage(imageSize,8,1);
		cvConvertImage(image1,gray1,CV_BGR2GRAY);
		cvConvertImage(image2,gray2,CV_BGR2GRAY);
        cvRemap( gray1, img1r, mx1, my1 );
        cvRemap( gray2, img2r, mx2, my2 );
		cvReleaseImage(&gray1);
		cvReleaseImage(&gray2);


				if( !isVerticalStereo )
                {
					cout << "!isVerticalStereo" << endl;
                    cvGetCols( pair, &part, 0, imageSize.width );
                    cvCvtColor( img1r, &part, CV_GRAY2BGR );
                    cvGetCols( pair, &part, imageSize.width,
                        imageSize.width*2 );
                    cvCvtColor( img2r, &part, CV_GRAY2BGR );
                    for( j = 0; j < imageSize.height; j += 16 )
                        cvLine( pair, cvPoint(0,j),
                        cvPoint(imageSize.width*2,j),
                        CV_RGB(0,255,0));
                }
                else
                {
					cout << "isVerticalStereo" << endl;
                    cvGetRows( pair, &part, 0, imageSize.height );
                    cvCvtColor( img1r, &part, CV_GRAY2BGR );
                    cvGetRows( pair, &part, imageSize.height,
                        imageSize.height*2 );
                    cvCvtColor( img2r, &part, CV_GRAY2BGR );
                    for( j = 0; j < imageSize.width; j += 16 )
                        cvLine( pair, cvPoint(j,0),
                        cvPoint(j,imageSize.height*2),
                        CV_RGB(0,255,0));
                }
		cvShowImage( "rectified", pair );
		cvSaveImage("rectified.jpg",pair);


		cvWaitKey(0);
	
		SMProc(img1r,img2r,dispLeft,dispRight,vdisp);
		//BMProc(img1r,img2r,disp,vdisp);
		//GCProc(img1r,img2r,dispLeft,dispRight,vdisp);
		//// find correspondence using String matching
		//SMCorrespondence(img1r,img2r,dispLeft,dispRight);


		//cvNormalize( dispLeft, vdisp, 0, 256, CV_MINMAX );
  //      cvNamedWindow( "disparityLeft" );
  //      cvShowImage( "disparityLeft", vdisp );		
		//cvSaveImage("disparityLeft.jpg",vdisp);
		//cvSave("disparityLeft.xml",dispLeft);

  //		cvNormalize( dispRight, vdisp, 0, 256, CV_MINMAX );
  //      cvNamedWindow( "disparityRight" );
  //      cvShowImage( "disparityRight", vdisp );
		//cvSaveImage("disparityRight.jpg",vdisp);
		//cvSave("disparityRight.xml",dispRight);



		// find correspondence using Graph Cut
		/*
		cvFindStereoCorrespondenceGC(img1r, img2r, dispLeft, dispRight, GCState);
		cvReleaseStereoGCState(&GCState);

  		cvNormalize( dispLeft, vdisp, 0, 256, CV_MINMAX );
        cvNamedWindow( "disparityLeft" );
        cvShowImage( "disparityLeft", vdisp );		
		cvSaveImage("disparityLeft.jpg",vdisp);
		cvSave("disparityLeft.xml",vdisp);

  		cvNormalize( dispRight, vdisp, 0, 256, CV_MINMAX );
        cvNamedWindow( "disparityRight" );
        cvShowImage( "disparityRight", vdisp );
		cvSaveImage("disparityRight.jpg",vdisp);
		cvSave("disparityRight.xml",vdisp);
		*/

		//CvStereoBMState *BMState = cvCreateStereoBMState();
  //      assert(BMState != 0);
  //      BMState->preFilterSize=33;
  //      BMState->preFilterCap=33;
  //      BMState->SADWindowSize=33;
		//BMState->minDisparity=-32;
  //      BMState->numberOfDisparities=192;
		//BMState->textureThreshold=10;
  //      BMState->uniquenessRatio=15;

		//CvMat part;// = cvCreateMat( imageSize.height,imageSize.width, CV_8U );
		//IplImage *gray1 = cvCreateImage(imageSize,8,1);
		//IplImage *gray2 = cvCreateImage(imageSize,8,1);
		//cvConvertImage(image1,gray1,CV_BGR2GRAY);
		//cvConvertImage(image2,gray2,CV_BGR2GRAY);
  //      cvRemap( gray1, img1r, mx1, my1 );
  //      cvRemap( gray2, img2r, mx2, my2 );
		//cvReleaseImage(&gray1);
		//cvReleaseImage(&gray2);

	 //  if( !isVerticalStereo )
  //              {
  //            // When the stereo camera is oriented vertically,
  //            // useUncalibrated==0 does not transpose the
  //            // image, so the epipolar lines in the rectified
  //            // images are vertical. Stereo correspondence
  //            // function does not support such a case.
  //                  cvFindStereoCorrespondenceBM( img1r, img2r, disp,
  //                      BMState);
  //                  cvNormalize( disp, vdisp, 0, 256, CV_MINMAX );
  //                  cvNamedWindow( "disparity" );
  //                  cvShowImage( "disparity", vdisp );
  //              }



//		IplImage *tdImg = cvCreateImage(imageSize,16,3);
		
		 CvMat* tdImg = cvCreateMat( imageSize.height,
            imageSize.width, CV_32FC3);
		 CvMat* normalizeTdImg = cvCreateMat( imageSize.height,
			 imageSize.width, CV_8UC3 ); 

		

		 if(CV_ARE_SIZES_EQ(dispLeft,tdImg))
			 cout << "size equal." << endl;
		cvReprojectImageTo3D(dispLeft,tdImg,&_Q,0);
		pointCloud = tdImg;
		//cvSaveImage("reprojectImage.jpg",tdImg);
		cvSave("reprojectImage.xml",tdImg);
		/* CvSize dispSize = {disp->width,disp->height};
		for(int y = 0; y < disp->height; y++ )
		{
			float* dptr0 = (float*)(tdImg->data.ptr + tdImg->step*y), *dptr = dptr0;
			 for(int x = 0; x < dispSize.width ; x++)
			 {
				cout << dptr[x*3] << "," << dptr[x*3+1] << "," << dptr[x*3+2] << endl;
			 }
		}*/
	}
	return true;
}

void Stereo::SMProc(CvMat* img1r,CvMat* img2r,CvMat* dispLeft,CvMat* dispRight,CvMat* vdisp)
{
		// find correspondence using String matching
		SMCorrespondence(img1r,img2r,dispLeft,dispRight);


		cvNormalize( dispLeft, vdisp, 0, 256, CV_MINMAX );
        cvNamedWindow( "disparityLeft" );
        cvShowImage( "disparityLeft", vdisp );		
		cvSaveImage("SMdisparityLeft.jpg",vdisp);
		cvSave("SMdisparityLeft.xml",dispLeft);

  		cvNormalize( dispRight, vdisp, 0, 256, CV_MINMAX );
        cvNamedWindow( "disparityRight" );
        cvShowImage( "disparityRight", vdisp );
		cvSaveImage("SMdisparityRight.jpg",vdisp);
		cvSave("SMdisparityRight.xml",dispRight);
}

void Stereo::BMProc(CvMat* img1r,CvMat* img2r,CvMat* disp,CvMat* vdisp){
		CvStereoBMState *BMState = cvCreateStereoBMState();
        assert(BMState != 0);
        BMState->preFilterSize=33;
        BMState->preFilterCap=33;
        BMState->SADWindowSize=33;
		BMState->minDisparity=-32;
        BMState->numberOfDisparities=192;
		BMState->textureThreshold=10;
        BMState->uniquenessRatio=15;

        cvFindStereoCorrespondenceBM( img1r, img2r, disp,BMState);
        cvNormalize( disp, vdisp, 0, 256, CV_MINMAX );
        cvNamedWindow( "disparity" );
        cvShowImage( "disparity", vdisp );
		cvSave("BMDisparity.xml",disp);
		cvSaveImage("BMdisparity.jpg",vdisp);
}

void Stereo::GCProc(CvMat* img1r,CvMat* img2r,CvMat* dispLeft,CvMat* dispRight,CvMat* vdisp){
// find correspondence using Graph Cut
		CvStereoGCState *GCState = cvCreateStereoGCState(192,2);
		GCState->minDisparity=-32;


		cvFindStereoCorrespondenceGC(img1r, img2r, dispLeft, dispRight, GCState);
		cvReleaseStereoGCState(&GCState);

  		cvNormalize( dispLeft, vdisp, 0, 256, CV_MINMAX );
        cvNamedWindow( "disparityLeft" );
        cvShowImage( "disparityLeft", vdisp );		
		cvSaveImage("disparityLeft.jpg",vdisp);
		cvSave("disparityLeft.xml",dispLeft);

  		cvNormalize( dispRight, vdisp, 0, 256, CV_MINMAX );
        cvNamedWindow( "disparityRight" );
        cvShowImage( "disparityRight", vdisp );
		cvSaveImage("disparityRight.jpg",vdisp);
		cvSave("disparityRight.xml",dispRight);
}

void Stereo::SMCorrespondence(CvMat *img1,CvMat *img2, CvMat *disp1, CvMat *disp2)
{

	int height = img1->height;
	int width = img1->width;
	//FILE* pathFile = fopen("pathout.out","a");
	IplImage *pathImg = cvCreateImage(cvSize(640,640),8,1);
	//cvNamedWindow("Path");
	for(int i=0;i<disp1->height;i++)
		for(int j=0;j<disp1->width;j++)
		{
			short* dptr0 = (short*)(disp1->data.ptr + disp1->step*i);
			dptr0[j] = 0;
			dptr0 = (short*)(disp2->data.ptr + disp2->step*i);
			dptr0[j] = 0;
			
		}
	for(int y=0;y<height;y++)
	{
		uchar *s1 = (uchar*)(img1->data.ptr + img1->step*y);
		uchar *s2 = (uchar*)(img2->data.ptr + img2->step*y);	
		double left,right,diagonal;
		
		for(int i=0;i<width;i++)
		{
			error[0][i] = 0;
			error[i][0] = 0;
			countskip[0][i] = i;
			countskip[i][0] = i;
		}
		for(int i=1;i<=width;i++)
			for(int j=1;j<=width;j++)
			{
				left = error[i][j-1] + 10;
			/*	if(countskip[i][j-1] > 300)
					left = 1000000;*/
				right = error[i-1][j] + 10;
				/*if(countskip[i-1][j] > 300)
					right = 1000000;*/
				double temp = 1000000;
				int numMatch=0; int all=0;
				if(i<j){ numMatch=i; all=j+1;} 
				else { numMatch=j; all=i+1; }
				float ky = 0.5 * (numMatch / all);
				temp = abs(s1[i]-s2[j]) - ky*numMatch;
				
				diagonal = error[i-1][j-1] + temp;
				if(y>0) diagonal += 0.5*abs( (i-j) - (short)(disp1->data.ptr + disp1->step*(y-1))[i] ) ;
				if(i!=width&&j!=width &&diagonal <= left && diagonal <= right)
				{
					error[i][j] = diagonal;
					path[i][j] = 0;
				}
				else if(left < diagonal && left < right)
				{
					error[i][j] = left;
					path[i][j] = 1;
					countskip[i][j] = countskip[i][j-1] + 1;
				}
				else
				{
					error[i][j] = right;					
					path[i][j] = 2;
					countskip[i][j] = countskip[i-1][j] + 1;
				}
			}
	// tracking
		int i=width;
		int j=width;
		int countmatch=0;	
		short* dptr1 = (short*)(disp1->data.ptr + disp1->step*y);
		short* dptr2 = (short*)(disp2->data.ptr + disp2->step*y);
		while(true)
		{
			if(i==0||j==0) break;
			if(path[i][j]==0) // match pixel here
			{	
				((uchar*)(pathImg->imageData + pathImg->widthStep*j))[i] = 255;

				short* ptrLeft = (short*)(disp1->data.ptr + disp1->step*y);
				ptrLeft[i] = i-j;
				dptr1[i] = i-j;		

				short* ptrRight = (short*)(disp2->data.ptr + disp2->step*y);
				ptrRight[j] = j-i;
				dptr2[j] = j-i; 

				i--;j--;
				countmatch++;					
			}
			else if(path[i][j]==1)j--;
			else if(path[i][j]==2)i--;

		}
		
		//cvShowImage("Path",pathImg);
		//cvWaitKey(10);
	}


	//cvReleaseImage(&pathImg);
	//fclose(pathFile);
	return;
}


 