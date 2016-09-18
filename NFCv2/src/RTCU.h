
#ifndef RTCU_H
#define RTCU_H

#include "protocol.h"


class RTCU{
private:

	static const uint32_t RX_BUFFER_LENGTH = 256;
	static const uint32_t TX_BUFFER_LENGTH = 256;

	static uint8_t rxBuffer[RX_BUFFER_LENGTH];
	static uint8_t txBuffer[TX_BUFFER_LENGTH];


public:

	static Protocol protocol;

	static void init();	

	static void process();

};





#endif


