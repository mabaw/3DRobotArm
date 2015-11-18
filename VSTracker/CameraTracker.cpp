#include "CameraTracker.h"




// define whether to use approximate nearest-neighbor search
#define USE_FLANN

CameraTracker::CameraTracker()
{	
	 g_tracker = 0;
	 g_window_width = 512;
	 g_window_height = 512;
	 g_input_lut = new char[30];
	 g_input_rs = new char[30];
	 g_input_ip = new char[30];
	 g_input_lut    =	 "cfg/hinge8x8.txt";
	 g_input_rs     =	 "cfg/hinge.rs.txt";
	 g_input_ip 	=    "cfg/ip.txt";
	 g_img = 0;


}
CameraTracker::~CameraTracker()
{
}


void CameraTracker::getCrosses( )
{
	int nPts = vsTrackerGetTemplateData(g_tracker, 0, 0);
	g_cross_points.resize(nPts*5*3);
	g_cross_codes.resize(nPts);
	vsTrackerGetTemplateData(g_tracker, &g_cross_points[0], &g_cross_codes[0]);
}

// tracker's call back
void CameraTracker::tracker_callback(void* data, IplImage* src, const double* prj, const double* mdl)
{
	printf("GetImage %d\n", (void*)src);
	g_img = src;
	memcpy(g_prj, prj, sizeof(double)*16);
	memcpy(g_mdl, mdl, sizeof(double)*16);
}

bool CameraTracker::resetTracker( )
{
	if (g_tracker) 
	{
		vsTrackerRelease(g_tracker);
	}
	g_tracker = vsTrackerCreate
	(	g_window_width
	,	g_window_height
	,	g_input_lut
	,	g_input_rs
	,	g_input_ip
	,	true
	);
	if (g_tracker)
	{
		vsTrackerSetCallback(g_tracker, tracker_callback, 0);
	}
	else
	{
		fprintf(stderr, "tracker creation fail\n");
		return false;
	}
	g_img = 0;	// no input, for now

	getCrosses( );

	return true;
}


bool CameraTracker::Initialize(){
	
	return true;
}
