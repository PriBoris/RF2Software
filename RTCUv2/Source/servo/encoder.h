
#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

class Encoder{
private:

	static uint32_t requestCounter;
	static uint32_t replyCounter;

	static uint16_t rawValue;
	//static uint16_t value;

	//static const uint16_t VALUE_MASK = 0x7FFF;
	//static const uint16_t VALUE_ADDEND = 0;
	//static const uint16_t VALUE_SUBTRAHEND = 10000;
	//static const int32_t VALUE_SIGN = +1;

public:
	
	static void init();
	static void sendRequest();
	static void getReply();
	static int32_t getValue();

//	static const int32_t FULL_ROTATION_TICKS = 4096;
	static const int32_t FULL_ROTATION_TICKS = 32768;

};



#endif
