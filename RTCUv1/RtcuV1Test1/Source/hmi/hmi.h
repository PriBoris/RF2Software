

#ifndef HMI_H
#define HMI_H

#include <stdint.h>

#include "protocol.h"


class HMI {

public:
	static const uint32_t RX_MAX_VALUE_LENGTH = 512;

private:
	
	static void txEnableAssert();
	static void txEnableDeassert();

	static const uint32_t RX_BUFFER_LENGTH = 2048;
	static const uint32_t TX_BUFFER_LENGTH = 4096;

	static uint8_t rxBuffer[RX_BUFFER_LENGTH];
	static uint8_t txBuffer[TX_BUFFER_LENGTH];


public:	
	static void init();	
	static void process();	

	static Protocol protocol;

	
};


class HmiRxMessage{

public:	
	uint8_t tag;
	uint16_t valueLen;
	uint32_t id;
	uint8_t value[HMI::RX_MAX_VALUE_LENGTH];
};

#define HMI_RX_MESSAGE_QUEUE_LEN	(32)
extern HmiRxMessage hmiRxMessageQueue[HMI_RX_MESSAGE_QUEUE_LEN];
extern uint16_t hmiRxMessageQueuePtrReceived;
extern uint16_t hmiRxMessageQueuePtrProcessed;

extern void flushHmiRxMessageQueue(void);
extern bool isHmiRxMessageQueueNotEmpty(void);
extern HmiRxMessage* popHmiRxMessage(void);







#endif
