
#include "GenericSet.h"

#include "GenericSetSettings.h"
#include "PositionTask.h"

int32_t GenericSet::pauseCounterMsec;

int32_t GenericSet::moveIndex;



//=================================================================================================
void GenericSet::start(){

	pauseCounterMsec = 0;
	moveIndex= 0;
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

	return 
		PositionTask::relPositionToAbsPosition(
			GenericSetSettings::set.startPositionRel
			);
}
//=================================================================================================
void GenericSet::moveComplete(){
	moveIndex++;
}
//=================================================================================================
bool GenericSet::isSetComplete(){
	return (moveIndex==GenericSetSettings::set.moveCount);
}
//=================================================================================================
int32_t GenericSet::getMoveDestinationPosition(){

	if (moveIndex<GenericSetSettings::set.moveCount){
		return 
			PositionTask::relPositionToAbsPosition(
				GenericSetSettings::set.moves[moveIndex].destinationPositionRel
				);
	}else{
		return 0;
	}
	
}
//=================================================================================================
int32_t GenericSet::getMoveIndex(){
	return moveIndex;
}
//=================================================================================================



