
#include "servo.h"

#include "stm32f4xx_gpio.h"

#include "servo/encoder.h"
#include "business/MachineSettings.h"


const float Servo::SECONDS_PER_ROTATION_AT_1HZ = 249.4623656f;
const float Servo::MIN_FREQUENCY = 2.0f;
const float Servo::MAX_FREQUENCY = 50.0f;


/*void Servo::parkingBrakeOFF(){
	GPIOB->BSRRL = (1<<2);
}	
void Servo::parkingBrakeON(){
	GPIOB->BSRRH = (1<<2);
}
*/
void Servo::parkingBrake(bool brake){
	if (brake==false){
		// GPIOB->BSRRL = (1<<2);
		GPIOD->BSRRL = (1<<14);
	}else{
		// GPIOB->BSRRH = (1<<2);
		GPIOD->BSRRH = (1<<14);
	}
}




// void Servo::moveLeftON(){
// 	GPIOE->BSRRL = (1<<3);
// }
// void Servo::moveLeftOFF(){
// 	GPIOE->BSRRH = (1<<3);
// }

void Servo::movePositive(bool move){
	if (move==false){
		GPIOE->BSRRH = (1<<3);
	}else{
		GPIOE->BSRRL = (1<<3);
	}
}


// void Servo::moveRightON(){
// 	GPIOE->BSRRL = (1<<4);
// }
// void Servo::moveRightOFF(){
// 	GPIOE->BSRRH = (1<<4);
// }

void Servo::moveNegative(bool move){
	if (move==false){
		GPIOE->BSRRH = (1<<4);
	}else{
		GPIOE->BSRRL = (1<<4);
	}
}



/*void Servo::stopON(){
	GPIOE->BSRRL = (1<<1);
}
void Servo::stopOFF(){
	GPIOE->BSRRH = (1<<1);
}
*/
void Servo::brake(bool on){
	if (on==false){
		GPIOE->BSRRH = (1<<1);
	}else{
		GPIOE->BSRRL = (1<<1);
	}

}



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
