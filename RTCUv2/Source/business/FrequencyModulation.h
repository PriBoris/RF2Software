
#ifndef FREQUENCY_MODULATION_H
#define FREQUENCY_MODULATION_H

#include <stdint.h>


class FrequencyModulation{

public:
	enum Law{
		LAW_0 = 0,
		LAW_2,
		LAW_4
	};


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
	float y;

	Law law;

public:

	void prepare(
		float mainFrequency,
		int32_t stopPosition,
		bool direction,
		Law law,
		int32_t minRange
		);


	float getFrequency();

	bool getDirection();



};



#endif
