
#include "GenericSet.h"

#include "GenericSetSettings.h"


int32_t GenericSet::pause1CounterMsec;




//=================================================================================================
void GenericSet::start(){



}
//=================================================================================================
void GenericSet::pause1Start(){

	pause1CounterMsec = GenericSetSettings::set.pause1;


}
//=================================================================================================
bool GenericSet::isPause1Done(){

	if (pause1CounterMsec<=0){
		pause1CounterMsec = 0;
		return true;
	}else{
		pause1CounterMsec-=100;
		return false;
	}


}
//=================================================================================================
int32_t GenericSet::getPause1TimeRemaining(){
	return pause1CounterMsec;
}
//=================================================================================================


