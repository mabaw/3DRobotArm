#include "Mesh.h"

Mesh::Mesh()
{	
	FILE *f = fopen("ambigous.in","r");
	int d,i;
	ambigous = new int[60];
	faces = new int[3840];
	fscanf(f,"%d",&d);
	i=0;
	while(d!=0)
	{
		ambigous[i++] = d;
		fscanf(f,"%d",&d);
	}
	fclose(f);

	f = fopen("faces.in","r");
	fscanf(f,"%d",&d);
	i=0;
	while(d!=300)
	{
		faces[i++] = d;
		fscanf(f,"%d",&d);
	}
	fclose(f);
}
Mesh::~Mesh()
{
}

void Mesh::CreateMesh(Cube *cube)
{	
		MVertex v0,v1,v2,v3,v4,v5,v6,v7;
		cubesList = new MCube[cube->n_points];

		mVector3f end = mVector3f(cube->origin.x+cube->size,cube->origin.y+cube->size,cube->origin.z+cube->size);
		int cx,cy,cz,count;
		int n=cube->numside;
		size=cube->n_points;
		cx=0;cy=0;cz=0;
		count=0;
		int xn = n*n;
		for(float x=cube->origin.x;x<=end.x;x+=cube->sideWidth)
		{
			cy=0;
			for(float y=cube->origin.y;y<=end.y;y+=cube->sideWidth)
			{
				cz=0;
				for(float z=cube->origin.z;z<=end.z;z+=cube->sideWidth)
				{
					/*if(count==4) 
						cout << "xx" <<endl;
					cout << x << " " << y  << " " <<  z << endl;*/
					v0 = MVertex(mVector3f(x,y,z),100*cube->getStatus(count));
					v1 = MVertex(mVector3f(x+cube->sideWidth,y,z),100*(cube->getStatus(xn*(cx+1)+n*cy+cz)));
					v2 = MVertex(mVector3f(x+cube->sideWidth,y,z+cube->sideWidth),100*cube->getStatus(xn*(cx+1)+n*cy+cz+1));
					v3 = MVertex(mVector3f(x,y,z+cube->sideWidth),100*cube->getStatus(xn*cx+n*cy+cz+1));
					v4 = MVertex(mVector3f(x,y+cube->sideWidth,z),100*cube->getStatus(xn*cx+n*(cy+1)+cz));
					v5 = MVertex(mVector3f(x+cube->sideWidth,y+cube->sideWidth,z),100*cube->getStatus(xn*(cx+1)+n*(cy+1)+cz));
					v6 = MVertex(mVector3f(x+cube->sideWidth,y+cube->sideWidth,z+cube->sideWidth),100*cube->getStatus(xn*(cx+1)+n*(cy+1)+cz+1));
					v7 = MVertex(mVector3f(x,y+cube->sideWidth,z+cube->sideWidth),100*cube->getStatus(xn*cx+n*(cy+1)+cz+1));
					cz++;
					MCube cube;// = new MCube();
					//cout << count << endl;
					cube.MCCube1(count,v0,v1,v2,v3,v4,v5,v6,v7);
					cube.computeEdges();
					cube.initialize(ambigous,faces);
					cubesList[count] = cube;
					
					count++;
					
					/*it = cubes.begin();
					cubes.insert(it,cube);*/
				}
				cy++;
			}	
			cx++;
		}
		countadd = count-1;
		//cout << "added. " <<count << endl;
}


void Mesh::addCube(int x,int y,int z)
{

}

void Mesh::drawMesh()
{
	
	for (int i=0;i<countadd;i++)
	{
		//if(i==4)
		cubesList[i].drawTriangles();
		//if(i==4) break;
	}
}