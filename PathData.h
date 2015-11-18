#ifndef _PATH_DATA_H_
#define _PATH_DATA_H_



class ArmDialog
{
public:
	ArmDialog();
	~ArmDialog();

	bool Initialize(dxKeyboard *_keyBoard, ArmGuiData* _armGuiData,
		ArmConnector* _armConnector,ArmConfig* _armConfig,
		Calibrate* _calibrate,Surf *_surf,CameraManager *_cameraMan,Stereo *_stereo);
	void Render();
	void DrawObject();

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
	Calibrate*			calibrate;
	TestFindChessboard	chess;
	Surf*				surf;
	CameraManager*		cameraMan;
	Stereo*				stereo;
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

	//HBITMAP				greenBmp;
	//HBITMAP				redBmp;



	ArmCalculate	armCal;
	float targetX,targetY,targetZ;
	float				rtri;
	Matrix41			result;


	bool InitDialog();
	void ResetDialog();
	void UpdateGUIData();
	void DrawArm(float length);
	void DrawGrip();
	void DrawAxis();
	void UpdateText();
	void Swap(float *a,float *b);

	mVector3f CalculateEndEffectorPosition();
	void CalculateInverseKinematic(mVector3f dest);
};

#endif // _PATH_DATA_H_
