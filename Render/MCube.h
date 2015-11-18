#ifndef _MCUBE_H_
#define _MCUBE_H_


#include <iostream>
#include <vector>
#include "MVertex.h"
#include "yOpenGL\GLApp.h"
using namespace std;
class MCube {
	private:
	
	public:		


		MCube();
		~MCube();
		void initialize(int *_ambigous,int *_faces);
		void MCCube1(int _id);
		void MCCube1(int _id, MVertex v0, MVertex v1, MVertex v2, MVertex v3, MVertex v4, MVertex v5, MVertex v6, MVertex v7);
		void MCCube1(int _id, MVertex *_v);
		bool isAmbigous(int n);
		int caseNumber();
		mVector3f computeEdge(MVertex v1, MVertex v2);
		void computeEdges();
		void drawTriangles();

		double SIZE;
		double DEFAULT_SEEK_VALUE;
		double seekValue;
		MVertex *v;
		mVector3f *e;    
		int id;
		bool hidden;
		bool amb;
		int ambLen;
		int vLen;
		int *ambigous;
		int *faces;
		


};

#endif // _MCUBE_H_