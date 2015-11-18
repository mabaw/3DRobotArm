#ifndef _ARMCONNECTOR_H_
#define _ARMCONNECTOR_H_

#include <tchar.h>
#include "yUtils/SerialPort.h"
#include "yMath/yMath.h"

#define MAX_CMD_LENGHT 100
#define MAX_NOT_ACTIVE_FRAME 10

#define MIN_THETA0 -90
#define MAX_THETA0 90
#define MIN_THETA1 -90
#define MAX_THETA1 90
#define MIN_THETA2 -90
#define MAX_THETA2 90
#define MIN_THETA3 -90
#define MAX_THETA3 90
#define MIN_THETA4 -90
#define MAX_THETA4 90

#define MIN_PULSE0 750
#define MAX_PULSE0 2250
#define MIN_PULSE1 1000 //800
#define MAX_PULSE1 2250
#define MIN_PULSE2 800
#define MAX_PULSE2 2250
#define MIN_PULSE3 600
#define MAX_PULSE3 2400
#define MIN_PULSE4 800
#define MAX_PULSE4 2250

class ArmConnector
{
public:
	enum
	{
		WRITE_MSG_ERR = 1, READ_MSG_ERR = 2, PARSE_COMPASS_ERR = 3, PARSE_INCLINE_ERR = 4
		,PARSE_DRIVE_ERR = 5
	};

	int errorCode;
	TCHAR* port;
	bool isOpen;
	bool isRobotActive;
	ArmConnector( );
	~ArmConnector();

	bool Initialize(TCHAR* _port);
	void Finalize();
	bool GrabData();
	bool WriteCommand(float p0,float p1,float p2,float p3,float p4,int speed);
	bool ReadVersion();
	bool IsArmMoving();
	
private:
	SerialPort serial;

	int		countNotActive;
	int AngleToPulse(float angle);
	bool CheckLimit(int p0,int p1,int p2,int p3,int p4);

	unsigned char ctox(unsigned char c);
	unsigned char is_checksum_error(char* str,unsigned char chk_sum);
	unsigned char gen_checksum(char* str);


//	bool WriteMsg( char* msg );
//	bool AddCheckSumAndCRLF( char* str );
};

#endif // _ARMCONNECTOR_H_