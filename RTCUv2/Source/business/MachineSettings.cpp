
#include "MachineSettings.h"

#include <string.h>
#include <stdlib.h>

#include "business/Settings.h"
#include "hmi/hmi.h"
#include "hmi/diagnostics.h"
#include "hmi/debugConsole.h"
#include "servo/servo.h"
#include "servo/encoder.h"
#include "business/FrequencyModulation.h"


MachineSettings::ProtocolStructExtended MachineSettings::protocolStructExtended;
bool MachineSettings::protocolStructExtendedValid;


//=================================================================================================
void MachineSettings::init(){

	Settings::Struct *settings = Settings::getStruct();

	if (
		(settings->positionMainMax==Settings::INT32_UNKNOWN_VALUE)||
		(settings->positionMainMin==Settings::INT32_UNKNOWN_VALUE)||
		(settings->positionAux1Max==Settings::INT32_UNKNOWN_VALUE)||
		(settings->positionAux1Min==Settings::INT32_UNKNOWN_VALUE)||
		(settings->positionAux2Max==Settings::INT32_UNKNOWN_VALUE)||
		(settings->positionAux2Min==Settings::INT32_UNKNOWN_VALUE)||
		(settings->positionAux3Max==Settings::INT32_UNKNOWN_VALUE)||
		(settings->positionAux3Min==Settings::INT32_UNKNOWN_VALUE)||
		(settings->speedAbsMainMax==Settings::INT32_UNKNOWN_VALUE)||
		(settings->speedAbsMainPersonal==Settings::INT32_UNKNOWN_VALUE)||
		(settings->encoderBitCount==Settings::INT32_UNKNOWN_VALUE)||
		(settings->encoderDirection==Settings::INT32_UNKNOWN_VALUE)||
		(settings->encoderOffset==Settings::INT32_UNKNOWN_VALUE)||
		(settings->forceSensorGain==Settings::FLOAT_UNKNOWN_VALUE)||
		(settings->forceSensorOffset0==Settings::FLOAT_UNKNOWN_VALUE)||
		(settings->forceSensorOffset1==Settings::FLOAT_UNKNOWN_VALUE)||
		(settings->forceSensorOffset2==Settings::FLOAT_UNKNOWN_VALUE)||
		(settings->minPositiveServoFrequency==Settings::FLOAT_UNKNOWN_VALUE)||
		(settings->maxPositiveServoFrequency==Settings::FLOAT_UNKNOWN_VALUE)||
		(settings->minNegativeServoFrequency==Settings::FLOAT_UNKNOWN_VALUE)||
		(settings->maxNegativeServoFrequency==Settings::FLOAT_UNKNOWN_VALUE)||
		(settings->concentricAccelerationLaw==Settings::INT32_UNKNOWN_VALUE)||
		(settings->concentricDecelerationLaw==Settings::INT32_UNKNOWN_VALUE)||
		(settings->eccentricAccelerationLaw==Settings::INT32_UNKNOWN_VALUE)||
		(settings->eccentricDecelerationLaw==Settings::INT32_UNKNOWN_VALUE)||

		false

	){
		protocolStructExtendedValid = false;
		memset(&protocolStructExtended,0,sizeof(protocolStructExtended));
		
		DebugConsole::pushMessage(" #MachineSettings INVALID");

		loadDefaultValues();

	}else{

		memset(&protocolStructExtended,0,sizeof(protocolStructExtended));

		protocolStructExtended.positionMainMax = settings->positionMainMax;
		protocolStructExtended.positionMainMin = settings->positionMainMin;
		protocolStructExtended.positionAux1Max = settings->positionAux1Max;
		protocolStructExtended.positionAux1Min = settings->positionAux1Min;
		protocolStructExtended.positionAux2Max = settings->positionAux2Max;
		protocolStructExtended.positionAux2Min = settings->positionAux2Min;
		protocolStructExtended.positionAux3Max = settings->positionAux3Max;
		protocolStructExtended.positionAux3Min = settings->positionAux3Min;
		protocolStructExtended.speedAbsMainMax = settings->speedAbsMainMax;
		protocolStructExtended.speedAbsMainPersonal = settings->speedAbsMainPersonal;

		protocolStructExtended.encoderBitCount = settings->encoderBitCount;
		protocolStructExtended.encoderDirection = settings->encoderDirection;
		protocolStructExtended.encoderOffset = settings->encoderOffset;
		protocolStructExtended.forceSensorGain = settings->forceSensorGain;
		protocolStructExtended.forceSensorOffset0 = settings->forceSensorOffset0;
		protocolStructExtended.forceSensorOffset1 = settings->forceSensorOffset1;
		protocolStructExtended.forceSensorOffset2 = settings->forceSensorOffset2;

		protocolStructExtended.minPositiveServoFrequency = settings->minPositiveServoFrequency;
		protocolStructExtended.maxPositiveServoFrequency = settings->maxPositiveServoFrequency;
		protocolStructExtended.minNegativeServoFrequency = settings->minNegativeServoFrequency;
		protocolStructExtended.maxNegativeServoFrequency = settings->maxNegativeServoFrequency;

		protocolStructExtended.concentricAccelerationLaw = settings->concentricAccelerationLaw;
		protocolStructExtended.concentricDecelerationLaw = settings->concentricDecelerationLaw;
		protocolStructExtended.eccentricAccelerationLaw = settings->eccentricAccelerationLaw;
		protocolStructExtended.eccentricDecelerationLaw = settings->eccentricDecelerationLaw;

		protocolStructExtendedValid = true;

		DebugConsole::pushMessage(" #MachineSettings VALID");


	}

}
//=================================================================================================
void MachineSettings::report(){

	if (protocolStructExtendedValid==false){

		Diagnostics::protocol.sendPacket(Protocol::TAG_ReportMachineSettings,0,0);
		Diagnostics::protocol.sendPacket(Protocol::TAG_ReportMachineSettingsExtended,0,0);
		HMI::protocol.sendPacket(Protocol::TAG_ReportMachineSettings,0,0);

	}else{

		Diagnostics::protocol.sendPacket(
			Protocol::TAG_ReportMachineSettings,
			(uint8_t*)&protocolStructExtended,
			ProtocolStructSize
			);
		Diagnostics::protocol.sendPacket(
			Protocol::TAG_ReportMachineSettingsExtended,
			(uint8_t*)&protocolStructExtended,
			sizeof(ProtocolStructExtended)
			);
		HMI::protocol.sendPacket(
			Protocol::TAG_ReportMachineSettings,
			(uint8_t*)&protocolStructExtended,
			ProtocolStructSize
			);
	}


}
//=================================================================================================
void MachineSettings::load(RxMessage *message){


	if (
		(message->tag==Protocol::TAG_LoadMachineSettingsExtended)&&
		(message->valueLen==sizeof(protocolStructExtended))&&
		(message->fromHMI==false)

	){

		ProtocolStructExtended newProtocolStructExtended;
		memcpy(&newProtocolStructExtended,message->value,sizeof(ProtocolStructExtended));

		if (checkProtocolStructExtended(&newProtocolStructExtended)==true){

			Settings::Struct *settings = Settings::getStruct();

			settings->positionMainMax = newProtocolStructExtended.positionMainMax;
			settings->positionMainMin = newProtocolStructExtended.positionMainMin;
			settings->positionAux1Max = newProtocolStructExtended.positionAux1Max;
			settings->positionAux1Min = newProtocolStructExtended.positionAux1Min;
			settings->positionAux2Max = newProtocolStructExtended.positionAux2Max;
			settings->positionAux2Min = newProtocolStructExtended.positionAux2Min;
			settings->positionAux3Max = newProtocolStructExtended.positionAux3Max;
			settings->positionAux3Min = newProtocolStructExtended.positionAux3Min;
			settings->speedAbsMainMax = newProtocolStructExtended.speedAbsMainMax;
			settings->speedAbsMainPersonal = newProtocolStructExtended.speedAbsMainPersonal;

			settings->encoderBitCount = newProtocolStructExtended.encoderBitCount;
			settings->encoderDirection = newProtocolStructExtended.encoderDirection;
			settings->encoderOffset = newProtocolStructExtended.encoderOffset;
			settings->forceSensorGain = newProtocolStructExtended.forceSensorGain;
			settings->forceSensorOffset0 = newProtocolStructExtended.forceSensorOffset0;
			settings->forceSensorOffset1 = newProtocolStructExtended.forceSensorOffset1;
			settings->forceSensorOffset2 = newProtocolStructExtended.forceSensorOffset2;

			settings->minPositiveServoFrequency = newProtocolStructExtended.minPositiveServoFrequency;
			settings->maxPositiveServoFrequency = newProtocolStructExtended.maxPositiveServoFrequency;
			settings->minNegativeServoFrequency = newProtocolStructExtended.minNegativeServoFrequency;
			settings->maxNegativeServoFrequency = newProtocolStructExtended.maxNegativeServoFrequency;

			settings->concentricAccelerationLaw = newProtocolStructExtended.concentricAccelerationLaw;
			settings->concentricDecelerationLaw = newProtocolStructExtended.concentricDecelerationLaw;
			settings->eccentricAccelerationLaw = newProtocolStructExtended.eccentricAccelerationLaw;
			settings->eccentricDecelerationLaw = newProtocolStructExtended.eccentricDecelerationLaw;

			Settings::updateStruct();

			memcpy(&protocolStructExtended,&newProtocolStructExtended,sizeof(ProtocolStructExtended));
			protocolStructExtendedValid = true;

			//DebugConsole::pushMessage("MachineSettings were loaded \0");

		}




	}	




}
//=================================================================================================
bool MachineSettings::checkProtocolStructExtended(ProtocolStructExtended *protocolStructExtended){

	bool valid = true;

	if (
		(protocolStructExtended->positionMainMax<0)||
		(protocolStructExtended->positionMainMin<0)||
		(protocolStructExtended->positionMainMax<protocolStructExtended->positionMainMin)||//TODO: should be dependent on encoder bits count

		(protocolStructExtended->concentricAccelerationLaw < FrequencyModulation::MIN_LAW)||
		(protocolStructExtended->concentricDecelerationLaw < FrequencyModulation::MIN_LAW)||
		(protocolStructExtended->eccentricAccelerationLaw < FrequencyModulation::MIN_LAW)||
		(protocolStructExtended->eccentricDecelerationLaw < FrequencyModulation::MIN_LAW)||
		(protocolStructExtended->concentricAccelerationLaw > FrequencyModulation::MAX_LAW)||
		(protocolStructExtended->concentricDecelerationLaw > FrequencyModulation::MAX_LAW)||
		(protocolStructExtended->eccentricAccelerationLaw > FrequencyModulation::MAX_LAW)||
		(protocolStructExtended->eccentricDecelerationLaw > FrequencyModulation::MAX_LAW)||

		(protocolStructExtended->minPositiveServoFrequency < 2.0f)||
		(protocolStructExtended->maxPositiveServoFrequency < 2.0f)||
		(protocolStructExtended->minPositiveServoFrequency > 60.0f)||
		(protocolStructExtended->maxPositiveServoFrequency > 60.0f)||
		(protocolStructExtended->minPositiveServoFrequency > protocolStructExtended->maxPositiveServoFrequency)||

		(protocolStructExtended->minNegativeServoFrequency > -2.0f)||
		(protocolStructExtended->maxNegativeServoFrequency > -2.0f)||
		(protocolStructExtended->minNegativeServoFrequency < -60.0f)||
		(protocolStructExtended->maxNegativeServoFrequency < -60.0f)||
		(protocolStructExtended->minNegativeServoFrequency < protocolStructExtended->maxNegativeServoFrequency)||


		//...

		false
	){
		valid = false;
	}





	return valid;
}
//=================================================================================================
int32_t MachineSettings::getMainRange(){

	int32_t mainRange = MachineSettings::protocolStructExtended.positionMainMax - MachineSettings::protocolStructExtended.positionMainMin;

	if (protocolStructExtendedValid!=true){
		return 0;
	}else if (mainRange<0){
		return (-mainRange);
	}else{
		return mainRange;
	}

}
//=================================================================================================
int32_t MachineSettings::moveDistanceToDegrees(int32_t moveDistance){

	const int32_t fullCircleDegrees = 360;
	int32_t fullCircle = 4096;
	switch(protocolStructExtended.encoderBitCount){
	case 15:
		fullCircle = 32767;
		break;
	}
	return abs(moveDistance)*fullCircleDegrees/fullCircle;

}
//=================================================================================================	static void loadDefaultValues (void);
void MachineSettings::loadDefaultValues(void){

	memset(&protocolStructExtended,0,sizeof(protocolStructExtended));

	protocolStructExtended.positionMainMax = 17200;
	protocolStructExtended.positionMainMin = 2650;
	protocolStructExtended.positionAux1Max = 800;
	protocolStructExtended.positionAux1Min = 100;
	protocolStructExtended.positionAux2Max = 3950;
	protocolStructExtended.positionAux2Min = 1300;
	protocolStructExtended.positionAux3Max = 2050;
	protocolStructExtended.positionAux3Min = 500;
	protocolStructExtended.speedAbsMainMax = 5000;
	protocolStructExtended.speedAbsMainPersonal = 15000;

	protocolStructExtended.encoderBitCount = 15;
	protocolStructExtended.encoderDirection = 1;
	protocolStructExtended.encoderOffset = -10000;
	protocolStructExtended.forceSensorGain = 0.000651042;
	protocolStructExtended.forceSensorOffset0 = 8000;
	protocolStructExtended.forceSensorOffset1 = 0;
	protocolStructExtended.forceSensorOffset2 = 0;

	protocolStructExtended.minPositiveServoFrequency = 5;
	protocolStructExtended.maxPositiveServoFrequency = 60;
	protocolStructExtended.minNegativeServoFrequency = -5;
	protocolStructExtended.maxNegativeServoFrequency = -60;

	protocolStructExtended.concentricAccelerationLaw = 1;
	protocolStructExtended.concentricDecelerationLaw = 4;
	protocolStructExtended.eccentricAccelerationLaw = 1;
	protocolStructExtended.eccentricDecelerationLaw = 4;

	protocolStructExtendedValid = true;

	DebugConsole::pushMessage(" #MachineSettings DEFAULTS loaded");

}
//=================================================================================================	static void loadDefaultValues (void);





