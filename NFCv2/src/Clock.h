
#include <stdint.h>


class Clock{
private:

	static const uint32_t HSE_FREQUENCY = 24000000;
	static const uint32_t SYSCLK_FREQUENCY = HSE_FREQUENCY;

	static uint32_t heartbeat;

	static bool mainTickStarted;
	static bool mainTickTriggered;
	static uint32_t mainTickCounter;
	static uint32_t mainTickTop;

public:	
	
	static void start();
	static void pause(uint32_t ms);
	
	static void heartbeatTick();
	
	static uint32_t getHeartbeat();

	static void startMainTick(uint32_t ms);
	static bool getMainTick();


};
