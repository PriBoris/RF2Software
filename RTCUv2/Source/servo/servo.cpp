
#include "servo.h"

#include "stm32f4xx_gpio.h"

#include "servo/encoder.h"
#include "business/MachineSettings.h"


bool Servo::actualMoveDirection = POSITIVE_DIRECTION;

int32_t Servo::heatsinkTemperature = 0;
int32_t Servo::internalTemperature = 0;
int32_t Servo::motorTemperature = 0;


//==================================================================================================================
void Servo::init(){

	parkingBrake(true);

	disable(); // !!! this is necessary for servo to start properly !!!

}
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
void Servo::enable(){
	GPIOE->BSRRL = (1<<3);
}
//==================================================================================================================
void Servo::disable(){
	GPIOE->BSRRH = (1<<3);
}
//==================================================================================================================
// void Servo::movePositive(bool move){
// 	if (move==false){
// 		GPIOE->BSRRH = (1<<3);
// 	}else{
// 		GPIOE->BSRRL = (1<<3);
// 	}
// }
void Servo::movePositive(void){
	// moveNegative(false);
	// movePositive(true);
	// brake(false);
	actualMoveDirection = POSITIVE_DIRECTION;
}
// //==================================================================================================================
// void Servo::moveNegative(bool move){
// 	if (move==false){
// 		GPIOE->BSRRH = (1<<4);
// 	}else{
// 		GPIOE->BSRRL = (1<<4);
// 	}
// }
void Servo::moveNegative(void){
	// movePositive(false);
	// moveNegative(true);
	// brake(false);
	actualMoveDirection = NEGATIVE_DIRECTION;
}
// //==================================================================================================================
// void Servo::brake(bool on){
// 	if (on==false){
// 		GPIOE->BSRRH = (1<<1);
// 	}else{
// 		GPIOE->BSRRL = (1<<1);
// 	}
// }
// void Servo::brake(void){
// 	brake(true);
// 	movePositive(false);
// 	moveNegative(false);
// }
// //==================================================================================================================
bool Servo::getMoveDirection(){
	return actualMoveDirection;
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
//const float Servo::MILLISECONDS_PER_ROTATION_AT_1HZ = 341398.0f;

/*
1380 motor rotations per minute @ 50Hz
1380/60=23.00 motor rotations per second @ 50Hz
1395/60/339.41=0.06776465 output shaft rotations per second @ 50Hz
339.41*60/1380=14.75695 seconds per output shaft rotation @ 50Hz
339.41*60/1380*50=737.8478 seconds per output shaft rotation @ 1Hz
339.41*60/1380*50*1000=737847.826 milliseconds per output shaft rotation @ 1Hz
*/
const float Servo::MILLISECONDS_PER_ROTATION_AT_1HZ = 737847.8f;

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

		float millisecondsPerRange = 
			Servo::MILLISECONDS_PER_ROTATION_AT_1HZ
			* (float)range
			/ (float)Encoder::FULL_ROTATION_TICKS
			;

		float frequency = millisecondsPerRange/(float)absSpeedMsec;

		//TODO: refactor this block using negative frequency limits as well
		if (frequency < MachineSettings::protocolStructExtended.minPositiveServoFrequency){

			frequency = MachineSettings::protocolStructExtended.minPositiveServoFrequency;

		} else if (frequency > MachineSettings::protocolStructExtended.maxPositiveServoFrequency){

			frequency = MachineSettings::protocolStructExtended.maxPositiveServoFrequency;
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

	if (POSITIVE_DIRECTION==direction){	

		if (frequency < MachineSettings::protocolStructExtended.minPositiveServoFrequency){
			return MachineSettings::protocolStructExtended.minPositiveServoFrequency;
		}else if (frequency > MachineSettings::protocolStructExtended.maxPositiveServoFrequency){
			return MachineSettings::protocolStructExtended.maxPositiveServoFrequency;
		}else {
			return frequency;
		}

	}else{

		if (frequency > MachineSettings::protocolStructExtended.minNegativeServoFrequency){
			return MachineSettings::protocolStructExtended.minNegativeServoFrequency;
		}else if (frequency < MachineSettings::protocolStructExtended.maxNegativeServoFrequency){
			return MachineSettings::protocolStructExtended.maxNegativeServoFrequency;
		}else {
			return frequency;
		}

	}

}
//==================================================================================================================


