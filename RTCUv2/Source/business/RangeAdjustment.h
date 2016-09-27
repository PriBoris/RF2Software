
#ifndef RANGE_ADJUSTMENT_H
#define RANGE_ADJUSTMENT_H

#include <stdint.h>

class RangeAdjustment{

private:



public:

	static float servoFrequency;

	static uint8_t actualButtonID;

	static const uint8_t BUTTONID_MAIN_MINUS = 1;
	static const uint8_t BUTTONID_MAIN_PLUS = 0;

	static const uint8_t BUTTONID_AUX1_MINUS = 2;
	static const uint8_t BUTTONID_AUX1_PLUS = 3;

	static const uint8_t BUTTONID_AUX2_MINUS = 4;
	static const uint8_t BUTTONID_AUX2_PLUS = 5;


};



#endif
