#include "ArmDialog.h"

BOOL CALLBACK MainDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	ArmDialog* mainDlg = (ArmDialog*)GetWindowLong( hDlg, GWL_USERDATA );
	return mainDlg->DlgProc( hDlg, message, wParam, lParam );
}

ArmDialog::ArmDialog() 
{
	positionPath=0;
	walkPath=0;
}

ArmDialog::~ArmDialog()
{	
	if(positionPath!=0);
		fclose(positionPath);
	if(walkPath!=0)
		fclose(walkPath);
}

bool ArmDialog::Initialize(dxKeyboard *_keyBoard, ArmGuiData* _armGuiData,
						   ArmConnector* _armConnector,ArmConfig* _armConfig,Calibrate* _calibrate,
						   Surf *_surf,CameraManager *_cameraMan,Stereo *_stereo,Cube* _cube,Mesh *_mesh)
{
	keyboard = _keyBoard;
	armDlgData = _armGuiData;
	armConnector = _armConnector;
	armConfig = _armConfig;
	stereo = _stereo;
	calibrate = _calibrate;
	cube = _cube;
	mesh = _mesh;


	targetX=0;targetY=0;targetZ=0;

	surf = _surf;
	cameraMan = _cameraMan;
	InitDialog();

	if(FAILED(keyboard->OnCreateDevice( m_hDlg )))
	{
		MessageBox( NULL, _T("Failed To Initialize keyboard."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return false;
	}

	glApp.m_hWnd = GetDlgItem( m_hDlg, IDC_Render_Pic );
	if ( !glApp.CreateGLWindow( GetModuleHandle(NULL), 0, 0, 0, 0 ) )
	{
		MessageBox( NULL, _T("Failed To Create GL Windows."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION );
		return FALSE;										// Quit If Window Was Not Created
	}

	camera.SetUpAxis(ModelViewerCamera::UPAXIS_Z);
	camera.m_fAngleY = G_PI;
	camera.m_fAngleX = G_PI/8;
	camera.CalPosition();
//	originCamera = camera;


	
	
	ResetDialog();

	armDlgData->isScanning	= false;
	armDlgData->hasObj		= false;
	armDlgData->noObj		= false;
	armDlgData->unknownObj	= false;

	positionPath	= fopen("img/savepos/path.txt","w");
	walkPath		= fopen("img/savepos/path2.txt","r");

	numsave			= 0;


	return true;
}

void ArmDialog::ResetDialog()
{
	/*
	// global
	CheckDlgButton(m_hDlg, IDC_Start_Chk, BST_UNCHECKED);
	CheckDlgButton(m_hDlg, IDC_Record_Chk, BST_UNCHECKED);
	CheckDlgButton(m_hDlg, IDC_Manual_Chk, BST_UNCHECKED);

	// device
	CheckDlgButton(m_hDlg, IDC_Robot_Enable_Chk, BST_CHECKED);
	CheckDlgButton(m_hDlg, IDC_Laser_Enable_Chk, BST_CHECKED);
	CheckDlgButton(m_hDlg, IDC_Camera_Enable_Chk, BST_CHECKED);
	CheckDlgButton(m_hDlg, IDC_Mic_Enable_Chk, BST_CHECKED);

	CheckDlgButton(m_hDlg, IDC_Robot_UseReal_Rdo, BST_CHECKED);
	CheckDlgButton(m_hDlg, IDC_Laser_UseReal_Rdo, BST_CHECKED);
	CheckDlgButton(m_hDlg, IDC_Camera_UseReal_Rdo, BST_CHECKED);
	CheckDlgButton(m_hDlg, IDC_Mic_UseReal_Rdo, BST_CHECKED);

	CheckDlgButton(m_hDlg, IDC_Robot_UseSim_Rdo, BST_UNCHECKED);
	CheckDlgButton(m_hDlg, IDC_Laser_UseSim_Rdo, BST_UNCHECKED);
	CheckDlgButton(m_hDlg, IDC_Camera_UseSim_Rdo, BST_UNCHECKED);
	CheckDlgButton(m_hDlg, IDC_Mic_UseSim_Rdo, BST_UNCHECKED);

	CheckDlgButton(m_hDlg, IDC_Robot_UseFile_Rdo, BST_UNCHECKED);
	CheckDlgButton(m_hDlg, IDC_Laser_UseFile_Rdo, BST_UNCHECKED);
	CheckDlgButton(m_hDlg, IDC_Camera_UseFile_Rdo, BST_UNCHECKED);
	CheckDlgButton(m_hDlg, IDC_Mic_UseFile_Rdo, BST_UNCHECKED);

	CheckDlgButton(m_hDlg, IDC_Robot_Record_Chk, BST_UNCHECKED);
	CheckDlgButton(m_hDlg, IDC_Laser_Record_Chk, BST_UNCHECKED);
	CheckDlgButton(m_hDlg, IDC_Camera_Record_Chk, BST_UNCHECKED);
	CheckDlgButton(m_hDlg, IDC_Mic_Record_Chk, BST_UNCHECKED);

	// module
	CheckDlgButton(m_hDlg, IDC_Navigator_Enable_Chk, BST_CHECKED);
	CheckDlgButton(m_hDlg, IDC_Speech_Enable_Chk, BST_UNCHECKED);
	CheckDlgButton(m_hDlg, IDC_Vision_Enable_Chk, BST_UNCHECKED);
	CheckDlgButton(m_hDlg, IDC_AI_Enable_Chk, BST_UNCHECKED);

	*/
	UpdateGUIData();
}

void ArmDialog::UpdateGUIData()
{
	/*
	// reset gui data
	guiData->isStart		= IsDlgButtonChecked(m_hDlg, IDC_Start_Chk);
	guiData->isRecord		= IsDlgButtonChecked(m_hDlg, IDC_Record_Chk);
	guiData->isManual		= IsDlgButtonChecked(m_hDlg, IDC_Manual_Chk);

	guiData->enableRobot	= IsDlgButtonChecked(m_hDlg, IDC_Robot_Enable_Chk);
	guiData->enableLaser	= IsDlgButtonChecked(m_hDlg, IDC_Laser_Enable_Chk);
	guiData->enableCamera	= IsDlgButtonChecked(m_hDlg, IDC_Camera_Enable_Chk);
	guiData->enableMic		= IsDlgButtonChecked(m_hDlg, IDC_Mic_Enable_Chk);

	if(IsDlgButtonChecked(m_hDlg, IDC_Robot_UseReal_Rdo)) guiData->sourceRobot = SourceType::USE_REAL;
	else if(IsDlgButtonChecked(m_hDlg, IDC_Robot_UseSim_Rdo)) guiData->sourceRobot = SourceType::USE_SIM;
	else if(IsDlgButtonChecked(m_hDlg, IDC_Robot_UseFile_Rdo)) guiData->sourceRobot = SourceType::USE_FILE;

	if(IsDlgButtonChecked(m_hDlg, IDC_Laser_UseReal_Rdo)) guiData->sourceLaser = SourceType::USE_REAL;
	else if(IsDlgButtonChecked(m_hDlg, IDC_Laser_UseSim_Rdo)) guiData->sourceLaser = SourceType::USE_SIM;
	else if(IsDlgButtonChecked(m_hDlg, IDC_Laser_UseFile_Rdo)) guiData->sourceLaser = SourceType::USE_FILE;

	if(IsDlgButtonChecked(m_hDlg, IDC_Camera_UseReal_Rdo)) guiData->sourceCamera = SourceType::USE_REAL;
	else if(IsDlgButtonChecked(m_hDlg, IDC_Camera_UseSim_Rdo)) guiData->sourceCamera = SourceType::USE_SIM;
	else if(IsDlgButtonChecked(m_hDlg, IDC_Camera_UseFile_Rdo)) guiData->sourceCamera = SourceType::USE_FILE;

	if(IsDlgButtonChecked(m_hDlg, IDC_Mic_UseReal_Rdo)) guiData->sourceMic = SourceType::USE_REAL;
	else if(IsDlgButtonChecked(m_hDlg, IDC_Mic_UseSim_Rdo)) guiData->sourceMic = SourceType::USE_SIM;
	else if(IsDlgButtonChecked(m_hDlg, IDC_Mic_UseFile_Rdo)) guiData->sourceMic	= SourceType::USE_FILE;

	guiData->recordRobot		= IsDlgButtonChecked(m_hDlg, IDC_Robot_Record_Chk);
	guiData->recordLaser		= IsDlgButtonChecked(m_hDlg, IDC_Laser_Record_Chk);
	guiData->recordCamera		= IsDlgButtonChecked(m_hDlg, IDC_Camera_Record_Chk);
	guiData->recordMic			= IsDlgButtonChecked(m_hDlg, IDC_Mic_Record_Chk);
															 
															 
	guiData->enableNavigator	= IsDlgButtonChecked(m_hDlg, IDC_Navigator_Enable_Chk);
	guiData->enableSpeech		= IsDlgButtonChecked(m_hDlg, IDC_Speech_Enable_Chk);
	guiData->enableVision		= IsDlgButtonChecked(m_hDlg, IDC_Vision_Enable_Chk);
	guiData->enableAI			= IsDlgButtonChecked(m_hDlg, IDC_AI_Enable_Chk);

	*/
}

bool ArmDialog::InitDialog()
{
	armDlgData->isKeyMove = 0;
	m_hInstance	= GetModuleHandle(NULL);
	m_hDlg		= CreateDialog(m_hInstance, MAKEINTRESOURCE(IDD_Arm_Dlg), NULL, (DLGPROC)MainDlgProc);
	if(!m_hDlg)
	{
		MessageBox(NULL, _T("Failed To Initialize MainGUI."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	SetWindowLong(m_hDlg, GWL_USERDATA, (LONG)this);

	/*
	greenBmp	= LoadBitmap( m_hInstance, MAKEINTRESOURCE(IDB_Green_Bmp) );
	redBmp		= LoadBitmap( m_hInstance, MAKEINTRESOURCE(IDB_Red_Bmp) );

	SendMessage(GetDlgItem(m_hDlg, IDC_Robot_Status_Btn), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)(greenBmp));
	SendMessage(GetDlgItem(m_hDlg, IDC_Laser_Status_Btn), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)(greenBmp));
	SendMessage(GetDlgItem(m_hDlg, IDC_Camera_Status_Btn), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)(redBmp));
	SendMessage(GetDlgItem(m_hDlg, IDC_Mic_Status_Btn), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)(redBmp));
	*/

	// Show Main Dialog
	ShowWindow(m_hDlg, SW_SHOW);

	return true;
}

bool CALLBACK ArmDialog::DlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	CvMat *mat;
	camera.HandleMessages(hDlg, message, wParam, lParam);
		int wmId;
	char buff[256];
	switch (message)
	{
	case WM_COMMAND:			
		wmId = LOWORD(wParam);
		switch(wmId)
		{
			case IDC_SIM1 :	
				cube->fAngle = fopen("img/savepos/Cube_final/cubeangle.in","r");
				
				for(int i=0;i<12;i++)
				{
					fscanf(cube->fAngle,"%f",&((cube->angle)[i]));
					cout << (cube->angle)[i] << endl;
					sprintf(buff,"img/savepos/Cube_final/%d.bmp",i);
					img[i] = cvLoadImage(buff);

					
				}
				cube->Test(img,12);
				break;

			case IDC_SIM2 :
				cube->fAngle = fopen("img/savepos/Octopus/cubeangle.in","r");
				for(int i=0;i<16;i++)
				{
					fscanf(cube->fAngle,"%f",&cube->angle[i]);
					sprintf(buff,"img/savepos/Octopus/%d.bmp",i);
					img[i] = cvLoadImage(buff);
					
				}
				cube->Test(img,16);

				break;
			case IDC_Draw_Mesh :
				armDlgData->isDrawMesh = !armDlgData->isDrawMesh;
				break;
			case IDC_Create_Mesh:
				mesh->CreateMesh(cube);
				break;
			case IDC_Re_Walk :
				fsetpos (walkPath, &filePosition);
				break;
			case IDC_Walk_Next :
				int c;
				fscanf(walkPath,"%d",&c);
				if(c==1)
					fscanf(walkPath,"%f %f %f %f %f",&(armConfig->ceta1),&(armConfig->ceta2),&(armConfig->ceta3),&(armConfig->ceta4),&(armConfig->ceta5));
				else
					cout << "End of path" << endl;
				break;
			case IDC_BUTTON_POSSAVE :
				fprintf(positionPath,"%f %f %f %f %f\n",armConfig->ceta1,armConfig->ceta2,armConfig->ceta3,armConfig->ceta4,armConfig->ceta5);
				break;
			case IDC_BUTTON_CUBE :
				sprintf(filename,"img/savepos/%d.bmp",numsave++);
				cvSaveImage(filename,cube->image);
				cube->Execute();
				break;
			case IDC_CHECK_HS : armDlgData->hasObj		= !armDlgData->hasObj; break;
			case IDC_CHECK_NO : armDlgData->noObj		= !armDlgData->noObj; break;
			case IDC_CHECK_UN : armDlgData->unknownObj	= !armDlgData->unknownObj; break;
			case IDC_Scan :	if(armDlgData->isScanning)
							{
								armDlgData->isScanning = false;
								SetDlgItemText(m_hDlg,IDC_Scan,L"Scan");
							}
							else
							{
								armDlgData->isScanning = true;
								SetDlgItemText(m_hDlg,IDC_Scan,L"Stop Scan");
							}
			case IDC_BUTTON_IMG1 : 
								
								//mat = visionUtil.CutGreenBackground(cameraMan->image);
								//cvReleaseMat(&mat);
								//break;
								stereo->collectImage1(cameraMan->image);	break;
			case IDC_BUTTON_IMG2 : stereo->collectImage2(cameraMan->image);	break;
			case IDC_BUTTON_STEEXE :  stereo->Execute();	break;
			case IDC_BUTTON_SURF :
				surf->Execute(cameraMan->image);
				break;
			case IDC_BUTTON_TESTCHESS :							  
							   chess.FindChessboard();
							   break;
			case IDC_TESTCALI : calibrate->TestCalibrate(); break;
			case IDC_CALIBRATE : calibrate->CalibrateLiveCamera(12); break;
			case IDC_PROJECT :	CalculateEndEffectorPosition(); 
							//	calibrate->ShowImageFromProjection(0,5,allMat,CalculateEndEffectorPosition());	
								break;
			case IDC_CHECK_STARTKEY : 
				if(armDlgData->isKeyMove==0) armDlgData->isKeyMove=1;
				else if(armDlgData->isKeyMove==1) armDlgData->isKeyMove=0;
				break;
			case IDC_CALC : CalculateEndEffectorPosition(); break;		
			case IDC_MOVEARM : armConnector->WriteCommand(armConfig->ceta1,armConfig->ceta2,armConfig->ceta3,armConfig->ceta4,armConfig->ceta5,300); break;
			case IDC_TEST_ARM_ACTIVE : armConnector->ReadVersion(); break;
			case IDC_ISMOVE : armConnector->IsArmMoving(); break;
			
			case IDC_GOTO : /*
				char szString[80];
				GetDlgItemText(hDlg, IDC_EDIT_X, szString, 80);
				cout << szString << endl;*/
				cout << armConfig->ceta1 << endl;
				cout << armConfig->ceta2 << endl;
				cout << armConfig->ceta3 << endl;
				//armCal.CalculateInverseKinematic(targetX,targetY,targetZ-armConfig->baseHeight,armConfig->foreArmLength,armConfig->armLength,&(armConfig->ceta1),&(armConfig->ceta2),&(armConfig->ceta3));
				cout << armConfig->ceta1 << endl;
				cout << armConfig->ceta2 << endl;
				cout << armConfig->ceta3 << endl;
				break;
			default : break;
		}
		break;
	case WM_KEYDOWN:										// Is A Key Being Held Down?
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);								// Send A Quit Message
		break;
	case WM_CLOSE:
		DestroyWindow(m_hDlg);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return false;
}


void ArmDialog::DrawTrianglePlane(mVector3f e1,mVector3f e2,mVector3f e3)
{
	GLRender3D::RenderTriangle(e1,e2,e3);
}

void ArmDialog::DrawArm(float length){
	glEnable(GL_LIGHTING);
	GLRender3D::SetMaterial(GLDraw::C_WHITE);
	glColor4fv(GLDraw::C_LIGHT_GRAY);
	glRotatef(90,0.0f,1.0f,0.0f);			// Rotate The Cube On X, Y & Z
	GLRender3D::RenderCylinder(1.0f, 1.0f, length);
	glRotatef(-90,0.0f,1.0f,0.0f);			// Rotate The Cube On X, Y & Z
	glDisable(GL_LIGHTING);
}

void ArmDialog::DrawGrip(){
	glEnable(GL_LIGHTING);
	GLRender3D::SetMaterial(GLDraw::C_ORANGE);
	glColor4fv(GLDraw::C_ORANGE);
	glRotatef(90.0f,1.0f,0.0f,0.0f);
	glTranslatef(0.0f,0.0f,-armConfig->gripWidth/2);
	GLRender3D::RenderCylinder(1.0f, 1.0f, armConfig->gripWidth);
	glTranslatef(0.0f,0.0f,armConfig->gripWidth/2);
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	glDisable(GL_LIGHTING);
}


void ArmDialog::Swap(float *a,float *b){
	float tmp;
	tmp = *a;
	*a = *b;
	*b = *a;
}

void ArmDialog::DrawAxis(){
	glBegin(GL_LINES);		// Drawing Using Line
	glColor4fv(GLDraw::C_BLUE);
	glVertex3f(0.0f, 0.0f, 5.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	GLDraw::Print( "z" );
	glColor4fv(GLDraw::C_GREEN);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	GLDraw::Print( "y" );
	glColor4fv(GLDraw::C_RED);
	glVertex3f(5.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	GLDraw::Print( "x" );
	glEnd();
}
void CalculateInverseKinematic(mVector3f dest){
}

mVector3f ArmDialog::CalculateEndEffectorPosition(){
	
	Matrix44f mTmp;

	mTmp.Identity(&mTmp);
	mat0.Identity(&mat0);	
	mat1.Identity(&mat1);
	mat2.Identity(&mat2);	
	mat3.Identity(&mat3);
	mat4.Identity(&mat4);
	mat5.Identity(&mat5);
	mat6.Identity(&mat6);
	mat7.Identity(&mat7);
	mat8.Identity(&mat8);

	// Initial
	Matrix44f::Translation(&mTmp,6.0f,34.3f,armConfig->baseHeight);
	mat0 *= mTmp;
	Matrix44f::RotationZ(&mTmp,90.0f/180*3.14); 
	mat0 *= mTmp;

	// 0 to 1
	Matrix44f::RotationZ(&mTmp,armConfig->ceta1/180*3.14); 
	mat0 *= mTmp;

	// 1 to 2
	Matrix44f::RotationX(&mTmp,90.0f/180*3.14); 
	mat1 *= mTmp;

	Matrix44f::RotationZ(&mTmp,(armConfig->ceta2)/180*3.14);
	mat1 *= mTmp;

	// 2 to 3
	Matrix44f::Translation(&mTmp,armConfig->foreArmLength,0.0f,0.0f);
	mat2 *= mTmp;
	Matrix44f::RotationZ(&mTmp,(armConfig->ceta3)/180*3.14);
	mat2 *= mTmp;
	
	// 3 to 4
	Matrix44f::Translation(&mTmp,armConfig->armLength,0.0f,0.0f);
	mat3 *= mTmp;
	Matrix44f::RotationZ(&mTmp,(armConfig->ceta4)/180*3.14);
	mat3 *= mTmp;

	// 4 to 5 (camera)
	Matrix44f::Translation(&mTmp,armConfig->handLength,0,0);
	mat4 *= mTmp;
	Matrix44f::RotationZ(&mTmp,30.0f/180.0f*3.14f);
	mat4 *= mTmp;

	mVector3f position = mat0 * mat1 * mat2 * mat3 * mat4 * mVector3f(0,0,0.0f);
	allMat = mat0 * mat1 * mat2 * mat3 * mat4;


	char buf[30];
	TCHAR tchar[30];
	_snprintf(buf, 30, "x=%3.2f  y=%3.2f  z=%3.2f ", position.x,position.y,position.z);
	ConvertCHARToTCHAR(buf,tchar,30);
	SetDlgItemText(m_hDlg,IDC_POS,tchar);

	return position;

}


void ArmDialog::UpdateText(){
	char buf[10];
	TCHAR tchar[10];
	_snprintf(buf, 10, "%3.2f", armConfig->ceta1);
	ConvertCHARToTCHAR(buf,tchar,10);
	SetDlgItemText(m_hDlg,IDC_T1,tchar);
	_snprintf(buf, 10, "%3.2f", armConfig->ceta2);
	ConvertCHARToTCHAR(buf,tchar,10);
	SetDlgItemText(m_hDlg,IDC_T2,tchar);
	_snprintf(buf, 10, "%3.2f", armConfig->ceta3);
	ConvertCHARToTCHAR(buf,tchar,10);
	SetDlgItemText(m_hDlg,IDC_T3,tchar);
	_snprintf(buf, 10, "%3.2f", armConfig->ceta4);
	ConvertCHARToTCHAR(buf,tchar,10);
	SetDlgItemText(m_hDlg,IDC_T4,tchar);
	_snprintf(buf, 10, "%3.2f", armConfig->ceta5);
	ConvertCHARToTCHAR(buf,tchar,10);
	SetDlgItemText(m_hDlg,IDC_T5,tchar);
}

void ArmDialog::DrawObject()
{
		CvMat *pointList = stereo->pointCloud;
		if(pointList==0) return;
		CvSize pointListSize = {pointList->width,pointList->height};
		for(int y = 0; y < pointListSize.height; y+=5 )
		{
			float* dptr0 = (float*)(pointList->data.ptr + pointList->step*y), *dptr = dptr0;
			 for(int x = 0; x < pointListSize.width ; x+=5)
			 {
	//			cout << dptr[x*3] << "," << dptr[x*3+1] << "," << dptr[x*3+2] << endl;
				glLoadIdentity();		
				glTranslatef(dptr[x*3]/100.0,dptr[x*3+1]/100.0,(dptr[x*3+2]-297)/100.0);	
				GLRender3D::RenderSphere(0.3);
			 }
		}
	return;
}

void ArmDialog::DrawCube()
{
	int n = cube->n_points;
	CvPoint3D32f  t;
	for(int i=0;i<n;i++)
	{
		t = cube->pointList[i];
		//if(t.z!=0) continue;
		glLoadIdentity();	
		glRotatef(180.00,1.0f,0.0f,0.0f);
		glTranslatef(t.x,t.y,t.z);	
		if(cube->status[i]==0) // unknown
		{		
			if(armDlgData->unknownObj)
			{
				glColor4fv(GLDraw::C_GRAY);
				GLRender3D::RenderSphere(0.1f);
			}
		}
		else if(cube->status[i]==1) // has obj
		{
			if(armDlgData->hasObj)
			{
				glColor4fv(GLDraw::C_WHITE);
				GLRender3D::RenderSphere(0.1f);
			}
		}
		else if(cube->status[i]==-1) // no obj
		{
			if(armDlgData->noObj)
			{
				glColor4fv(GLDraw::C_CYAN);
				GLRender3D::RenderSphere(0.1f);
			}
		}
	}
	/*for(int i=n+1,j=0;i<n+48;i++,j++)
	{
		if(j==4) continue;
		t = cube->pointList[i];
		glLoadIdentity();	
		glTranslatef(t.x,t.y,t.z);	
		glColor4fv(GLDraw::C_CYAN);
		GLRender3D::RenderSphere(0.1);
	}*/

}

void ArmDialog::Render()
{
	mVector3f position = CalculateEndEffectorPosition();
	GLApp::SelectWindow(&glApp);
	
//	calibrate->ShowImageFromProjection(0,5,allMat,CalculateEndEffectorPosition(),allMat);	
	RECT rect;
	GetClientRect( glApp.m_hWnd, &rect );
	int wWidth = rect.right - rect.left;
	int wHeight = rect.bottom - rect.top;

	keyboard->ReadImmediateData( );

	if(keyboard->keyboardState[DIK_U])
		armConfig->ceta1 += 1.0;
	if(keyboard->keyboardState[DIK_J])
		armConfig->ceta1 -= 1.0;
	if(keyboard->keyboardState[DIK_I])
		armConfig->ceta2 += 1.0;
	if(keyboard->keyboardState[DIK_K])
		armConfig->ceta2 -= 1.0;
	if(keyboard->keyboardState[DIK_O])
		armConfig->ceta3 += 1.0;
	if(keyboard->keyboardState[DIK_L])
		armConfig->ceta3 -= 1.0;
	if(keyboard->keyboardState[DIK_P])
		armConfig->ceta4 += 1.0;
	if(keyboard->keyboardState[DIK_SEMICOLON])
		armConfig->ceta4 -= 1.0;
	if(keyboard->keyboardState[DIK_LBRACKET])
		armConfig->ceta5 += 1.0;
	if(keyboard->keyboardState[DIK_APOSTROPHE])
		armConfig->ceta5 -= 1.0;
	
	if(keyboard->keyboardState[DIK_Q])
		targetX += 1.0;
	if(keyboard->keyboardState[DIK_A])
		targetX -= 1.0;
	if(keyboard->keyboardState[DIK_W])
		targetY += 1.0;
	if(keyboard->keyboardState[DIK_S])
		targetY -= 1.0;
	if(keyboard->keyboardState[DIK_E])
		targetZ += 1.0;
	if(keyboard->keyboardState[DIK_D])
		targetZ -= 1.0;
	
	UpdateText();

	

	if(keyboard->keyboardState[DIK_F])
	{
		// set view
		Matrix44f::Translation(&lookAt,1.0f,0.0f,0.0f);
		Matrix44f::Translation(&upVec,0.0f,-1.0f,0.0f);

		//cout << "press f " << endl;
		camera2.m_vPosition.x = position.x;
		camera2.m_vPosition.y = position.y;
		camera2.m_vPosition.z = position.z;
	//	camera2.m_vPosition = mat0 * mat1 * mVector3f(0.0f,0.0f,0.0f);
		mVector3f lookAtPos = allMat * lookAt * mVector3f(0.0f,0.0f,0.0f);
		mVector3f upVecPos = allMat * upVec * mVector3f(0.0f,0.0f,0.0f);
		camera2.m_vLookAt.x = lookAtPos.x;
		camera2.m_vLookAt.y = lookAtPos.y;
		camera2.m_vLookAt.z = lookAtPos.z;
		camera2.m_upVec.x = upVecPos.x - position.x;
		camera2.m_upVec.y = upVecPos.y - position.y;
		camera2.m_upVec.z = upVecPos.z - position.z;
	}
	

	
	camera.m_bPressCtrl = keyboard->keyboardState[DIK_LCONTROL] || keyboard->keyboardState[DIK_RCONTROL];
	camera.FrameMove( );

	
	float aspect_ratio = (float)wWidth/max(1, wHeight);
	GLApp::GetPerspectiveMat(&projMat, GLApp::g_fFovY*G_PI/180, aspect_ratio, GLApp::g_fNearClippingPlane, GLApp::g_fFarClippingPlane );
	if(keyboard->keyboardState[DIK_F])
		GLApp::GetLookAtMat(&viewMat, camera2.m_vPosition, camera2.m_vLookAt, camera2.m_upVec);
	else
		GLApp::GetLookAtMat(&viewMat, camera.m_vPosition, camera.m_vLookAt, camera.m_upVec);
	Matrix44f mat = projMat*viewMat;
	GLApp::SetRender3D( wWidth, wHeight, &mat );		// set to render 3D

	// clear framebuffer
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

	glLoadIdentity();
	glColor4fv(GLDraw::C_ORANGE);
	//GLRender3D::RenderPlane(mVector3f(0.0f,0.0f,0.0f),mVector3f(39.7f,0.0f,0.0f),mVector3f(39.7f,38.7f,0.0f),mVector3f(0.0f,39.7,0.0f));

	glColor4fv(GLDraw::C_PINK);
	if(armDlgData->isDrawMesh)
		mesh->drawMesh();
	
	glColor4fv(GLDraw::C_PINK);
	//DrawObject();

	glLoadIdentity();
	float x = CV_MAT_ELEM(*cube->cameraMatrix_inv, float, 0, 3);
	float y = CV_MAT_ELEM(*cube->cameraMatrix_inv, float, 1, 3);
	float z = CV_MAT_ELEM(*cube->cameraMatrix_inv, float, 2, 3);
	glTranslatef(x,y,z);
	glColor4fv(GLDraw::C_PINK);
	GLRender3D::RenderSphere();

	DrawCube();

	glLoadIdentity();		
	DrawAxis();
	GLApp::SetGLMatrix(&mat0);
	glEnable(GL_LIGHTING);
	GLRender3D::SetMaterial(GLDraw::C_WHITE);
	glTranslatef(0.0f,0.0f,-armConfig->baseHeight);
	//GLRender3D::RenderCapCylinder(armConfig->baseRadius,armConfig->baseRadius,armConfig->baseHeight);
	glTranslatef(0.0f,0.0f,armConfig->baseHeight);
	glDisable(GL_LIGHTING);
	DrawAxis();

	GLApp::SetGLMatrix(&mat1);
	DrawAxis();	
	//DrawArm(armConfig->foreArmLength);

	GLApp::SetGLMatrix(&mat2);
	DrawAxis();
	//DrawArm(armConfig->armLength);

	GLApp::SetGLMatrix(&mat3);
	DrawAxis();
	//DrawArm(armConfig->handLength);

	GLApp::SetGLMatrix(&mat4);
	DrawAxis();

	glLoadIdentity();	
	glTranslatef(targetX,targetY,targetZ);
	GLRender3D::RenderSphere();

	//for(int i = -10; i <= 10; i+=4)
	//{
	//	for(int j = -10; j <= 10; j+=4)
	//	{
	//		glColor4fv(GLDraw::C_BLUE);
	//		//GLRender3D::SetMaterial(GLDraw::C_BLUE);
	//		glLoadIdentity();	
	//		glTranslatef(i*10,j*10,100);
	//		GLRender3D::RenderSphere();
	//		glLoadIdentity();	
	//		glTranslatef(i*10,j*10,-100);
	//		GLRender3D::RenderSphere();

	//		glColor4fv(GLDraw::C_GREEN);
	//		//GLRender3D::SetMaterial(GLDraw::C_GREEN);
	//		glLoadIdentity();	
	//		glTranslatef(i*10,100,j*10);
	//		GLRender3D::RenderSphere();
	//		glLoadIdentity();	
	//		glTranslatef(i*10,-100,j*10);
	//		GLRender3D::RenderSphere();

	//		glColor4fv(GLDraw::C_RED);
	//		//GLRender3D::SetMaterial(GLDraw::C_RED);
	//		glLoadIdentity();	
	//		glTranslatef(100,i*10,j*10);
	//		GLRender3D::RenderSphere();
	//		glLoadIdentity();	
	//		glTranslatef(-100,i*10,j*10);
	//		GLRender3D::RenderSphere();
	//	}
	//}
	// switch to render 2D
	GLApp::SetRender2D( wWidth, wHeight );
	
	// draw Axis
	float axisPanelWidth = 64;
	float axisPanelHeight = 64;
	float incX = wWidth - axisPanelWidth;
	float incY = wHeight - axisPanelHeight;
	Matrix44f rotViewMat = viewMat;
	rotViewMat.m14 = 0; rotViewMat.m24 = 0; rotViewMat.m34 = -3.5f;
	mVector2f po = GLApp::WorldToImage(0, 0, 0, axisPanelWidth, axisPanelHeight, &projMat, &rotViewMat);
	mVector2f px = GLApp::WorldToImage(1, 0, 0, axisPanelWidth, axisPanelHeight, &projMat, &rotViewMat);
	mVector2f py = GLApp::WorldToImage(0, 1, 0, axisPanelWidth, axisPanelHeight, &projMat, &rotViewMat);
	mVector2f pz = GLApp::WorldToImage(0, 0, 1, axisPanelWidth, axisPanelHeight, &projMat, &rotViewMat);
	
	glBegin(GL_LINES);		// Drawing Using Line
	glColor4fv(GLDraw::C_RED);
	glVertex2f(incX + po.x, incY + po.y);
	glVertex2f(incX + px.x, incY + px.y);
	glColor4fv(GLDraw::C_GREEN);
	glVertex2f(incX + po.x, incY + po.y);
	glVertex2f(incX + py.x, incY + py.y);
	glColor4fv(GLDraw::C_BLUE);
	glVertex2f(incX + po.x, incY + po.y);
	glVertex2f(incX + pz.x, incY + pz.y);
	glEnd();
	
	glColor4fv(GLDraw::C_RED);
	glRasterPos2f( incX + px.x, incY + px.y ); GLDraw::Print( "x" );
	glColor4fv(GLDraw::C_GREEN);
	glRasterPos2f( incX + py.x, incY + py.y ); GLDraw::Print( "y" );
	glColor4fv(GLDraw::C_BLUE);
	glRasterPos2f( incX + pz.x, incY + pz.y ); GLDraw::Print( "z" );

	glFlush();
	SwapBuffers( glApp.m_hDC );
}
