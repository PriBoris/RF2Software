


#ifndef SYSTEM_HEARTBEAT_H
#define SYSTEM_HEARTBEAT_H

#include <stdint.h>

class Heartbeat{
private:
	static uint32_t counter;
	static uint8_t mainTickCounter;

public:
	
	static void init();
	static void tick();
	
	static bool mainTick();
	static uint32_t getCounterValue();

	static bool mainTickReady;


};



#endif //SYSTEM_HEARTBEAT_H
