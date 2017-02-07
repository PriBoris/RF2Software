
#include "ForceTestDynamic.h"

#include <string.h>

#include "business/PersonalSettings.h"
#include "business/MachineSettings.h"
#include "servo/servo.h"


int32_t ForceTestDynamic::pauseBeforeTestMsec;
bool ForceTestDynamic::testType;

float ForceTestDynamic::servoFrequencyPositive = 0.0f;
float ForceTestDynamic::servoFrequencyNegative = 0.0f;

const int32_t ForceTestDynamic::PHASE_HOMING;
const int32_t ForceTestDynamic::PHASE_PAUSE;
const int32_t ForceTestDynamic::PHASE_TEST_CONCENTRIC;
const int32_t ForceTestDynamic::PHASE_TEST_ECCENTRIC;


//=================================================================================================
bool ForceTestDynamic::load(RxMessage *message){

	switch(message->tag){
	case Protocol::TAG_TestConcentric:
		testType = CONCENTRIC;
		break;
	case Protocol::TAG_TestEccentric:
		testType = ECCENTRIC;
		break;
	default:
		return false;

	}

	if (message->valueLen==sizeof(pauseBeforeTestMsec)){

		memcpy(&pauseBeforeTestMsec,message->value,sizeof(pauseBeforeTestMsec));
		if (
			(pauseBeforeTestMsec>=0)&&(pauseBeforeTestMsec<=60000)
			){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}
//=================================================================================================
int32_t ForceTestDynamic::getStartPosition(){

	if (testType==CONCENTRIC){
		return PersonalSettings::protocolStruct.positionMainB;
	}else{
		return PersonalSettings::protocolStruct.positionMainA;
	}

}
//=================================================================================================
int32_t ForceTestDynamic::getStopPosition(){

	if (testType==CONCENTRIC){
		return PersonalSettings::protocolStruct.positionMainA;
	}else{
		return PersonalSettings::protocolStruct.positionMainB;
	}

}
//=================================================================================================
void ForceTestDynamic::recalculateServoFrequency(){

	if (testType==CONCENTRIC){
		servoFrequencyPositive = Servo::rangeToFrequency(
			PersonalSettings::getMainRange(),
			PersonalSettings::protocolStruct.speedRelMainTestConcentric
			);
	}else{
		servoFrequencyPositive = Servo::rangeToFrequency(
			PersonalSettings::getMainRange(),
			PersonalSettings::protocolStruct.speedRelMainTestEccentric
			);
	}
	servoFrequencyNegative = -(servoFrequencyPositive);

}
//=================================================================================================
