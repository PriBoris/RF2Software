
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
		(MachineSettings::protocolStructExtendedValid==true)
	){

		ProtocolStruct newProtocolStruct;
		memcpy(&newProtocolStruct,message->value,sizeof(ProtocolStruct));

		if (checkProtocolStruct(&newProtocolStruct)==true){

			memcpy(&protocolStruct,&newProtocolStruct,sizeof(ProtocolStruct));
			protocolStructValid = true;

			DebugConsole::pushMessage(" #PersonalSettings load OK");

		}else{
			DebugConsole::pushMessage(" #PersonalSettings load FAIL");

		}


	}


		
		

		


	
}
//=================================================================================================
bool PersonalSettings::checkProtocolStruct(ProtocolStruct *protocolStruct){

	//check position A
	if (protocolStruct->positionMainA > MachineSettings::protocolStructExtended.positionMainMax){
		protocolStruct->positionMainA = MachineSettings::protocolStructExtended.positionMainMax;
	}
	if (protocolStruct->positionMainA < MachineSettings::protocolStructExtended.positionMainMin){
		protocolStruct->positionMainA = MachineSettings::protocolStructExtended.positionMainMin;
	}

	//check position B
	if (protocolStruct->positionMainB > MachineSettings::protocolStructExtended.positionMainMax){
		protocolStruct->positionMainB = MachineSettings::protocolStructExtended.positionMainMax;
	}
	if (protocolStruct->positionMainB < MachineSettings::protocolStructExtended.positionMainMin){
		protocolStruct->positionMainB = MachineSettings::protocolStructExtended.positionMainMin;
	}

	// check parking position
	protocolStruct->positionMainParking = (MachineSettings::protocolStructExtended.positionMainMax + MachineSettings::protocolStructExtended.positionMainMin) / 2;
	if (protocolStruct->positionMainParking > MachineSettings::protocolStructExtended.positionMainMax){
		protocolStruct->positionMainParking = MachineSettings::protocolStructExtended.positionMainMax;
	}
	if (protocolStruct->positionMainParking < MachineSettings::protocolStructExtended.positionMainMin){
		protocolStruct->positionMainParking = MachineSettings::protocolStructExtended.positionMainMin;
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
	if (protocolStruct->speedAbsMainParking < MachineSettings::protocolStructExtended.speedAbsMainMax){
		protocolStruct->speedAbsMainParking = MachineSettings::protocolStructExtended.speedAbsMainMax;
	}

	// check aux1 position
	if (protocolStruct->positionAux1 > MachineSettings::protocolStructExtended.positionAux1Max){
		protocolStruct->positionAux1 = MachineSettings::protocolStructExtended.positionAux1Max;
	}
	if (protocolStruct->positionAux1 < MachineSettings::protocolStructExtended.positionAux1Min){
		protocolStruct->positionAux1 = MachineSettings::protocolStructExtended.positionAux1Min;
	}

	// check aux2 position
	if (protocolStruct->positionAux2 > MachineSettings::protocolStructExtended.positionAux2Max){
		protocolStruct->positionAux2 = MachineSettings::protocolStructExtended.positionAux2Max;
	}
	if (protocolStruct->positionAux2 < MachineSettings::protocolStructExtended.positionAux2Min){
		protocolStruct->positionAux2 = MachineSettings::protocolStructExtended.positionAux2Min;
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
//=================================================================================================
int32_t PersonalSettings::getMainRangeDegrees(){

	return MachineSettings::moveDistanceToDegrees(getMainRange());

}
//=================================================================================================


