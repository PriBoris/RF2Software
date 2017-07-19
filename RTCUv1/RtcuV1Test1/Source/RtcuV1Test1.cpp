

#include "RtcuV1Test1.h"


#include "hmi/hmi.h"
#include "hmi/diagnostics.h"
#include "nfc/nfc.h"
#include "system/heartbeat.h"
#include "system/ports.h"
#include "servo/servo.h"




bool modeProcessFlag = false;


int main()
{

	
	
	
 	Heartbeat::init();
	Ports::init();
	
	OdometerRTC::init();
	
	
	Crc32::init();
	HMI::init();
	Diagnostics::init();


	Servo::init();
	strainGaugeInit();
	
	nfcInit();
	
	modeInit();
	
	while(true){

		HMI::process();
		Diagnostics::process();
		
		Servo::process();
		//rtcProcess();
		nfcTxProcess();
	
		if (Heartbeat::mainTickReady==true){

			Heartbeat::mainTickReady = false;
			modeProcess();

			OdometerRTC::readCurrentDateTime(true);

		}
		
	}
	

	
}








	

