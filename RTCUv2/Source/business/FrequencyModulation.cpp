

#include "FrequencyModulation.h"

#include <math.h>

#include "servo/encoder.h"

//================================================================================================
void FrequencyModulation::prepare(
	float mainFrequency,
	int32_t stopPosition,
	bool direction
	){

	this->mainFrequency = mainFrequency;
	this->stopPosition = stopPosition;
	this->startPosition = this->actualPosition = Encoder::getValue();
	this->direction = direction;

	this->x = 0.0f;
	this->y = 0.0f;
	
}
//================================================================================================
bool FrequencyModulation::getDirection(){
	return direction;	
}
//================================================================================================
float FrequencyModulation::getFrequency(
	){

	this->actualPosition = Encoder::getValue();

	{
		float xTemp = (float)(this->actualPosition-this->startPosition)/(float)(this->stopPosition-this->startPosition);
		if (xTemp > 1.0f){
			xTemp = 1.0f;
		}else if (xTemp < 0.0f){
			xTemp = 0.0f;
		}else{
		}
		this->x = xTemp;
	}

	{
		// LAW = 2
		float yTemp = 2*(this->x-0.5f);
		this->y = 1-yTemp*yTemp;
	}

	this->frequency = this->mainFrequency * this->y;


	return mainFrequency;
}
//================================================================================================


