

#include "ExcerciseSettings.h"

#include <string.h>

#include "hmi/hmi.h"
#include "hmi/diagnostics.h"
//#include "hmi/debugConsole.h"
#include "business/MachineSettings.h"
#include "business/PersonalSettings.h"



uint32_t ExcerciseSettings::setCount;
ExcerciseSettings::Set ExcerciseSettings::sets[ExcerciseSettings::MAX_SET_COUNT];
bool ExcerciseSettings::valid;


//=================================================================================================
void ExcerciseSettings::init(){

	setCount = 0;
	memset(&sets[0],0,sizeof(sets));
	valid = false;

}
//=================================================================================================
void ExcerciseSettings::report(){

	if (setCount==0){
		
		Diagnostics::protocol.sendPacket(Protocol::TAG_ReportExcerciseSettings,0,0);
		HMI::protocol.sendPacket(Protocol::TAG_ReportExcerciseSettings,0,0);
		
	}else{

		Diagnostics::protocol.sendPacket(Protocol::TAG_ReportExcerciseSettings,(uint8_t*)&sets[0],setCount*sizeof(Set));
		HMI::protocol.sendPacket(Protocol::TAG_ReportExcerciseSettings,(uint8_t*)&sets[0],setCount*sizeof(Set));



	}





}
//=================================================================================================
void ExcerciseSettings::load(RxMessage *message){

	if (
		(message->tag==Protocol::TAG_LoadExcerciseSettings)&&
		(message->valueLen!=0)&&
		(MachineSettings::protocolStructExtendedValid==true)&&
		(PersonalSettings::protocolStructValid==true)
	){

		uint32_t newSetCount = (message->valueLen)/sizeof(Set);
		if ((newSetCount*sizeof(Set))==(message->valueLen)){

			Set newSet;

			uint32_t newSetCountChecked = 0;
			for(uint32_t s=0;s<newSetCount;s++){

				memcpy(
					&newSet,
					&(message->value[s*sizeof(Set)]),
					sizeof(Set)
					);
				if (checkSet(&newSet)==true){
					newSetCountChecked++;
				}
			}

			if (newSetCountChecked==newSetCount){

				setCount = newSetCount;
				for(uint32_t s=0;s<setCount;s++){

					memcpy(
						&sets[s],
						&(message->value[s*sizeof(Set)]),
						sizeof(Set)
						);

				}
				valid = true;

				//DebugConsole::pushMessage(" #ExcerciseSettings were loaded");

			}




		}


	}

	
}
//=================================================================================================
bool ExcerciseSettings::checkSet(Set *set){



	return true;
}
//=================================================================================================

