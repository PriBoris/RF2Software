
#ifndef GENERIC_SET_H
#define GENERIC_SET_H

#include <stdint.h>

class GenericSet{
private:


	static int32_t pause1CounterMsec;

public:

	static const int32_t PHASE_PAUSE1 = 9;
	static const int32_t PHASE_HOMING = 0;
	static const int32_t PHASE_PAUSE2 = 10;
	static const int32_t PHASE_GENERIC_MOVE = 11;


	static void start();


	static void pause1Start();
	static bool isPause1Done();
	static int32_t getPause1TimeRemaining();


};



#endif


