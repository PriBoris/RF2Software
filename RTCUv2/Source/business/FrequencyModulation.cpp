

#include "FrequencyModulation.h"

#include "servo/encoder.h"

//================================================================================================
void FrequencyModulation::prepare(
	float mainFrequency,
	int32_t stopPosition,
	bool direction
	){

	this->mainFrequency = mainFrequency;
	this->stopPosition = stopPosition;
	this->startPosition = Encoder::getValue();
	this->direction = direction;

	
}
//================================================================================================
float FrequencyModulation::getFrequency(
	){


	return mainFrequency;
}
//================================================================================================
bool FrequencyModulation::getDirection(){
	return direction;	
}
//================================================================================================


