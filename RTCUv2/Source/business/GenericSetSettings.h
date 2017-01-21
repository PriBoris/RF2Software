
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

	static const uint32_t MOVE_COUNT_MIN = (1);
	static const uint32_t MOVE_COUNT_MAX = (50);

	struct Set{
		int32_t pause1;
		int32_t pause2;
		int32_t startPosition;
		int32_t moveCount;
		Move moves[MOVE_COUNT_MAX];
	};

	static Set set;	
	static Set newSet;

	static void init();	
	static void report();
	static void load(RxMessage *message);

	static bool valid;



private:

	static uint32_t getSetSizeBytes();
	static bool checkSetSizeBytes(uint32_t sizeBytes);
	static bool checkSet(Set &setToCheck);



};


#endif
