#ifndef _ARMCFG_H_
#define _ARMCFG_H_

class ArmConfig {
	public:
		// Arm dimension
		float baseHeight;
		float baseRadius;

		float foreArmLength;
		float armLength;

		float handLength;		
		float cameraLength;

		float maxOpen;

		float dGripLength;

		float gripWidth;
		float gripDepth;

		float ceta1;
		float ceta2;
		float ceta3;
		float ceta4;
		float ceta5;

		ArmConfig() {
			baseHeight		= 6.9f;
			baseRadius		= 4.52f;


			foreArmLength	= 14.7f;	

			armLength		= 18.5f;
			//handLength		= 6.3f;
			handLength		= 9.5f;
			dGripLength		= 0.8f; 

			cameraLength	= 3.0f;

			maxOpen			= 0.0f;

			gripWidth		= 6.45f;
			gripDepth		= 3.0f;

			ceta1 = 0.0f;
			ceta2 = 80.0f;
			ceta3 = 60.0f;
			ceta4 = 50.0;
			ceta5 = 0.0f;
		}

		~ArmConfig(){}


};


#endif // _ARMCFG_H_
