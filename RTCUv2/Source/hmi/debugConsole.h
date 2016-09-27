
#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#include <stdint.h>

class DebugConsole{

private:

	static bool enabled;

	static const uint32_t TX_BUFFER_LENGTH = 4096;
	static uint8_t txBuffer[TX_BUFFER_LENGTH];
	static uint32_t ptrPending;
	static uint32_t ptrTransmitted;

	static const uint32_t MAX_MESSAGE_LENGTH = 64;
	static uint8_t txMessage[MAX_MESSAGE_LENGTH];


public:

	static void init(bool enabled);
	static void process();

	static void pushMessage(char *msg);


};



#endif

