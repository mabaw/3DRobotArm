#include "Cube.h"

Cube::Cube()
{	
	ccc=0;
}
Cube::~Cube()
{
	//if(fAngle!=0)
	//	fclose(fAngle);
}

bool Cube::Initialize(float _sideWidth,float _size,CvPoint3D32f _origin,Calibrate *_calibrate,IplImage *_image,ArmConfig *_armConfig){
	//fAngle = fopen("img/savepos/cubeangle.in","r");
	useFile = true;
		cout << "8" << endl;

	LoadCamera();
	armConfig = _armConfig;
	size = _size;
	sideWidth = _sideWidth;
	origin = _origin;
	calibrate = _calibrate;
	image = _image;

			cout << "9" << endl;

	center.x = 2.7f;
	center.y = -8.2f;
	center.z = 0.0f;

	numside = (size / sideWidth) + 1;
	n_points = numside*numside*numside;
	if(pointList==0) 
		pointList = new CvPoint3D32f[n_points+48];
	status = new int[n_points+48];
	memset(status,0,sizeof(int)*(n_points+48));
	object_points	=	cvCreateMat(n_points+48, 3, CV_32FC1);
	image_points	=	cvCreateMat(n_points+48, 2, CV_32FC1);
	cutBgImg = cvCreateMat( image->height,image->width, CV_8U );
	rotation = cvCreateMat(3, 1, CV_32FC1);
	translation = cvCreateMat(3, 1, CV_32FC1);

			cout << "10" << endl;


	rotation_normal = cvCreateMat(3,3,CV_32FC1);
	cameraMatrix= cvCreateMat(4,4,CV_32FC1);
	cameraMatrix_inv= cvCreateMat(4,4,CV_32FC1);

	draw = cvCreateImage(cvSize(image->width,image->height),8,3);
	white = cvCreateImage(cvSize(image->width,image->height),8,3);
	for(int i=0;i<image->height;i++)
		for(int j=0;j<image->width;j++)
		{
			((uchar*)(white->imageData + white->widthStep*i))[j*3] = 255;	
			((uchar*)(white->imageData + white->widthStep*i))[j*3+1] = 255;	
			((uchar*)(white->imageData + white->widthStep*i))[j*3+2] = 255;	
		}

				
	CvPoint3D32f t;
	// create array of 3D-point
	float i,j,k;
	int count=0;
	// create array of 3D-point
		for(i=origin.x;i<=origin.x+size;i+=sideWidth)
		for(j=origin.y;j<=origin.y+size;j+=sideWidth)
			for(k=origin.z;k<=origin.z+size;k+=sideWidth)
			{
				pointList[count].x = i;
				pointList[count].y = j;
				pointList[count].z = k;	
				//status[count] = 1;
				//if(i==origin.x||i==origin.x+size||j==origin.y||j==origin.y+size||k==origin.z||k==origin.z+size)
				//	status[count] = 0;
				count++;
			}

	//

		//For NSC : Clean up
		//cvNamedWindow("Draw");
		cvNamedWindow("Im");
		ccc = 0;
	return true;
}

void Cube::ResetCube(float _sideWidth,float _size,CvPoint3D32f _origin)
{
	LoadCamera();
	size = _size;
	sideWidth = _sideWidth;
	origin = _origin;

	int n = (size / sideWidth) + 1;
	n_points = n*n*n;
	if(pointList==0) 
		pointList = new CvPoint3D32f[n_points];
	status = new int[n_points];
	memset(status,0,sizeof(int)*n_points);
	CvPoint3D32f t;
	float i,j,k;
	int count=0;
	// create array of 3D-point
		for(i=origin.x;i<=origin.x+size;i+=sideWidth)
		for(j=origin.y;j<=origin.y+size;j+=sideWidth)
			for(k=origin.z;k<origin.z+size;k+=sideWidth)
			{
				pointList[count].x = i;
				pointList[count].y = j;
				pointList[count++].z = k;
			}

}

int Cube::getStatus(int index)
{
	if(index>=0&&index<n_points)
	{
		if(status[index]!=-1)
		return status[index];
		else return 0;
	}
	else{
		//cout << "index out of bound : Cube::getStatus " << index  << endl;
		return 0;
	}
}

bool Cube::FindRealPosition(CvPoint3D32f point,CvPoint3D32f center,float angleZ,CvPoint3D32f *realPoint,int *index){
	Matrix44f mat,tmp1,tmp2,tmp3,tmp4;
	
	Matrix44f::Identity(&mat);
	Matrix44f::Identity(&tmp1);
	Matrix44f::Identity(&tmp2);
	Matrix44f::Identity(&tmp3);
	Matrix44f::Identity(&tmp4);

	Matrix44f::Translation(&tmp1,center.x,center.y,center.z);	
	Matrix44f::RotationZ(&tmp2,angleZ/180*3.14);
	Matrix44f::Translation(&tmp3,-center.x,-center.y,-center.z);
	Matrix44f::Translation(&tmp4,point.x,point.y,point.z);

	mat = tmp1 * tmp2 * tmp3 * tmp4;

	realPoint->x = mat.m14;
	realPoint->y = mat.m24;
	realPoint->z = mat.m34;

	// calculate i
	float x_dist = realPoint->x - origin.x;
	float y_dist = realPoint->y - origin.y;
	float z_dist = realPoint->z - origin.z;

	if(x_dist >= 0 && x_dist <= size
		&& y_dist >= 0 && y_dist <= size
			&& z_dist >= 0 && z_dist <= size)
	{
		float xf = x_dist / sideWidth;
		float yf = y_dist / sideWidth;
		float zf = z_dist / sideWidth;
		
		float x_fractpart, x_intpart;
		x_fractpart = modf (xf , &x_intpart);
		float y_fractpart, y_intpart;
		y_fractpart = modf (yf, &y_intpart);
		float z_fractpart, z_intpart;
		z_fractpart = modf (zf , &z_intpart);

		int i,j,k;
		if(x_fractpart < 0.5f)
			i = xf;
		else
			i = ceil(xf);
		if(y_fractpart < 0.5f)
			j = yf;
		else
			j = ceil(yf);
		if(z_fractpart < 0.5f)
			k = zf;
		else
			k = ceil(zf);
		*index = numside*numside*i + numside*j + k;
		return true;
	}
	else
		return false;
}
void Cube::ProjectBack(CvMat *rotation,CvMat *translation,CvMat *cutBg,IplImage *img){

		for(int i=0;i<n_points+48;i++)
		{
					CV_MAT_ELEM(*object_points, float, i, 0) = pointList[i].x;
					CV_MAT_ELEM(*object_points, float, i, 1) = pointList[i].y;
					CV_MAT_ELEM(*object_points, float, i, 2) = pointList[i].z;	
					//if(pointList[i].z==0)
					//	cout << pointList[i].x << "," << pointList[i].y << ","  << pointList[i].z << endl;
		}
		
			if(useFile)	
					armConfig->ceta5 = angle[ccc++];

				cout << armConfig->ceta5 << endl;

		//cout << endl;

		cvProjectPoints2(object_points,rotation,translation,intrinsic_matrix,distortion_coeffs,image_points);
		//cvSave("imgpoint.xml",image_points);
			
		cvCopyImage(white,draw);
		CvPoint point;
		for(int i=0;i<n_points+24;i++)
		//for(int i=0;i<n_points;i++)
		{
				int index;
				CvPoint3D32f realPoint;
				/*if(ccc>=0 && ccc <=7) armConfig->ceta5 = 0.0f;
				else if(ccc>=8 && ccc <=17) armConfig->ceta5 = -85.0f;
				else armConfig->ceta5 = 85.0f;
				ccc++;*/


				if(!FindRealPosition(pointList[i],center,-(armConfig->ceta5),&realPoint,&index))
					continue;
				if(status[index]==-1) continue;
				int x = CV_MAT_ELEM(*image_points, float, i, 0);
				int y = CV_MAT_ELEM(*image_points, float, i, 1);
				point.x = x;
				point.y = y;	
				//cout << x << "," << y << endl;

				
				if( x>=0&&x<cutBg->width && y>=0&&y<cutBg->height)
				{					
					if(	CV_MAT_ELEM(*cutBg, uchar, y, x) == 0 )
						status[index] = 1;
					else
						status[index] = -1;
				}
				//cvDrawCircle(draw,point,1,cvScalar(0x0,0x0,0xff),3);	
				char buff[30];
				//sprintf(buff,"%.1f,%.1f,%.1f",pointList[i].x,pointList[i].y,pointList[i].z);
				if(i<n_points )
				{
					//CvFont* font = new CvFont();
					//cvInitFont(font,CV_FONT_HERSHEY_PLAIN,1.0,1.0);
					//cvPutText(draw,buff,point,font,cvScalar(0x0,0x0,0xff));
					if(pointList[index].z==0)
					{
						cvDrawCircle(img,point,1,cvScalar(0xff,0x0,0x0),2);
					}
					//if(pointList[i].z==-4)
					//{
					//	cvDrawCircle(img,point,1,cvScalar(0x0,0xff,0x0),2);

					//}
				}
				else if (i>n_points)
				{
					int j = i-n_points;
					//if(j!=0&&j!=5)
						cvDrawCircle(img,point,1,cvScalar(0x0,0x0,0xff),2);			
				}

				
		}		
		//For NSC : Clean up
		//cvShowImage("Draw",draw);
		//cvShowImage("Im",img);
		cvWaitKey(5);

}


void Cube::LoadCamera(){
	intrinsic_matrix = (CvMat*)cvLoad("IntrinsicsLive.xml");
	distortion_coeffs = (CvMat*)cvLoad("DistortionLive.xml");

	for(int i=0;i<4;i++)
		CV_MAT_ELEM(*distortion_coeffs, float, i, 0) = 0;

}

void Cube::Execute(){
	visionUtil.CutGreenBackground(image,cutBgImg);
	if(calibrate->FindExtrinsic(intrinsic_matrix,distortion_coeffs,rotation,translation,image))
	{
		ProjectBack(rotation,translation,cutBgImg,image);
		cvRodrigues2(rotation,rotation_normal);

		cvSave("translation.xml",translation);
		cvSave("rotation_rodrigue.xml",rotation);
			cvSave("rotation_normal.xml",rotation_normal);

			CV_MAT_ELEM(*cameraMatrix, float, 0, 0) = CV_MAT_ELEM(*rotation_normal, float, 0, 0);
			CV_MAT_ELEM(*cameraMatrix, float, 0, 1) = CV_MAT_ELEM(*rotation_normal, float, 0, 1);
			CV_MAT_ELEM(*cameraMatrix, float, 0, 2) = CV_MAT_ELEM(*rotation_normal, float, 0, 2);
			
			CV_MAT_ELEM(*cameraMatrix, float, 1, 0) = CV_MAT_ELEM(*rotation_normal, float, 1, 0);
			CV_MAT_ELEM(*cameraMatrix, float, 1, 1) = CV_MAT_ELEM(*rotation_normal, float, 1, 1);		
			CV_MAT_ELEM(*cameraMatrix, float, 1, 2) = CV_MAT_ELEM(*rotation_normal, float, 1, 2);

			
			CV_MAT_ELEM(*cameraMatrix, float, 2, 0) = CV_MAT_ELEM(*rotation_normal, float, 2, 0);
			CV_MAT_ELEM(*cameraMatrix, float, 2, 1) = CV_MAT_ELEM(*rotation_normal, float, 2, 1);			
			CV_MAT_ELEM(*cameraMatrix, float, 2, 2) = CV_MAT_ELEM(*rotation_normal, float, 2, 2);

			CV_MAT_ELEM(*cameraMatrix, float, 0, 3) = CV_MAT_ELEM(*translation, float, 0, 0);
			CV_MAT_ELEM(*cameraMatrix, float, 1, 3) = CV_MAT_ELEM(*translation, float, 1, 0);
			CV_MAT_ELEM(*cameraMatrix, float, 2, 3) = CV_MAT_ELEM(*translation, float, 2, 0);

			CV_MAT_ELEM(*cameraMatrix, float, 3, 0) = 0;
			CV_MAT_ELEM(*cameraMatrix, float, 3, 1) = 0;
			CV_MAT_ELEM(*cameraMatrix, float, 3, 2) = 0;
			CV_MAT_ELEM(*cameraMatrix, float, 3, 3) = 1;

			cvInvert(cameraMatrix,cameraMatrix_inv);
			cvSave("cameraMatrix_inv.xml",cameraMatrix_inv);
	}




}

void Cube::Test(IplImage **img,int size)
{
	int num_points=48;
	CvPoint2D32f* corners = new CvPoint2D32f[num_points];
	CvSize pattern_size = cvSize(8,6);
	int corner_count;
	
	//CvPoint3D32f origin;
	//origin.x = -2.0f;
	//origin.y = -1.0f;
	//origin.z = 0.0f;


	//ResetCube(1.0f,10.0f,origin);
	for(int i=0;i<size;i++)
	{
		cout << i << " : ";
			visionUtil.CutGreenBackground(img[i],cutBgImg);

			IplImage *gryImg = cvCreateImage(cvGetSize(img[i]), IPL_DEPTH_8U, 1);
			cvCvtColor(img[i], gryImg, CV_RGB2GRAY);

			int found = cvFindChessboardCorners(img[i], pattern_size, corners, &corner_count,1);
			//cout << "corner_count : " << corner_count << endl;
			//cvDrawChessboardCorners(gryImg,pattern_size,corners,corner_count,found);

			//cvNamedWindow("Hare");
			//cvShowImage("Hare", gryImg);
			//cvWaitKey();
			if(found!=0)
			{
				cout << "found." << endl;
				IplImage *gray_image = cvCreateImage(cvGetSize(img[i]), 8, 1);
				cvCvtColor(img[i], gray_image, CV_BGR2GRAY);
				cvFindCornerSubPix(gray_image, corners, corner_count, cvSize(11,11), cvSize(-1,-1),
									cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));

				for (int j = 0, k = 0; k < num_points; j++,k++) {
					//cvDrawCircle(img,cvPoint(corners[k].x,corners[k].y),3,cvScalar(255,0,0,0),3);
					if(j==0) cout << corners[k].x << "," <<corners[k].y << endl;
					//CV_MAT_ELEM(*image_points, float, n_points+j, 0) = corners[k].x;
					//CV_MAT_ELEM(*image_points, float, n_points+j, 1) = corners[k].y;
					pointList[n_points+j].x = (k%pattern_size.width)*0.9;
					pointList[n_points+j].y = (k/pattern_size.width)*0.9;
					pointList[n_points+j].z = 0;
					CV_MAT_ELEM(*object_points, float, n_points+j, 0) = (k%pattern_size.width)*0.9;
					CV_MAT_ELEM(*object_points, float, n_points+j, 1) = (k/pattern_size.width)*0.9;
					CV_MAT_ELEM(*object_points, float, n_points+j, 2) = 0.0f;
				}
				cvReleaseImage(&gray_image);
			}
			else 
			{
				cout <<  "not found. " << endl;
			}
			
			if(calibrate->FindExtrinsic(intrinsic_matrix,distortion_coeffs,rotation,translation,img[i]))
			{
				ProjectBack(rotation,translation,cutBgImg,img[i]);
				cvRodrigues2(rotation,rotation_normal);
			}

			cvSave("rotation_rodrigue.xml",rotation);
			cvSave("rotation_normal.xml",rotation_normal);

			CV_MAT_ELEM(*cameraMatrix, float, 0, 0) = CV_MAT_ELEM(*rotation_normal, float, 0, 0);
			CV_MAT_ELEM(*cameraMatrix, float, 0, 1) = CV_MAT_ELEM(*rotation_normal, float, 0, 1);
			CV_MAT_ELEM(*cameraMatrix, float, 0, 2) = CV_MAT_ELEM(*rotation_normal, float, 0, 2);
			
			CV_MAT_ELEM(*cameraMatrix, float, 1, 0) = CV_MAT_ELEM(*rotation_normal, float, 1, 0);
			CV_MAT_ELEM(*cameraMatrix, float, 1, 1) = CV_MAT_ELEM(*rotation_normal, float, 1, 1);		
			CV_MAT_ELEM(*cameraMatrix, float, 1, 2) = CV_MAT_ELEM(*rotation_normal, float, 1, 2);

			
			CV_MAT_ELEM(*cameraMatrix, float, 2, 0) = CV_MAT_ELEM(*rotation_normal, float, 2, 0);
			CV_MAT_ELEM(*cameraMatrix, float, 2, 1) = CV_MAT_ELEM(*rotation_normal, float, 2, 1);			
			CV_MAT_ELEM(*cameraMatrix, float, 2, 2) = CV_MAT_ELEM(*rotation_normal, float, 2, 2);

			CV_MAT_ELEM(*cameraMatrix, float, 0, 3) = CV_MAT_ELEM(*translation, float, 0, 0);
			CV_MAT_ELEM(*cameraMatrix, float, 1, 3) = CV_MAT_ELEM(*translation, float, 1, 0);
			CV_MAT_ELEM(*cameraMatrix, float, 2, 3) = CV_MAT_ELEM(*translation, float, 2, 0);

			CV_MAT_ELEM(*cameraMatrix, float, 3, 0) = 0;
			CV_MAT_ELEM(*cameraMatrix, float, 3, 1) = 0;
			CV_MAT_ELEM(*cameraMatrix, float, 3, 2) = 0;
			CV_MAT_ELEM(*cameraMatrix, float, 3, 3) = 1;

			cvInvert(cameraMatrix,cameraMatrix_inv);
			cvSave("cameraMatrix_inv.xml",cameraMatrix_inv);
						

			//cvReleaseMat(&cutBgImg);
	}
}

