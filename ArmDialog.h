#ifndef _ARMDLG_H_
#define _ARMDLG_H_

// globle
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>
#include "resource.h"

#include "yOpenGL\GLApp.h"
#include "yUtils\dxKeyboard.h"
#include "yMath\yMath.h"
#include "yUtils\yFile.h"
#include "Render\Mesh.h"
//#include "Render\MCube.h"
//#include "Render\MVertex.h"


#include "ArmConfig.h"
#include "ArmConnector.h"
#include "ArmCalculate.h"
#include "vision\Calibrate.h" 
#include "vision\TestFindChessboard.h"
#include "vision\Surf.h"
#include "vision\CameraManager.h"
#include "vision\Stereo.h"
#include "vision\VisionUtil.h"
#include "vision\Cube.h"


typedef struct _armGuiData
{
	
public :
	int isKeyMove;
	bool isScanning;
	bool hasObj;
	bool noObj;
	bool unknownObj;
	bool isDrawMesh;
	_armGuiData()
	{
		
	}
}ArmGuiData;

typedef struct _matrix41
{
public :
	float *data;
	_matrix41()
	{
		data = new float[4];
		data[0] = 0;
		data[1] = 0;
		data[2] = 0;
		data[3] = 1;
	}

	~_matrix41()
	{
		delete data;
	}
}Matrix41;


class ArmDialog
{
public:
	ArmDialog();
	~ArmDialog();

	bool Initialize(dxKeyboard *_keyBoard, ArmGuiData* _armGuiData,
		ArmConnector* _armConnector,ArmConfig* _armConfig,
		Calibrate* _calibrate,Surf *_surf,CameraManager *_cameraMan,Stereo *_stereo,Cube *_cube,Mesh *_mesh);
	void Render();
	void DrawObject();
	void DrawTrianglePlane(mVector3f e1,mVector3f e2,mVector3f e3);

	bool CALLBACK DlgProc( HWND hDlg, UINT message, 
		WPARAM wParam, LPARAM lParam );




private:
	HWND				m_hDlg;
	HINSTANCE			m_hInstance;

	dxKeyboard*			keyboard;
	ArmGuiData*			armDlgData;

	ArmConfig*			armConfig;
	ArmConnector*		armConnector;

	
	// vision
	Mesh*				mesh;	

	Calibrate*			calibrate;
	TestFindChessboard	chess;
	Surf*				surf;
	CameraManager*		cameraMan;
	Stereo*				stereo;
	Cube*				cube;



	GLApp				glApp;
	ModelViewerCamera	camera;
	ModelViewerCamera	camera2;
	ModelViewerCamera	originCamera;
	mVector3f			oldPosition;
	mVector3f			oldLookAtPos;
	mVector3f			oldUpVecPos;
	Matrix44f			viewMat;
	Matrix44f			projMat;

	Matrix44f			mat0;
	Matrix44f			mat1;
	Matrix44f			mat2;
	Matrix44f			mat3;
	Matrix44f			mat4;
	Matrix44f			mat5;
	Matrix44f			mat6;
	Matrix44f			mat7;
	Matrix44f			mat8;
	Matrix44f			allMat;
	Matrix44f			lookAt;
	Matrix44f			upVec;

	VisionUtil visionUtil;

	//HBITMAP				greenBmp;
	//HBITMAP				redBmp;



	ArmCalculate	armCal;
	float targetX,targetY,targetZ;
	float				rtri;
	Matrix41			result;

	// Path saver
	FILE			*positionPath;
	int				numsave;	
	char			filename[30];
	FILE			*walkPath;
	fpos_t			filePosition;

	
	IplImage		*img[80];





	bool InitDialog();
	void ResetDialog();
	void UpdateGUIData();
	void DrawArm(float length);
	void DrawGrip();
	void DrawAxis();
	void DrawCube();
	void UpdateText();
	void Swap(float *a,float *b);

	mVector3f CalculateEndEffectorPosition();
	void CalculateInverseKinematic(mVector3f dest);
};

#endif // _ARMDLG_H_
