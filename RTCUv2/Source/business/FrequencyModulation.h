
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
	bool direction;

	float x;
	float y;

public:

	void prepare(
		float mainFrequency,
		int32_t stopPosition,
		bool direction
		);


	float getFrequency();

	bool getDirection();


};



#endif
