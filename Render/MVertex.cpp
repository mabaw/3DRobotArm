#include "MVertex.h"


MVertex::MVertex()
{
}

MVertex::~MVertex()
{


}

MVertex::MVertex(mVector3f _position, int _weight)
{	
	DEFAULT_WEIGHT = 0;
	SIZE = 24;	 
	position = _position;
    weight = _weight;  
}


/**
 * draw the vertex into a specified graphics
 * @param g the graphics in which the vertex is to be drawn
 */
void MVertex::display()
{        
  //  MCRenderer.drawCircle(new MCVector4(this.position), SIZE);        
}
