
#include "GenericSetSettings.h"

#include <string.h>

#include "hmi/hmi.h"
#include "hmi/diagnostics.h"



GenericSetSettings::Set GenericSetSettings::set;	
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
