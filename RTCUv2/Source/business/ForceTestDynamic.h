
#ifndef FORCE_TEST_DYNAMIC_H
#define FORCE_TEST_DYNAMIC_H

#include <stdint.h>

#include "hmi/RxMessageQueue.h"




class ForceTestDynamic{

public:

	static bool testType;
	static const bool CONCENTRIC = false;
	static const bool ECCENTRIC = true;

	static bool load(RxMessage *message);
	static int32_t pauseBeforeTestMsec;

	static int32_t getStartPosition();
	static int32_t getStopPosition();

	static void recalculateServoFrequency();

	static float servoFrequencyPositive;
	static float servoFrequencyNegative;	



	static const int32_t PHASE_HOMING=0;
	static const int32_t PHASE_PAUSE=1;
	static const int32_t PHASE_TEST_CONCENTRIC=2;
	static const int32_t PHASE_TEST_ECCENTRIC=3;



};






#endif

