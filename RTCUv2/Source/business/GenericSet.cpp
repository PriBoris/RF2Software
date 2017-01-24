
#include "GenericSet.h"

#include "GenericSetSettings.h"


int32_t GenericSet::pauseCounterMsec;




//=================================================================================================
void GenericSet::start(){



}
//=================================================================================================
void GenericSet::pause1Start(){
	pauseCounterMsec = GenericSetSettings::set.pause1;
}
void GenericSet::pause2Start(){
	pauseCounterMsec = GenericSetSettings::set.pause2;
}
//=================================================================================================
bool GenericSet::isPauseDone(){

	if (pauseCounterMsec<=0){
		pauseCounterMsec = 0;
		return true;
	}else{
		pauseCounterMsec-=100;
		return false;
	}
}
//=================================================================================================
int32_t GenericSet::getPauseTimeRemaining(){
	return pauseCounterMsec;
}
//=================================================================================================


