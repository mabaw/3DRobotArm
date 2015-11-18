#include <stdio.h>
#include "ArmConnector.h"
#include "yUtils/yFile.h"
#define WHEELBASE	0.5f

ArmConnector::ArmConnector() : isRobotActive( false ), isOpen( false ), countNotActive( 0 )
{
	errorCode	= 0;
		
}
ArmConnector::~ArmConnector()
{
}

bool ArmConnector::Initialize( TCHAR* _port )
{
	port = _port;
	if ( !serial.IsOpen() )
	{
		if ( !serial.Open( port,115200,8,NOPARITY , ONESTOPBIT , GENERIC_READ|GENERIC_WRITE) )
		{
			char str[20];
			ConvertTCHARToCHAR( _port, str , 20);
			printf("Cann't open Arm's Port %s\n",str);
			isOpen = false;
			countNotActive = MAX_NOT_ACTIVE_FRAME;
			return false;
		}
		printf("Arm Connector Start!!\n");
		
		serial.ChangeBaudRate(115200);
	}


	isRobotActive = ReadVersion();
	

	// flush buffer
	char c;
	while( serial.ReadByte( &c, 1 ) ) ;
	cout << c  << endl;	
	
	countNotActive = MAX_NOT_ACTIVE_FRAME;
	isOpen = true;
	return true;
}

void ArmConnector::Finalize()
{
	if(serial.IsOpen())
		serial.Close();
}

int ArmConnector::AngleToPulse(float angle){
	return ((float)(angle+90)/180.0f*1500) + 750;
}
bool ArmConnector::CheckLimit(int p0,int p1,int p2,int p3,int p4){
	if(p0 < MIN_PULSE0 || p0 > MAX_PULSE0)
		return false;
	if(p1 < MIN_PULSE1 || p1 > MAX_PULSE1)
		return false;
	if(p2 < MIN_PULSE2 || p2 > MAX_PULSE2)
		return false;
	if(p3 < MIN_PULSE3 || p3 > MAX_PULSE3)
		return false;
	if(p4 < MIN_PULSE4 || p4 > MAX_PULSE4)
		return false;
	return true;
}

bool ArmConnector::ReadVersion()
{
	char buffer[256];
	int msgLen;
	char c;
	msgLen = sprintf(buffer,"VER%c",13);	
	cout << buffer << endl;
	serial.Write(buffer,msgLen);	

		while( serial.ReadByte( &c, 20 ) > 0 &&  msgLen < 256) {
			if(c!=13)
				cout << c ;
			else {
				cout << endl;
				cout << "Arm Active" << endl;
				return true;
			}
		}
	cout << "Arm not active" << endl;
	return false;
}

bool ArmConnector::IsArmMoving()
{
	char buffer[256];
	int msgLen;
	char c;
	msgLen = sprintf(buffer,"Q%c",13);	
	cout << buffer << endl;
	serial.Write(buffer,msgLen);	

		if( serial.ReadByte( &c, 20 ) > 0) {
			if(c=='+'){
				cout << "Arm is moving" <<endl;
				return true;
			}
			else {
				cout << "Arm is not moving" <<endl;
				return false;
			}
		}
		else {
			cout << "ERROR : cannot read Arm moving status" << endl;
			return true;
		}
}

bool ArmConnector::WriteCommand(float p0,float p1,float p2,float p3,float p4,int speed){
	char buffer[256];
	int msgLen,out0,out1,out2,out3,out4;
	
	p0 = -p0;
	p1 -= 93.0f;
	p1 = -p1;
	p2 -= 95.0f;
	p3 = -p3;
	p3 -= 10;
	p4 += 5;

	cout << p0 << " " <<  p1 << " " <<  p2 << " " << p3 << endl;

	out0 = AngleToPulse(p0);
	out1 = AngleToPulse(p1);
	out2 = AngleToPulse(p2);
	out3 = AngleToPulse(p3);
	out4 = AngleToPulse(p4);
	if(!CheckLimit(out0,out1,out2,out3,out4)){
		cout << "ERROR : Pulse out of limit." << endl;
		return false;
	}		
	//# <ch> P <pw> S <spd>... # <ch> P <pw> S <spd> T <time><cr>
	//"#5 P1600 <cr>"
	msgLen = sprintf(buffer,"#0 P%d S%d #1 P%d S%d #2 P%d S%d #3 P%d S%d #4 P%d S%d %c",out0,speed,out1,speed,out2,speed,out3,speed,out4,speed,13);	
	//msgLen = sprintf(buffer,"#2 P%d%c",1400,13);	
	cout << buffer << endl;
	serial.Write(buffer,msgLen);	
	return true;
}

