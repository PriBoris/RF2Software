
#include "GenericSetSettings.h"

#include <string.h>

#include "hmi/hmi.h"
#include "hmi/diagnostics.h"

#include "business/MachineSettings.h"
#include "business/PersonalSettings.h"


GenericSetSettings::Set GenericSetSettings::set;	
GenericSetSettings::Set GenericSetSettings::newSet;
bool GenericSetSettings::valid;

//=================================================================================================
void GenericSetSettings::init(){

	memset(&set,0,sizeof(set));
	valid = false;

}
//=================================================================================================
void GenericSetSettings::report(){

	Diagnostics::protocol.sendPacket(Protocol::TAG_ReportGenericSetSettings,(uint8_t*)&set,getSetSizeBytes());
	HMI::protocol.sendPacket(Protocol::TAG_ReportGenericSetSettings,(uint8_t*)&set,getSetSizeBytes());

}
//=================================================================================================
uint32_t GenericSetSettings::getSetSizeBytes(){

	if (valid==false){
		return 0;
	}else{
		return sizeof(int32_t)*4 + set.moveCount*sizeof(Move);
	}

}
//=================================================================================================
void GenericSetSettings::load(RxMessage *message){

	if (
		(message->tag==Protocol::TAG_LoadGenericSetSettings)&&
		(checkSetSizeBytes(message->valueLen)==true)&&
		(MachineSettings::protocolStructExtendedValid==true)&&
		(PersonalSettings::protocolStructValid==true)
	){

		memset(&newSet,0,sizeof(newSet));
		memcpy(&newSet,&message->value,message->valueLen);

		//uint32_t newMoveCount =  (message->valueLen - sizeof(int32_t)*4) / sizeof(Move);

		if(checkSet(newSet)==true){
			memcpy(&set,&newSet,message->valueLen);
			valid = true;
			//DebugConsole::pushMessage(" #GenericSetSettings is loaded");
		}



	}


}
//=================================================================================================
bool GenericSetSettings::checkSetSizeBytes(uint32_t sizeBytes){

	const uint32_t headerSizeBytes = sizeof(int32_t)*4;

	uint32_t minSizeBytes = headerSizeBytes + MOVE_COUNT_MIN*sizeof(Move);
	uint32_t maxSizeBytes = headerSizeBytes + MOVE_COUNT_MAX*sizeof(Move);

	if (sizeBytes<minSizeBytes){
		return false;
	}else if (sizeBytes>maxSizeBytes){
		return false;
	}else{
		uint32_t movesSizeBytes = sizeBytes - headerSizeBytes;
		return (movesSizeBytes%sizeof(Move))==0;
	}


}
//=================================================================================================
bool GenericSetSettings::checkSet(Set &setToCheck){

	return true;
}
//=================================================================================================

