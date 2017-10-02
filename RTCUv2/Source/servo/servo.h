
#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>


class Servo{
	
private:

	static bool actualMoveDirection;
	static bool parkingBrakeState;

	// static void movePositive(bool move);
	// static void moveNegative(bool move);
	// static void brake(bool on);
	

public:
	
	static void init();

	static const uint16_t LEFT_FREQUENCY_INDEX = 0; // 0 means Fixed Frequency 1
	static const uint16_t RIGHT_FREQUENCY_INDEX = 1; // 1 means Fixed Frequency 2

	static const float MILLISECONDS_PER_ROTATION_AT_1HZ;

	static const bool POSITIVE_DIRECTION = true;
	static const bool NEGATIVE_DIRECTION = false;

	static void parkingBrake(bool brake);	
	static bool getParkingBrakeState(void);	

	static void enable();
	static void disable();

	static void movePositive(void);
	static void moveNegative(void);
	//static void brake(void);
	static bool getMoveDirection();




/*	static bool validateActualPosition();
	static bool validateActualPosition(bool direction);
*/
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

	static float limitFrequency(float frequency, bool direction);

};






#endif
