#include "MCube.h"

MCube::MCube()
{	

	SIZE = 100;
    DEFAULT_SEEK_VALUE = 0.0;
    seekValue = DEFAULT_SEEK_VALUE;
	ambLen = 60;
	vLen = 8;



	

}
MCube::~MCube()
{
}

    void MCube::initialize(int *_ambigous,int *_faces)
	{
		ambigous = _ambigous;
		faces = _faces;
	}


    void MCube::MCCube1(int _id, MVertex v0, MVertex v1, MVertex v2, MVertex v3, MVertex v4, MVertex v5, MVertex v6, MVertex v7)
    {
        id = _id;
        
        v = new MVertex[8];
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        v[3] = v3;
        v[4] = v4;
        v[5] = v5;
        v[6] = v6;
        v[7] = v7;
        
        e = new mVector3f[12];
        
        
        //this.neighbours = new Vector(0, 1);
        
        computeEdges();
    }
    
    /**
     * constructor of a MCCube object
     * @param id identifier of the cube
     * @param v array containing the vertexes
     */
    void MCube::MCCube1(int _id, MVertex *_v)
    {
        id = _id;
        v = _v;
        e = new mVector3f[12];
        computeEdges();
    }


   /**
     * indicates if a number corresponds to an ambigous case
     * @param n number of the case to test
     * @return true if the case if ambigous
     */
    bool MCube::isAmbigous(int n)
    {
        bool result = false;
		for (int index = 0; index < ambLen; index++)
        {
            result |= ambigous[index] == n;
        }
        return result;
    }
    
    /**
     * computes the case number of the cube
     * @return the number of the case corresponding to the cube
     */
    int MCube::caseNumber()
    {
        int caseNumber = 0;
        for (int index = -1; ++index < vLen; caseNumber += ((v[index].weight) - seekValue > 0)?1 << index:0);
		//{
			//cout << index << " : " << v[index].weight << endl;

		//}
        return caseNumber;
    }

	 /**
     * computes the interpolated point along a specified whose weight equals the reference value
     * @param v1 first extremity of the edge
     * @param v2 second extremity of the edge
     * @return the point on the edge where weight equals the isovalue; null is interpolated point is beyond edge boundaries
     */
    mVector3f MCube::computeEdge(MVertex v1, MVertex v2)
    {
		double t = (seekValue - (v1.weight))/(double) ((v2.weight) - (v1.weight));
        if (t >= 0 && t <= 1)
        {
            mVector3f vDir = v2.position -   v1.position;
            return v1.position + vDir*t;
			
        }
		return mVector3f(0,0,0);
    }

	  /**
     * computes interpolated values along each edge of the cube (null if interpolated value doesn't belong to the edge)
     */
    void MCube::computeEdges()
    {
        e[0] = computeEdge(v[0], v[1]);
        e[1] = computeEdge(v[1], v[2]);
        e[2] = computeEdge(v[2], v[3]);
        e[3] = computeEdge(v[3], v[0]);
        
        e[4] = computeEdge(v[4], v[5]);
        e[5] = computeEdge(v[5], v[6]);
        e[6] = computeEdge(v[6], v[7]);
        e[7] = computeEdge(v[7], v[4]);
        
        e[8] = computeEdge(v[0], v[4]);
        e[9] = computeEdge(v[1], v[5]);
        e[10] = computeEdge(v[3], v[7]);
        e[11] = computeEdge(v[2], v[6]);
    }

  /**
     * draws triangles between interpolated values along the edges
     */
    void MCube::drawTriangles()
    {
	
		//cout << "id : " << id << endl;
        int cn = caseNumber();
		//cout << "case number :" <<  cn << endl;
		//for(int i=0;i<8;i++)
		//	cout << v[i].position << endl;
		//cout << "---" << endl;
        bool directTable = !(isAmbigous(cn) && !amb);
		mVector3f e1,e2,e3;
        // address in the table
        int offset = (directTable)?cn*15:(255-cn)*15;
        for (int index = 0; index < 5; index++)
        {
            // if there's a triangle
            if (faces[offset] != -1)
            {
                // pick up vertexes of the current triangle
				e1 = e[faces[offset + 0]];
                e2 = e[faces[offset + 1]];
                e3 = e[faces[offset + 2]];  
				glEnable(GL_LIGHTING);
				glLoadIdentity();	
				glRotatef(180.00,1.0f,0.0f,0.0f);
              	GLRender3D::RenderTriangle(e1,e2,e3);
				glDisable(GL_LIGHTING);
            }
            offset += 3;
        }
    }

	/**
     * draws the vertexes of the cubes
     * @param v array containing the vertexes transformed by the matrix stack
     */
    //void MCube::drawVertexes(mVector3f *v)
    //{
    //    // vertexes of the cube
    //    for (int index = 0; index < vLen; index++)
    //    {
    //        //// modify the color using the vertex weight
    //        //int c = (int) (127*this.v[index].weight()/(double) MCApplet.DEFAULT_RANGE);
    //        //MCRenderer.g.setColor(MCRenderer.vertexColor(c));
    //        //// draws the circle
    //        //MCRenderer.drawCircle(new MCVector4(v[index]), MCVertex.SIZE);
    //        //MCRenderer.g.setColor(MCRenderer.TEXT_COLOR);
    //        //// and then its number
    //        //MCRenderer.drawString(v[index].add(new MCVector3(0, -MCVertex.SIZE, 0)), (new Integer(index)).toString());
    //    }
    //    
    //    // interpolated vertexes along the edges
    //    ;//MCRenderer.g.setColor(MCRenderer.I_VERTEX_COLOR);
    //    for (int index = 0; index < 12; index++)
    //    {
    //        if (e[index] != 0)
    //        {
    //           ;// MCRenderer.drawCircle(MCCanvas.matrixStack.peek().mult(new MCVector4(this.e[index])), MCVertex.SIZE);
    //        }
    //    }
    //}