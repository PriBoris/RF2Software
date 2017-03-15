
#ifndef FREQUENCY_MODULATION_H
#define FREQUENCY_MODULATION_H

#include <stdint.h>


class FrequencyModulation{


private:

	float mainFrequency;
	float frequency;
	int32_t startPosition;
	int32_t stopPosition;
	int32_t actualPosition;
	int32_t range;
	float frange;
	bool direction;
	int32_t minRange;
	bool rangeOk;

	float mainFrequencyToRangeRatio;

	float x;
	float x1;
	float y;

	int32_t accelerationLaw;
	int32_t decelerationLaw;
	int32_t actualLaw;


public:

	void prepare(
		float mainFrequency,
		int32_t stopPosition,
		bool direction,
		int32_t accelerationLaw,
		int32_t decelerationLaw,
		int32_t minRange
		);


	float getFrequency();

	bool getDirection();




	static const int32_t LAW_CREEP = 0;
	static const int32_t LAW_CONST = 1;
	static const int32_t LAW_POWER2 = 2;
	static const int32_t LAW_POWER4 = 3;
	static const int32_t LAW_QUARTER_POWER2 = 4;

	static const int32_t MIN_LAW = 0;
	static const int32_t MAX_LAW = 4;


};



#endif
