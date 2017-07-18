

#ifndef HMI_RX_MESSAGE_QUEUE_H
#define HMI_RX_MESSAGE_QUEUE_H

#include <stdint.h>

#include "hmi/hmi.h"


class RxMessage{
public:	
	uint8_t tag;
	uint16_t valueLen;
	uint32_t id;
	bool fromHMI;
	uint8_t value[HMI::RX_MAX_VALUE_LENGTH];
};

class RxMessageQueue{

private:


	static const uint32_t MAX_QUEUE_LEN = 8;
	static uint32_t ptrReceived;
	static uint32_t ptrProcessed;
	static RxMessage messages[MAX_QUEUE_LEN];


	static uint32_t getMessageCount();
	static bool isEmpty();
	static bool isFull();



public:

	static void init();



	static void push(
		uint8_t tag,
		uint16_t valueLen,
		uint32_t id,
		bool fromHMI,
		uint8_t* value
		);

	static void flush();

	static RxMessage* pop();


	static bool cancelMessageReceived();
	static bool resetErrorMessageReceived();

};



#endif //HMI_RX_MESSAGE_QUEUE_H
