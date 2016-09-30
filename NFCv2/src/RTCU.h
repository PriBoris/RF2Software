
#ifndef RTCU_H
#define RTCU_H

#include "protocol.h"


class RTCU{
private:

	static const uint32_t RX_MAX_VALUE_LENGTH = 256;

	static const uint32_t RX_BUFFER_LENGTH = 256;
	static const uint32_t TX_BUFFER_LENGTH = 256;

	static uint8_t rxBuffer[RX_BUFFER_LENGTH];
	static uint8_t txBuffer[TX_BUFFER_LENGTH];
	static uint8_t rxValue[RX_MAX_VALUE_LENGTH];


public:

	static Protocol protocol;

	static void init();	

	static void processTx();
	static bool processRx();

	static uint32_t requestCounter;

};





#endif


