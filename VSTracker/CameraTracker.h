#ifndef _CAMERA_TRACKER_H_
#define _CAMERA_TRACKER_H_

// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// includes, GL
//#include <GL/glew.h>
//#include <GL/glut.h>

// includes, CV
#include "cxcore.h"
#include "cv.h"
#include "highgui.h"

//#include "isltMath.h"
#include "VS_wrap.h"

#include <vector>

using namespace std;


double g_prj[16]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
double g_mdl[16]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

class CameraTracker {
	private:

	public:		
		

		CameraTracker();
		~CameraTracker();
		bool Initialize();
		bool resetTracker( );
		void getCrosses( );
		void tracker_callback(void* data, IplImage* src, const double* prj, const double* mdl);

		TrackerID g_tracker;
		std::vector<double> g_cross_points;
		std::vector<unsigned long> g_cross_codes;
		int g_window_width;
		int g_window_height;
		char *g_input_lut;
		char *g_input_rs;
		char *g_input_ip;
		
		IplImage* g_img;

	
};

#endif // _SURF_H_