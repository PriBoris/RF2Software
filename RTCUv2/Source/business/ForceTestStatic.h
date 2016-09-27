
#ifndef FORCE_TEST_STATIC_H
#define FORCE_TEST_STATIC_H

#include <stdint.h>

#include "hmi/RxMessageQueue.h"




class ForceTestStatic{

public:

	static bool load(RxMessage *message);
	static int32_t pauseBeforeTestMsec;
	static int32_t testDurationMsec;
	static int32_t positionRel;
	static int32_t positionAbs;


	static int32_t getTestPosition();

	static const int32_t PHASE_HOMING=0;
	static const int32_t PHASE_PAUSE=1;
	static const int32_t PHASE_TEST_STATIC=8;




};



#endif
