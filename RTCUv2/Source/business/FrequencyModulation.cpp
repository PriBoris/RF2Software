

#include "FrequencyModulation.h"

#include <math.h>

#include "servo/encoder.h"

//================================================================================================
void FrequencyModulation::prepare(
	float mainFrequency,
	int32_t stopPosition,
	bool direction,
	int32_t accelerationLaw,
	int32_t decelerationLaw,
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


	this->accelerationLaw = accelerationLaw;
	this->decelerationLaw = decelerationLaw;

	if (this->accelerationLaw < MIN_LAW){
		this->accelerationLaw = MIN_LAW;
	}
	if (this->decelerationLaw < MIN_LAW){
		this->decelerationLaw = MIN_LAW;
	}
	if (this->accelerationLaw > MAX_LAW){
		this->accelerationLaw = MAX_LAW;
	}
	if (this->decelerationLaw > MAX_LAW){
		this->decelerationLaw = MAX_LAW;
	}

	
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

/*	int32_t law;
	if (this->x<0.5f){
		//acceleration
		this->x = 1.0f-(this->x);
		law = accelerationLaw;
	}else{
		//deceleration
		law = decelerationLaw;
	}

	switch(law){
	default:	
	case 1:
		



	}
*/


	{
		if (this->rangeOk!=false){
			float yTemp = 2*(this->x-0.5f);
			this->y = 1-yTemp*yTemp;
		}else{
			this->y = 0;
		}
	}

	this->frequency = this->mainFrequency * this->y;

	//return this->mainFrequency;
	return this->frequency;
}
//================================================================================================


