

#include "PositionTask.h"


#include "business/PersonalSettings.h"
#include "business/MachineSettings.h"
#include "business/PositionTask.h"


#include "servo/servo.h"
#include "servo/encoder.h"

//=================================================================================================
bool PositionTask::checkPositionStatically(int32_t taskPosition){

	int32_t actualPosition = Encoder::getValue();
	
	int32_t permissibleError = MachineSettings::getMainRange()/100; //1%

	if (actualPosition>(taskPosition+permissibleError)){
		return false;
	}else if (actualPosition<(taskPosition-permissibleError)){
		return false;
	}else{
		return true;// [actualPosition] is close to [parkingPosition]
	}

}
//=================================================================================================
bool PositionTask::checkPositionDynamically(int32_t taskPosition, bool servoMoveDirection){

	int32_t actualPosition = Encoder::getValue();

//	int32_t permissibleError = MachineSettings::getMainRange()/100; //1%
	int32_t permissibleError = 0;

	if (servoMoveDirection==Servo::POSITIVE_DIRECTION){

		if (actualPosition>(taskPosition-permissibleError)){
			return true;
		}else{
			return false;
		}

	}else{

		if (actualPosition<(taskPosition+permissibleError)){
			return true;
		}else{
			return false;
		}
	}


}
//=================================================================================================
bool PositionTask::getDirection(int32_t taskPosition){

	int32_t actualPosition = Encoder::getValue();

	if (taskPosition>=actualPosition){

		return Servo::POSITIVE_DIRECTION;
	}else{

		return Servo::NEGATIVE_DIRECTION;
	}



}
//=================================================================================================
int32_t PositionTask::getRelativePosition(bool saturate){

	int32_t actualPosition = Encoder::getValue();

	int32_t relativePosition = (actualPosition-PersonalSettings::protocolStruct.positionMainA)*10000;

	relativePosition = relativePosition/(PersonalSettings::protocolStruct.positionMainB-PersonalSettings::protocolStruct.positionMainA);

	if (saturate==true){
		if (relativePosition<0){
			relativePosition=0;
		}else if (relativePosition>10000){
			relativePosition = 10000;
		}
	}


	return relativePosition;


}
//=================================================================================================
int32_t PositionTask::relPositionToAbsPosition(int32_t relPosition){


	float absPosition = 
		(float)relPosition/10000.0f
		*(float)(PersonalSettings::protocolStruct.positionMainB-PersonalSettings::protocolStruct.positionMainA)
		+(float)PersonalSettings::protocolStruct.positionMainA
		;

	return (int32_t)absPosition;
}
//=================================================================================================
