#ifndef _FIND_TAG_NUMBER_H_
#define _FIND_TAG_NUMBER_H_

#include <stdio.h>
#include <iostream>
#include "cv.h"
#include "cxmisc.h"
#include "highgui.h"
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>

using namespace std;

class FindTagNumber {

	private:
		//IplImage	*image;
		int sthreshold;
		double hlower;
		double hupper;

	public:		
		

		FindTagNumber();
		~FindTagNumber();
		bool Initialize();

		bool Execute(IplImage *image);


	
		
};

#endif // _FIND_TAG_NUMBER_H_