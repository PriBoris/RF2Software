

#include "FrequencyModulation.h"

#include <math.h>

#include "servo/encoder.h"

//================================================================================================
void FrequencyModulation::prepare(
	float mainFrequency,
	int32_t stopPosition,
	bool direction,
	Law law,
	int32_t minRange
	){

	this->mainFrequency = mainFrequency;
	this->stopPosition = stopPosition;
	this->startPosition = this->actualPosition = Encoder::getValue();
	this->direction = direction;
	this->range = (this->stopPosition - this->startPosition);
	this->frange = (float)this->range;

	{
		this->minRange = minRange;
		int32_t absRange = (this->range>0)?(this->range):(-this->range);
		if (absRange>=minRange){
			this->rangeOk = true;
		}else{
			this->rangeOk = false;
		}
	}





	this->mainFrequencyToRangeRatio = this->mainFrequency /	this->frange;

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
		float xTemp = (float)(this->actualPosition-this->startPosition)/this->frange;
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
		if (this->rangeOk!=false){
			float yTemp = 2*(this->x-0.5f);
			this->y = 1-yTemp*yTemp;
		}else{
			this->y = 0;
		}

		break;
	case LAW_4:
		if (this->rangeOk!=false){
			float yTemp = 2*(this->x-0.5f);
			yTemp *= yTemp;
			this->y = 1-yTemp*yTemp;
		}else{
			this->y = 0;
		}
		break;
	}


	this->frequency = this->mainFrequency * this->y;

	//return this->mainFrequency;
	return this->frequency;
}
//================================================================================================


