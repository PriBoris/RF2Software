
#include "servo.h"

#include "stm32f4xx_gpio.h"

#include "servo/encoder.h"
#include "business/MachineSettings.h"


const float Servo::SECONDS_PER_ROTATION_AT_1HZ = 249.4623656f;
const float Servo::MIN_FREQUENCY = 2.0f;
const float Servo::MAX_FREQUENCY = 30.0f;//50.0f;

bool Servo::actualMoveDirection = POSITIVE_DIRECTION;


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
float Servo::absSpeedToFrequency(
	int32_t fullRange,
	int32_t absSpeedMsec
){

	if (fullRange==0){

		return 0.0f;

	}else{

		float absSpeedSec = (float)absSpeedMsec/1000.0f;

		float secondsPerRange = Servo::SECONDS_PER_ROTATION_AT_1HZ*(float)fullRange/(float)Encoder::FULL_ROTATION_TICKS;

		float frequency = secondsPerRange/(float)absSpeedSec;
		if (frequency<Servo::MIN_FREQUENCY){
			frequency=Servo::MIN_FREQUENCY;
		}
		if (frequency>Servo::MAX_FREQUENCY){
			frequency=Servo::MAX_FREQUENCY;
		}

		return frequency;
		
	}


}
//==================================================================================================================
float Servo::relSpeedToFrequency(
	int32_t relRange,
	int32_t relSpeedMsec
){

	return absSpeedToFrequency(relRange,relSpeedMsec);

}
//==================================================================================================================
