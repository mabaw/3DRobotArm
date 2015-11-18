#ifndef _ARMCAL_H_
#define _ARMCAL_H_


#include "yMath.h"

#define M_PI 3.14159265358979323846

class ArmCalculate {
	public:		
		ArmCalculate();
		~ArmCalculate();

		bool Initialize();
		void ArmCalculate::CalculateInverseKinematic(float x,float y,float z,float l1,float l2,float *ceta0,float *ceta1,float *ceta2);

};

#endif // _ARMCAL_H_