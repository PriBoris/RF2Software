

#include "RtcuV1Test1.h"





bool modeProcessFlag = false;


int main()
{

	
	
	
 	SysTickInit();
	gpioInit();
	OdometerRTC::init();
	
	hmiInit();
	blackBoxInit();


	servoInit();
	strainGaugeInit();
	
	nfcInit();
	
	modeInit();
	
	while(1)
	{
		hmiProcess();
		blackBoxProcess();
		servoProcess();
		//rtcProcess();
		nfcTxProcess();
	
		if (modeProcessFlag==true){

			modeProcessFlag = false;
			modeProcess();

			OdometerRTC::readCurrentDateTime(true);

		}
		
	}
	

	
}








	

