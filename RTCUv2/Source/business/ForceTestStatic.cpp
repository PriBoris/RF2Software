
#include "ForceTestStatic.h"

#include <string.h>

#include "business/PersonalSettings.h"
#include "business/MachineSettings.h"
#include "business/PositionTask.h"
#include "servo/servo.h"

int32_t ForceTestStatic::pauseBeforeTestMsec;
int32_t ForceTestStatic::testDurationMsec;
int32_t ForceTestStatic::positionRel;
int32_t ForceTestStatic::positionAbs;

const int32_t ForceTestStatic::PHASE_HOMING;
const int32_t ForceTestStatic::PHASE_PAUSE;
const int32_t ForceTestStatic::PHASE_TEST_STATIC;



bool ForceTestStatic::load(RxMessage *message){

	if (message->valueLen==(sizeof(pauseBeforeTestMsec)+sizeof(testDurationMsec)+sizeof(positionRel))){

		memcpy(&pauseBeforeTestMsec,&message->value[0],sizeof(pauseBeforeTestMsec));
		memcpy(&testDurationMsec,&message->value[4],sizeof(testDurationMsec));
		memcpy(&positionRel,&message->value[8],sizeof(positionRel));
		if (
			(pauseBeforeTestMsec>=0)&&(pauseBeforeTestMsec<=60000)&&
			(testDurationMsec>=0)&&(testDurationMsec<=60000)&&
			(positionRel>=0)&&(positionRel<=10000)
		){
			positionAbs = PositionTask::relPositionToAbsPosition(positionRel);
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}

}


int32_t ForceTestStatic::getTestPosition(){

	return positionAbs;


}
