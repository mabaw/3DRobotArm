#ifndef _TEST_FIND_CHESS_H_
#define _TEST_FIND_CHESS_H_



#include "yMath\yMath.h"
#include "cv.h"
#include "cxmisc.h"
#include "highgui.h"

class TestFindChessboard {
	public:		
		

		TestFindChessboard();
		~TestFindChessboard();
		bool Initialize();


		void FindChessboard();
		
};

#endif // _TEST_FIND_CHESS_H_