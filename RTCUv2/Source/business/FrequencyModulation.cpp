

#include "FrequencyModulation.h"

#include <math.h>

#include "servo/encoder.h"

//================================================================================================
void FrequencyModulation::prepare(
	float mainFrequency,
	int32_t stopPosition,
	bool direction,
	Law law
	){

	this->mainFrequency = mainFrequency;
	this->stopPosition = stopPosition;
	this->startPosition = this->actualPosition = Encoder::getValue();
	this->direction = direction;

	this->mainFrequencyToRangeRatio = this->mainFrequency /	 (float)(this->stopPosition - this->startPosition);

	this->x = 0.0f;
	this->y = 0.0f;

	this->law = law;
	
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

	switch(this->law){
	default:
	case LAW_2:
		{
			float yTemp = 2*(this->x-0.5f);
			this->y = 1-yTemp*yTemp;
		}
		break;
	case LAW_4:
		{
			float yTemp = 2*(this->x-0.5f);
			yTemp *= yTemp;
			yTemp *= yTemp;//6
			this->y = 1-yTemp*yTemp;
		}
		break;
	}


	this->frequency = this->mainFrequency * this->y;

	//return this->mainFrequency;
	return this->frequency;
}
//================================================================================================


