
#include "MainTick.h"

#include <string.h>

#include "hmi/diagnostics.h"
#include "hmi/hmi.h"
#include "nfc/nfc.h"
#include "servo/encoder.h"
#include "servo/servo.h"


bool MainTick::reportServoModeIssued = false;
int32_t MainTick::actualServoCommand = REPORT_SERVO_MODE_STOP;


//==================================================================================================================
void MainTick::reportServoModeDefault(){
	
	if (reportServoModeIssued==false){

		uint8_t message[MSGLEN_ReportServoMode];
		memset(message,0,sizeof(message));
		reportServoModeHeader(message);

		{
			uint32_t cmd = REPORT_SERVO_MODE_STOP;
			memcpy(&message[MSGPOS_ReportServoMode_servoCommand],&cmd,sizeof(cmd));
		}

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

	memcpy(&message[14],&servoFrequencyPositive,sizeof(servoFrequencyPositive));
	memcpy(&message[18],&servoFrequencyNegative,sizeof(servoFrequencyNegative));

	{
		int32_t encoderValue = Encoder::getValue();
		memcpy(
			&message[22],
			&encoderValue,
			sizeof(int32_t)
			);
	}

	memcpy(&message[MSGPOS_ReportServoMode_heatsinkTemperature],&Servo::heatsinkTemperature,sizeof(Servo::heatsinkTemperature));
	memcpy(&message[MSGPOS_ReportServoMode_internalTemperature],&Servo::internalTemperature,sizeof(Servo::internalTemperature));
	memcpy(&message[MSGPOS_ReportServoMode_motorTemperature],&Servo::motorTemperature,sizeof(Servo::motorTemperature));

	{
		uint32_t bytesCountToHMI = HMI::protocol.getTxBytesCount();
		uint32_t bytesCountFromHMI = HMI::protocol.getRxBytesCount();
		uint32_t bytesCountToNFC = NFC::protocol.getTxBytesCount();
		uint32_t bytesCountFromNFC = NFC::protocol.getRxBytesCount();

		memcpy(&message[MSGPOS_ReportServoMode_bytesCountToHMI], &bytesCountToHMI, sizeof(uint32_t));
		memcpy(&message[MSGPOS_ReportServoMode_bytesCountFromHMI], &bytesCountFromHMI, sizeof(uint32_t));
		memcpy(&message[MSGPOS_ReportServoMode_bytesCountToNFC], &bytesCountToNFC, sizeof(uint32_t));
		memcpy(&message[MSGPOS_ReportServoMode_bytesCountFromNFC], &bytesCountFromNFC, sizeof(uint32_t));


	}

}
//==================================================================================================================
void MainTick::reportServoMode(int32_t servoCommand){

	uint8_t message[MSGLEN_ReportServoMode];
	memset(message,0,sizeof(message));
	reportServoModeHeader(message);

	memcpy(&message[MSGPOS_ReportServoMode_servoCommand],&servoCommand,sizeof(servoCommand));
	actualServoCommand = servoCommand;

	
	Diagnostics::protocol.sendPacket(Protocol::TAG_ReportServoMode,message,sizeof(message));
	
	reportServoModeIssued = true;

}
void MainTick::reportServoModePositive(){
	reportServoMode(REPORT_SERVO_MODE_POSITIVE);
}
void MainTick::reportServoModeNegative(){
	reportServoMode(REPORT_SERVO_MODE_NEGATIVE);
}
void MainTick::reportServoModeStop(){
	reportServoMode(REPORT_SERVO_MODE_STOP);
}
void MainTick::reportServoModeContinue(){
	reportServoMode(actualServoCommand);
}
//==================================================================================================================



