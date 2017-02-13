
#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>


class Servo{
	
private:

	static bool actualMoveDirection;
	

public:
	
	static const uint16_t LEFT_FREQUENCY_INDEX = 0; // 0 means Fixed Frequency 1
	static const uint16_t RIGHT_FREQUENCY_INDEX = 1; // 1 means Fixed Frequency 2

	static const float MILLISECONDS_PER_ROTATION_AT_1HZ;
	static const float MIN_FREQUENCY;
	static const float MAX_FREQUENCY;

	static const bool POSITIVE_DIRECTION = true;
	static const bool NEGATIVE_DIRECTION = false;

	static void parkingBrake(bool brake);	
	static void movePositive(bool move);
	static void moveNegative(bool move);
	static void brake(bool on);

	static void movePositive(void);
	static void moveNegative(void);
	static void brake(void);
	static bool getMoveDirection();




	static bool validateActualPosition();
	static bool validateActualPosition(bool direction);

	static float rangeToFrequency(
		int32_t absRange,
		int32_t absSpeedMsec
		);

	static float moveDurationToFrequency(
		int32_t startPosition,
		int32_t destinationPosition,
		int32_t durationMsec
		);

	static int32_t heatsinkTemperature;
	static int32_t internalTemperature;
	static int32_t motorTemperature;

};






#endif
