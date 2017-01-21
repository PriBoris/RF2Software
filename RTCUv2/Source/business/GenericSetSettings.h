
#ifndef GENERIC_SET_SETTINGS_H
#define GENERIC_SET_SETTINGS_H

#include <stdint.h>

#include "hmi/RxMessageQueue.h"


class GenericSetSettings{

public:

	struct Move{
		int32_t destinationPosition;
		int32_t speed;
	};

	static const uint32_t MOVE_COUNT_MAX = (50);

	struct Set{
		int32_t pause1;
		int32_t pause2;
		int32_t startPosition;
		int32_t moveCount;
		Move moves[MOVE_COUNT_MAX];
	};

	static Set set;	

	static void init();	
	static void report();
	static void load(RxMessage *message);

	static bool valid;



private:

	static uint32_t getSetSizeBytes();

};


#endif
