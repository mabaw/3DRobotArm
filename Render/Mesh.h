#ifndef _MESH_H_
#define _MESH_H_

#include <iostream>
#include <vector>
using namespace std;

#include "MCube.h"
//#include "MVertex.h"
#include "yMath\yMath.h"
#include "..\vision\Cube.h"
class Mesh {
	private:
	

	public:			
		Mesh();
		~Mesh();
		MCube *cubesList;
		int size;
		

		void addCube(int x,int y,int z);
		void CreateMesh(Cube *cube);
		void drawMesh();

		
		int *ambigous;
		int *faces;
		int countadd;


};

#endif // _MESH_H_