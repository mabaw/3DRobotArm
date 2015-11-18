#ifndef _CHESSBOARD_PLATE_H_
#define _CHESSBOARD_PLATE_H_



#include "yMath\yMath.h"
#include "cv.h"
#include "cxmisc.h"
#include "highgui.h"

class ChessboardPlate {
	public:		
		

		ChessboardPlate();
		~ChessboardPlate();
		bool Initialize();


		void FindChessboard(IplImage *img);
		
};

#endif // _CHESSBOARD_PLATE_H_