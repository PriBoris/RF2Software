
#ifndef NFC_H
#define NFC_H

#include <stdint.h>

#include "protocol.h"


class NFC{

private:

	static const uint32_t RX_BUFFER_LENGTH = 256;
	static const uint32_t TX_BUFFER_LENGTH = 256;
	static const uint32_t RX_VALUE_LENGTH = 256;

	static uint8_t rxBuffer[RX_BUFFER_LENGTH];
	static uint8_t txBuffer[TX_BUFFER_LENGTH];
	static uint8_t rxValue[RX_VALUE_LENGTH];


	static const uint32_t MAX_UID_LENGTH = 16;



public:

	static void init();	
	static void processTx();

	static Protocol protocol;



	
};



#endif
