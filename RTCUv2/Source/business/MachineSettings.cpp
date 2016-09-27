
#include "MachineSettings.h"

#include <string.h>

#include "business/Settings.h"
#include "hmi/hmi.h"
#include "hmi/diagnostics.h"
#include "hmi/debugConsole.h"
#include "servo/servo.h"
#include "servo/encoder.h"


MachineSettings::ProtocolStruct MachineSettings::protocolStruct;
bool MachineSettings::protocolStructValid;


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
		(settings->speedAbsMainPersonal==Settings::INT32_UNKNOWN_VALUE)
	){
		protocolStructValid = false;
		memset(&protocolStruct,0,sizeof(protocolStruct));
		DebugConsole::pushMessage(" #MachineSettings INVALID");


	}else{

		memset(&protocolStruct,0,sizeof(protocolStruct));

		protocolStruct.positionMainMax = settings->positionMainMax;
		protocolStruct.positionMainMin = settings->positionMainMin;
		protocolStruct.positionAux1Max = settings->positionAux1Max;
		protocolStruct.positionAux1Min = settings->positionAux1Min;
		protocolStruct.positionAux2Max = settings->positionAux2Max;
		protocolStruct.positionAux2Min = settings->positionAux2Min;
		protocolStruct.positionAux3Max = settings->positionAux3Max;
		protocolStruct.positionAux3Min = settings->positionAux3Min;
		protocolStruct.speedAbsMainMax = settings->speedAbsMainMax;
		protocolStruct.speedAbsMainPersonal = settings->speedAbsMainPersonal;

		protocolStructValid = true;
		DebugConsole::pushMessage(" #MachineSettings VALID");
	}

}
//=================================================================================================
void MachineSettings::report(){

	if (protocolStructValid==false){

		Diagnostics::protocol.sendPacket(Protocol::TAG_ReportMachineSettings,0,0);
		HMI::protocol.sendPacket(Protocol::TAG_ReportMachineSettings,0,0);

	}else{

		Diagnostics::protocol.sendPacket(Protocol::TAG_ReportMachineSettings,(uint8_t*)&protocolStruct,sizeof(ProtocolStruct));
		HMI::protocol.sendPacket(Protocol::TAG_ReportMachineSettings,(uint8_t*)&protocolStruct,sizeof(ProtocolStruct));

	}


}
//=================================================================================================
void MachineSettings::load(RxMessage *message){


	if (
		(message->tag==Protocol::TAG_LoadMachineSettings)&&
		(message->valueLen==sizeof(ProtocolStruct))&&
		(message->fromHMI==false)

	){

		ProtocolStruct newProtocolStruct;
		memcpy(&newProtocolStruct,message->value,sizeof(ProtocolStruct));

		if (checkProtocolStruct(&newProtocolStruct)==true){

			Settings::Struct *settings = Settings::getStruct();

			settings->positionMainMax = newProtocolStruct.positionMainMax;
			settings->positionMainMin = newProtocolStruct.positionMainMin;
			settings->positionAux1Max = newProtocolStruct.positionAux1Max;
			settings->positionAux1Min = newProtocolStruct.positionAux1Min;
			settings->positionAux2Max = newProtocolStruct.positionAux2Max;
			settings->positionAux2Min = newProtocolStruct.positionAux2Min;
			settings->positionAux3Max = newProtocolStruct.positionAux3Max;
			settings->positionAux3Min = newProtocolStruct.positionAux3Min;
			settings->speedAbsMainMax = newProtocolStruct.speedAbsMainMax;
			settings->speedAbsMainPersonal = newProtocolStruct.speedAbsMainPersonal;

			Settings::updateStruct();

			memcpy(&protocolStruct,&newProtocolStruct,sizeof(ProtocolStruct));
			protocolStructValid = true;

			DebugConsole::pushMessage("MachineSettings were loaded \0");

		}




	}	




}
//=================================================================================================
bool MachineSettings::checkProtocolStruct(ProtocolStruct *protocolStruct){

	bool valid = true;

	if (
		(protocolStruct->positionMainMax<0)||
		(protocolStruct->positionMainMax>4095)||
		(protocolStruct->positionMainMin<0)||
		(protocolStruct->positionMainMax<protocolStruct->positionMainMin)


		//...

	){
		valid = false;
	}





	return valid;
}
//=================================================================================================
int32_t MachineSettings::getMainRange(){

	int32_t mainRange = MachineSettings::protocolStruct.positionMainMax - MachineSettings::protocolStruct.positionMainMin;

	if (protocolStructValid!=true){
		return 0;
	}else if (mainRange<0){
		return (-mainRange);
	}else{
		return mainRange;
	}

}
//=================================================================================================





