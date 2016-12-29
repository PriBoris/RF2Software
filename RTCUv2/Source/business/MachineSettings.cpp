
#include "MachineSettings.h"

#include <string.h>

#include "business/Settings.h"
#include "hmi/hmi.h"
#include "hmi/diagnostics.h"
//#include "hmi/debugConsole.h"
#include "servo/servo.h"
#include "servo/encoder.h"


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
		(settings->forceSensorDirection==Settings::INT32_UNKNOWN_VALUE)||
		(settings->forceSensorOffset==Settings::INT32_UNKNOWN_VALUE)||
		(settings->forceSensorGain==Settings::FLOAT_UNKNOWN_VALUE)||
		false

	){
		protocolStructExtendedValid = false;
		memset(&protocolStructExtended,0,sizeof(protocolStructExtended));
		//DebugConsole::pushMessage(" #MachineSettings INVALID");


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
		protocolStructExtended.forceSensorDirection = settings->forceSensorDirection;
		protocolStructExtended.forceSensorOffset = settings->forceSensorOffset;
		protocolStructExtended.forceSensorGain = settings->forceSensorGain;

		protocolStructExtendedValid = true;
		//DebugConsole::pushMessage(" #MachineSettings VALID");


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
			settings->forceSensorDirection = newProtocolStructExtended.forceSensorDirection;
			settings->forceSensorOffset = newProtocolStructExtended.forceSensorOffset;
			settings->forceSensorGain = newProtocolStructExtended.forceSensorGain;



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
		(protocolStructExtended->positionMainMax<protocolStructExtended->positionMainMin)
//TODO: should be dependent on encoder bits count

		//...

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





