

#include "RtcuV1Test1.h"


#include "hmi/hmi.h"
#include "hmi/diagnostics.h"
#include "nfc/nfc.h"
#include "system/heartbeat.h"
#include "system/ports.h"




bool modeProcessFlag = false;


int main()
{

	
	
	
 	Heartbeat::init();
	Ports::init();
	
	OdometerRTC::init();
	
	
	Crc32::init();
	HMI::init();
	Diagnostics::init();


	servoInit();
	strainGaugeInit();
	
	nfcInit();
	
	modeInit();
	
	while(true){

		HMI::process();
		Diagnostics::process();
		
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








	

