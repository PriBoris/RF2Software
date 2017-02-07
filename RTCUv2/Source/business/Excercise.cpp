
#include "Excercise.h"

#include "ExcerciseSettings.h"
#include "PersonalSettings.h"

#include "servo/servo.h"



const int32_t Excercise::PHASE_HOMING=0;
const int32_t Excercise::PHASE_PAUSE=1;
const int32_t Excercise::PHASE_ISOKINETIC_FIRSTMOVE=4;
const int32_t Excercise::PHASE_ISOKINETIC_FIRSTINTERRUPTION=5;
const int32_t Excercise::PHASE_ISOKINETIC_SECONDMOVE=6;
const int32_t Excercise::PHASE_ISOKINETIC_SECONDINTERRUPTION=7;

int32_t Excercise::actualSet;
int32_t Excercise::actualRepetition;

int32_t Excercise::setPauseCounterMsec;
float Excercise::servoFrequencyPositive=0.0f;
float Excercise::servoFrequencyNegative=0.0f;

int32_t Excercise::firstInterruptionCounterMsec;
int32_t Excercise::secondInterruptionCounterMsec;



//=================================================================================================
void Excercise::excerciseStart(){

	actualSet = 0;

}
//=================================================================================================
int32_t Excercise::getPositionMainHoming(){

	if (ExcerciseSettings::sets[actualSet].startPoint==ExcerciseSettings::START_POSITION_A){
		return PersonalSettings::protocolStruct.positionMainA;
	}else{
		return PersonalSettings::protocolStruct.positionMainB;
	}
	
}
//=================================================================================================
bool Excercise::isSetPauseDone(){

	setPauseCounterMsec-=100;
	if (setPauseCounterMsec<=0){
		return true;
	}else{
		return false;
	}

}
//=================================================================================================
void Excercise::recalculateServoFrequency(){

	if (ExcerciseSettings::sets[actualSet].startPoint==ExcerciseSettings::START_POSITION_A){

		servoFrequencyPositive = Servo::rangeToFrequency(
			PersonalSettings::getMainRange(),
			ExcerciseSettings::sets[actualSet].speedBA
			);

		servoFrequencyNegative = -Servo::rangeToFrequency(
			PersonalSettings::getMainRange(),
			ExcerciseSettings::sets[actualSet].speedAB
			);

	}else{

		servoFrequencyPositive = Servo::rangeToFrequency(
			PersonalSettings::getMainRange(),
			ExcerciseSettings::sets[actualSet].speedAB
			);

		servoFrequencyNegative = -Servo::rangeToFrequency(
			PersonalSettings::getMainRange(),
			ExcerciseSettings::sets[actualSet].speedBA
			);

	}

	//TODO: wrong result if B>A 


}
//=================================================================================================
int32_t Excercise::getPositionMainFirstMovement(){

	if (ExcerciseSettings::sets[actualSet].startPoint==ExcerciseSettings::START_POSITION_A){
		return PersonalSettings::protocolStruct.positionMainB;
	}else{
		return PersonalSettings::protocolStruct.positionMainA;
	}

}
//=================================================================================================
int32_t Excercise::getPositionMainSecondMovement(){

	if (ExcerciseSettings::sets[actualSet].startPoint==ExcerciseSettings::START_POSITION_A){
		return PersonalSettings::protocolStruct.positionMainA;
	}else{
		return PersonalSettings::protocolStruct.positionMainB;
	}


}
//=================================================================================================
int32_t Excercise::getSpeedFirstMovement(){

	if (ExcerciseSettings::sets[actualSet].startPoint==ExcerciseSettings::START_POSITION_A){
		return ExcerciseSettings::sets[actualSet].speedAB;
	}else{
		return ExcerciseSettings::sets[actualSet].speedBA;
	}
}
//=================================================================================================
int32_t Excercise::getSpeedSecondMovement(){
	if (ExcerciseSettings::sets[actualSet].startPoint==ExcerciseSettings::START_POSITION_A){
		return ExcerciseSettings::sets[actualSet].speedBA;
	}else{
		return ExcerciseSettings::sets[actualSet].speedAB;
	}
}
//=================================================================================================
void Excercise::setStart(){

	setPauseCounterMsec = ExcerciseSettings::sets[actualSet].pauseTime;
	actualRepetition = 0;

}
//=================================================================================================
void Excercise::repetitionStart(){

	firstInterruptionCounterMsec = ExcerciseSettings::sets[actualSet].firstInterruptionTime;
	secondInterruptionCounterMsec = ExcerciseSettings::sets[actualSet].secondInterruptionTime;

}
//=================================================================================================
bool Excercise::firstInterruptionEnabled(){

	if (firstInterruptionCounterMsec>0){
		return true;
	}else{
		return false;
	}

}
bool Excercise::secondInterruptionEnabled(){

	if (secondInterruptionCounterMsec>0){
		return true;
	}else{
		return false;
	}

}

bool Excercise::isFirstInterruptionDone(){

	firstInterruptionCounterMsec-=100;
	if (firstInterruptionCounterMsec<=0){
		return true;
	}else{
		return false;
	}

	
}
bool Excercise::isSecondInterruptionDone(){

	secondInterruptionCounterMsec-=100;
	if (secondInterruptionCounterMsec<=0){
		return true;
	}else{
		return false;
	}

	
}

void Excercise::repetitionDone(){


	actualRepetition++;


}


bool Excercise::isSetDone(){

	if (actualRepetition>=ExcerciseSettings::sets[actualSet].repsCount){
		actualSet++;
		return true;
	}else{

		return false;
	}



}

bool Excercise::isExcerciseDone(){

	if (actualSet>=ExcerciseSettings::setCount){
		return true;
	}else{
		return false;
	}

}

int32_t Excercise::getActualSetIndex(){

	return actualSet;

}

int32_t Excercise::getActualepetitionIndex(){

	return actualRepetition;

}


int32_t Excercise::getActualTimeToSet(){

	return setPauseCounterMsec;	
}

int32_t Excercise::getActualRepDirection(int32_t phase){

	if (phase==PHASE_ISOKINETIC_FIRSTMOVE){
		if(ExcerciseSettings::sets[actualSet].startPoint==ExcerciseSettings::START_POSITION_A){
			return 0;
		}else{
			return 1;
		}
	}else if(phase==PHASE_ISOKINETIC_SECONDMOVE){
		if(ExcerciseSettings::sets[actualSet].startPoint==ExcerciseSettings::START_POSITION_B){
			return 0;
		}else{
			return 1;
		}
	}else{
		return -1;
	}

}


int32_t Excercise::getActualTimeToMovement(int32_t phase){

	if (phase==PHASE_ISOKINETIC_FIRSTINTERRUPTION){
		return firstInterruptionCounterMsec;
	}else if(phase==PHASE_ISOKINETIC_SECONDINTERRUPTION){
		return secondInterruptionCounterMsec;
	}else{
		return 0;
	}
}

