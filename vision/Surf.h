#ifndef _SURF_H_
#define _SURF_H_

#include <cv.h>
#include <highgui.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define NUMOBJECT 3

class Surf {
	private:
		IplImage* objects[NUMOBJECT];

	public:		
		

		Surf();
		~Surf();
		bool Initialize();
		void Execute(IplImage *img);
		void LoadObject();
		void Compare(IplImage* object,IplImage* image);

		double compareSURFDescriptors( const float* d1, const float* d2, double best, int length );
		int naiveNearestNeighbor( const float* vec, int laplacian,
			const CvSeq* model_keypoints,const CvSeq* model_descriptors );
		void findPairs( const CvSeq* objectKeypoints, 
			const CvSeq* objectDescriptors,
           const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, 
		   vector<int>& ptpairs );
		void flannFindPairs( const CvSeq*, const CvSeq* objectDescriptors,
           const CvSeq*, const CvSeq* imageDescriptors, vector<int>& ptpairs );
		int locatePlanarObject( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
                    const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
                    const CvPoint src_corners[4], CvPoint dst_corners[4] );
};

#endif // _SURF_H_