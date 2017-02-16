
#include "servo.h"

#include "stm32f4xx_gpio.h"

#include "servo/encoder.h"
#include "business/MachineSettings.h"


bool Servo::actualMoveDirection = POSITIVE_DIRECTION;

int32_t Servo::heatsinkTemperature = 0;
int32_t Servo::internalTemperature = 0;
int32_t Servo::motorTemperature = 0;

//==================================================================================================================
void Servo::parkingBrake(bool brake){
	if (brake==false){
		// GPIOB->BSRRL = (1<<2);
		GPIOD->BSRRL = (1<<14);
	}else{
		// GPIOB->BSRRH = (1<<2);
		GPIOD->BSRRH = (1<<14);
	}
}
//==================================================================================================================
void Servo::movePositive(bool move){
	if (move==false){
		GPIOE->BSRRH = (1<<3);
	}else{
		GPIOE->BSRRL = (1<<3);
	}
}
//==================================================================================================================
void Servo::moveNegative(bool move){
	if (move==false){
		GPIOE->BSRRH = (1<<4);
	}else{
		GPIOE->BSRRL = (1<<4);
	}
}
//==================================================================================================================
void Servo::brake(bool on){
	if (on==false){
		GPIOE->BSRRH = (1<<1);
	}else{
		GPIOE->BSRRL = (1<<1);
	}
}
//==================================================================================================================
void Servo::movePositive(void){
	moveNegative(false);
	movePositive(true);
	brake(false);
	actualMoveDirection = POSITIVE_DIRECTION;
}
void Servo::moveNegative(void){
	movePositive(false);
	moveNegative(true);
	brake(false);
	actualMoveDirection = NEGATIVE_DIRECTION;
}
void Servo::brake(void){
	brake(true);
	movePositive(false);
	moveNegative(false);
}
bool Servo::getMoveDirection(){
	return actualMoveDirection;
}
//==================================================================================================================



bool Servo::validateActualPosition(){

	int32_t encoderActualValue = Encoder::getValue();
	if (encoderActualValue>MachineSettings::protocolStructExtended.positionMainMax){
		return false;
	}else if (encoderActualValue<MachineSettings::protocolStructExtended.positionMainMin){
		return false;
	}else{
		return true;
	}

}
//==================================================================================================================
bool Servo::validateActualPosition(bool direction){

	int32_t encoderActualValue = Encoder::getValue();
	if (POSITIVE_DIRECTION==direction){

		// validate before moving servo in positive direction
		if (encoderActualValue>MachineSettings::protocolStructExtended.positionMainMax){
			return false;
		}else{
			return true;
		}

	}else if (NEGATIVE_DIRECTION==direction){

		// validate before moving servo in negative direction
		if (encoderActualValue<MachineSettings::protocolStructExtended.positionMainMin){
			return false;
		}else{
			return true;
		}

	}else{
		return false;
	}

}
//==================================================================================================================
/*
1395 motor rotations per minute @ 50Hz
1395/60=23.25 motor rotations per second @ 50Hz
1395/60/158.75=0.14645669 output shaft rotations per second @ 50Hz
158.75*60/1395=6.82796 seconds per output shaft rotation @ 50Hz
158.75*60/1395*50=341.398 seconds per output shaft rotation @ 1Hz
158.75*60/1395*50*1000=341398 milliseconds per output shaft rotation @ 1Hz
*/

const float Servo::MILLISECONDS_PER_ROTATION_AT_1HZ = 341398.0f;

const float Servo::MIN_FREQUENCY = 2.0f;
const float Servo::MAX_FREQUENCY = 30.0f;//50.0f;

const float Servo::MIN_NEGATIVE_FREQUENCY = -MIN_FREQUENCY;
const float Servo::MAX_NEGATIVE_FREQUENCY = -MAX_FREQUENCY;

//==================================================================================================================
float Servo::rangeToFrequency(
	int32_t range,
	int32_t absSpeedMsec
){

	if (range<=0){

		return 0.0f;

	}else if (absSpeedMsec<=0){

		return 0.0f;

	} else {

		float millisecondsPerRange = Servo::MILLISECONDS_PER_ROTATION_AT_1HZ*(float)range/(float)Encoder::FULL_ROTATION_TICKS;

		float frequency = millisecondsPerRange/(float)absSpeedMsec;

		if (frequency<Servo::MIN_FREQUENCY){
			frequency=Servo::MIN_FREQUENCY;
		} else if (frequency>Servo::MAX_FREQUENCY){
			frequency=Servo::MAX_FREQUENCY;
		}

		return frequency;
		
	}


}
//==================================================================================================================
float Servo::moveDurationToFrequency(
	int32_t startPosition,
	int32_t destinationPosition,
	int32_t durationMsec
	){

	int32_t moveRange = destinationPosition - startPosition;
	if (moveRange<0){
		moveRange = -moveRange;
	}

	return rangeToFrequency(moveRange,durationMsec);
}
//==================================================================================================================
float Servo::limitFrequency(float frequency, bool direction){

/*	if (frequency<Servo::MIN_FREQUENCY){
		return Servo::MIN_FREQUENCY;
	} else if (frequency>Servo::MAX_FREQUENCY){
		frequency=Servo::MAX_FREQUENCY;
	}
*/

	if (POSITIVE_DIRECTION==direction){	

		if (frequency<Servo::MIN_FREQUENCY){
			return Servo::MIN_FREQUENCY;
		}else if (frequency>Servo::MAX_FREQUENCY){
			return Servo::MAX_FREQUENCY;
		}else {
			return frequency;
		}

	}else{

		if (frequency>Servo::MIN_NEGATIVE_FREQUENCY){
			return Servo::MIN_NEGATIVE_FREQUENCY;
		}else if (frequency<Servo::MAX_NEGATIVE_FREQUENCY){
			return Servo::MAX_NEGATIVE_FREQUENCY;
		}else {
			return frequency;
		}

	}

}
//==================================================================================================================


