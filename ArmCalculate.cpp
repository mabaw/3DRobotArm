#include <stdio.h>
#include "ArmCalculate.h"
#include "math.h"
#include <iostream>

using namespace std;

ArmCalculate::ArmCalculate()
{	
}
ArmCalculate::~ArmCalculate()
{
}

bool ArmCalculate::Initialize(){
	return true;
}

void ArmCalculate::CalculateInverseKinematic(float x,float y,float z,float l1,float l2,float *ceta0,float *ceta1,float *ceta2)
{
	*ceta0 = atan2(y,x);
	float x2 = y / sinf(*ceta0);
	float y2 = z;
	
	float phi = atanf(y2/x2);
	cout << x2 <<endl;
	cout << y2 <<endl;
	cout << phi / M_PI * 180 <<endl;
	float c2 = (x2*x2 + y2*y2 - l1*l1 - l2*l2) / (2*l1*l2);
	float s2 = sqrt(1-c2*c2);
	*ceta2 = (atan2(s2,c2) / M_PI * 180);	
	float k1 = l1 + l2*c2;
	float k2 = l2*s2;
	float r = sqrt(k1*k1+k2*k2);
	*ceta1 =  (atan2(y2,x2) - atan2(k2,k1) / M_PI * 180);
	*ceta0  = *ceta0 / M_PI * 180; 
}

