
#ifndef EXCERCISE_SETTINGS_H
#define EXCERCISE_SETTINGS_H

#include <stdint.h>

#include "hmi/RxMessageQueue.h"


class ExcerciseSettings{




public:


	struct Set{
		int32_t pauseTime;
		int32_t firstInterruptionTime;
		int32_t secondInterruptionTime;
		int32_t	startPoint;
		int32_t	speedAB;
		int32_t	speedBA;
		int32_t	repsCount;
	};

	static const uint32_t MAX_SET_COUNT	= (50);
	static uint32_t setCount;

	static void init();	
	static void report();
	static void load(RxMessage *message);

	static Set sets[MAX_SET_COUNT];

	static bool valid;

	static const int32_t START_POSITION_A = 0;
	static const int32_t START_POSITION_B = 1;


private:

	static bool checkSet(Set *set);


};



#endif
