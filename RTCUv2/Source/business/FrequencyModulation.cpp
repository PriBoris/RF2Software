

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

	if (this->rangeOk==false){

		actualLaw = 0;
		this->x1 = 0.0f;

	}else{

		if (this->x<0.5f){
			//acceleration
			this->x1 = 1.0f-2.0f*(this->x);
			actualLaw = accelerationLaw;
		}else{
			//deceleration
			this->x1 = -1.0f+2.0f*(this->x);
			actualLaw = decelerationLaw;
		}

	}

	this->y = 0.0f;
	switch(actualLaw){
	case 0:
		this->y = 0.0f; // 1-1	
		break;
	case 1:
		this->y = 1.0f - this->x1;
		break;
	case 2:
		this->y = 1.0f - this->x1*this->x1;
		break;
	case 3:
		this->y = 1.0f - this->x1*this->x1*this->x1;
		break;
	case 4:
		{
			float x2 = this->x1*this->x1;
			this->y = 1.0f - x2*x2;
		}
		break;
	case 5:
		{
			float x2 = this->x1*this->x1;
			this->y = 1.0f - x2*x2*this->x1;
		}
		break;
	case 6:
		{
			float x3 = this->x1*this->x1*this->x1;
			this->y = 1.0f - x3*x3;
		}
		break;
	default:// >6
		{
			float x2 = this->x1*this->x1;
			float x4 = x2*x2;
			float x8 = x4*x4;
			float x16 = x8*x8;
			this->y = 1.0f - x16;
		}
		break;

	}

	this->frequency = this->mainFrequency * this->y;

	return this->frequency;
}
//================================================================================================


