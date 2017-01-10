
#include "MainTick.h"

#include <string.h>

#include "hmi/diagnostics.h"


bool MainTick::reportServoModeIssued = false;


//==================================================================================================================
void MainTick::reportServoModeDefault(){
	
	if (reportServoModeIssued==false){



		uint8_t message[MSGLEN_ReportServoMode_DEFAULT];
		memset(message,0,sizeof(message));
		reportServoModeHeader(message);


		Diagnostics::protocol.sendPacket(Protocol::TAG_ReportServoMode,message,sizeof(message));



	}else{
		reportServoModeIssued = false;
	}

	
	
}
//==================================================================================================================
void MainTick::reportServoModeHeader(uint8_t *message){

	NV::RealTimeClock::TCurrentDateTime currentDateTime;
	NV::RealTimeClock::getCurrentDateTime(currentDateTime);


	memcpy(&message[0],&tickID,sizeof(tickID));
	memcpy(&message[4],&currentDateTime,sizeof(currentDateTime));

	message[10] = (uint8_t)modePrev;
	message[11] = (uint8_t)submodePrev;
	message[12] = (uint8_t)mode;
	message[13] = (uint8_t)submode;

}
//==================================================================================================================
	
