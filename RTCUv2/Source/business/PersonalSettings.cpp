
#include "PersonalSettings.h"

#include <string.h>


#include "hmi/hmi.h"
#include "hmi/diagnostics.h"
#include "hmi/debugConsole.h"
#include "business/MachineSettings.h"



PersonalSettings::ProtocolStruct PersonalSettings::protocolStruct;
bool PersonalSettings::protocolStructValid;



//=================================================================================================
void PersonalSettings::init(){


	protocolStructValid = false;
	memset(&protocolStruct,0,sizeof(protocolStruct));


	
}
//=================================================================================================

void PersonalSettings::report(){

	if (protocolStructValid==false){

		Diagnostics::protocol.sendPacket(Protocol::TAG_ReportPersonalSettings,0,0);
		HMI::protocol.sendPacket(Protocol::TAG_ReportPersonalSettings,0,0);

	}else{

		Diagnostics::protocol.sendPacket(Protocol::TAG_ReportPersonalSettings,(uint8_t*)&protocolStruct,sizeof(ProtocolStruct));
		HMI::protocol.sendPacket(Protocol::TAG_ReportPersonalSettings,(uint8_t*)&protocolStruct,sizeof(ProtocolStruct));


	}


}
//=================================================================================================
void PersonalSettings::load(RxMessage *message){

	if (
		(message->tag==Protocol::TAG_LoadPersonalSettings)&&
		(message->valueLen==sizeof(ProtocolStruct))&&
		(MachineSettings::protocolStructValid==true)
	){

		ProtocolStruct newProtocolStruct;
		memcpy(&newProtocolStruct,message->value,sizeof(ProtocolStruct));

		if (checkProtocolStruct(&newProtocolStruct)==true){

			memcpy(&protocolStruct,&newProtocolStruct,sizeof(ProtocolStruct));
			protocolStructValid = true;

			DebugConsole::pushMessage(" #PersonalSettings were loaded");




		}


	}


		
		

		


	
}
//=================================================================================================
bool PersonalSettings::checkProtocolStruct(ProtocolStruct *protocolStruct){


	// check parking position
	if (protocolStruct->positionMainParking > MachineSettings::protocolStruct.positionMainMax){
		protocolStruct->positionMainParking = MachineSettings::protocolStruct.positionMainMax;
	}
	if (protocolStruct->positionMainParking < MachineSettings::protocolStruct.positionMainMin){
		protocolStruct->positionMainParking = MachineSettings::protocolStruct.positionMainMin;
	}

	//check position A
	if (protocolStruct->positionMainA > MachineSettings::protocolStruct.positionMainMax){
		protocolStruct->positionMainA = MachineSettings::protocolStruct.positionMainMax;
	}
	if (protocolStruct->positionMainA < MachineSettings::protocolStruct.positionMainMin){
		protocolStruct->positionMainA = MachineSettings::protocolStruct.positionMainMin;
	}

	//check position B
	if (protocolStruct->positionMainB > MachineSettings::protocolStruct.positionMainMax){
		protocolStruct->positionMainB = MachineSettings::protocolStruct.positionMainMax;
	}
	if (protocolStruct->positionMainB < MachineSettings::protocolStruct.positionMainMin){
		protocolStruct->positionMainB = MachineSettings::protocolStruct.positionMainMin;
	}

	//check AB distance
	{
		int32_t mainDistance = protocolStruct->positionMainA - protocolStruct->positionMainB;
		if (mainDistance<0){
			mainDistance = -mainDistance;
		}
		if (mainDistance<100){
			return false;
		}
	}

	//check parking speed
	if (protocolStruct->speedAbsMainParking < MachineSettings::protocolStruct.speedAbsMainMax){
		protocolStruct->speedAbsMainParking = MachineSettings::protocolStruct.speedAbsMainMax;
	}

	// check aux1 position
	if (protocolStruct->positionAux1 > MachineSettings::protocolStruct.positionAux1Max){
		protocolStruct->positionAux1 = MachineSettings::protocolStruct.positionAux1Max;
	}
	if (protocolStruct->positionAux1 < MachineSettings::protocolStruct.positionAux1Min){
		protocolStruct->positionAux1 = MachineSettings::protocolStruct.positionAux1Min;
	}

	// check aux2 position
	if (protocolStruct->positionAux2 > MachineSettings::protocolStruct.positionAux2Max){
		protocolStruct->positionAux2 = MachineSettings::protocolStruct.positionAux2Max;
	}
	if (protocolStruct->positionAux2 < MachineSettings::protocolStruct.positionAux2Min){
		protocolStruct->positionAux2 = MachineSettings::protocolStruct.positionAux2Min;
	}




//		int32_t	speedRelMainTestEccentric;
//		int32_t	speedRelMainTestConcentric;


	return true;

}
//=================================================================================================
int32_t PersonalSettings::getMainRange(){

	int32_t mainRange = PersonalSettings::protocolStruct.positionMainA - PersonalSettings::protocolStruct.positionMainB;

	if (protocolStructValid!=true){
		return 0;
	}else if (mainRange<0){
		return (-mainRange);
	}else{
		return mainRange;
	}

}

int32_t PersonalSettings::getMainRangeDegrees(){

	return getMainRange()/11;

	// 4096/360=11
}

//=================================================================================================


