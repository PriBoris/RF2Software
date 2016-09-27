
#ifndef HMI_H
#define HMI_H


#include <stdint.h>

#include "protocol.h"

class HMI {
public:
	static const uint32_t RX_MAX_VALUE_LENGTH = 1024;
	
private:
	
	static const uint32_t RX_BUFFER_LENGTH = 2048;
	static const uint32_t TX_BUFFER_LENGTH = 4096;

	static uint8_t rxBuffer[RX_BUFFER_LENGTH];
	static uint8_t txBuffer[TX_BUFFER_LENGTH];
	static uint8_t rxValue[RX_MAX_VALUE_LENGTH];


public:	
	static void init();	
	static void process();

	static void powerOn();
	static void powerOff();	
	static void powerKeyAssert();
	static void powerKeyDeassert();
	
	static bool stopButtonPressed();	

	static Protocol protocol;



	
};






#endif
