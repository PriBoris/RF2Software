
#ifndef HMI_DIAGNOSTICS_H
#define HMI_DIAGNOSTICS_H

#include <stdint.h>

#include "protocol.h"

class Diagnostics {

private:
	static const uint32_t RX_MAX_VALUE_LENGTH = 512;
	
	static const uint32_t RX_BUFFER_LENGTH = 2048;
	static const uint32_t TX_BUFFER_LENGTH = 4096;

	static uint8_t rxBuffer[RX_BUFFER_LENGTH];
	static uint8_t txBuffer[TX_BUFFER_LENGTH];

	static uint8_t rxValue[RX_MAX_VALUE_LENGTH];


public:	
	static void init();	
	static void process();	

	static Protocol protocol;

	
};



#endif //HMI_DIAGNOSTICS_H
