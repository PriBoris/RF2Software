
#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <stdint.h>


class Heartbeat{
private:
	static uint32_t counter;
	static uint8_t mainTickCounter;
	static bool mainTickReady;
	static bool mainMidtickReady;
	static bool mainTickStarted;

public:
	
	static void init();
	static void tick();
	
	static bool mainTick();
	static bool mainMidtick();
	static void startMainTick();
	static uint32_t getCounterValue();



};






#endif

