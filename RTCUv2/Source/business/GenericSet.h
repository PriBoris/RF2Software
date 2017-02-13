
#ifndef GENERIC_SET_H
#define GENERIC_SET_H

#include <stdint.h>

class GenericSet{
private:


	static int32_t pauseCounterMsec;

	static int32_t moveIndex;

	static int32_t getMoveDuration();


public:

	static const int32_t PHASE_PAUSE1 = 9;
	static const int32_t PHASE_HOMING = 0;
	static const int32_t PHASE_PAUSE2 = 10;
	static const int32_t PHASE_GENERIC_MOVE = 11;


	static void start();


	static void pause1Start();
	static void pause2Start();
	static bool isPauseDone();
	static int32_t getPauseTimeRemaining();

	static int32_t getPositionMainStart();


	static void moveComplete();
	static bool isSetComplete();

	static bool isMoveStatic();
	static void staticMoveStart();
	static bool isStaticMoveDone();

	static int32_t getMoveDestinationPosition();
	static int32_t getMoveStartPosition();
	static int32_t getMoveIndex();


	static void recalculateServoFrequency(bool direction);
	static float servoFrequencyPositive_;
	static float servoFrequencyNegative_;




};



#endif


