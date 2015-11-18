#ifndef _MVERTEX_H_
#define _MVERTEX_H_

#include "yMath\yMath.h"

class MVertex {
	private:
	
	public:		
		
	
		int DEFAULT_WEIGHT;
		int SIZE;	    
		int weight;	    
		mVector3f position;    
	
		MVertex(mVector3f _position, int _weight);
		MVertex();
		~MVertex();
		//void MCVertex1(mVector3f _position, int _weight);

	    void display();

};

#endif // _VISION_UTIL_H_