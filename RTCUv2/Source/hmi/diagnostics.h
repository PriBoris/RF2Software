
#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H 

#include <stdint.h>

#include "protocol.h"

#include "hmi.h"


class Diagnostics {

private:
	
	static const uint32_t RX_BUFFER_LENGTH = 2048;
	static const uint32_t TX_BUFFER_LENGTH = 4096;

	static uint8_t rxBuffer[RX_BUFFER_LENGTH];
	static uint8_t txBuffer[TX_BUFFER_LENGTH];
	static uint8_t rxValue[HMI::RX_MAX_VALUE_LENGTH];


public:	
	static void init();	
	static void process();

	static Protocol protocol;
	
};



#endif
