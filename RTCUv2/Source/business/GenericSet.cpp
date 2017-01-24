
#include "GenericSet.h"

#include "GenericSetSettings.h"
#include "PositionTask.h"

int32_t GenericSet::pauseCounterMsec;




//=================================================================================================
void GenericSet::start(){

	pauseCounterMsec = 0;

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
int32_t GenericSet::getPositionMainStart(){

	return PositionTask::relPositionToAbsPosition(GenericSetSettings::set.startPosition);
}
//=================================================================================================


