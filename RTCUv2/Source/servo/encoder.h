
#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

class Encoder{
private:

	static uint32_t requestCounter;
	static uint32_t replyCounter;

	static uint16_t rawValue;
	static uint16_t value;



public:
	
	static void init();
	static void sendRequest();
	static void getReply();
	static uint16_t getValue();

	static const int32_t FULL_ROTATION_TICKS = 4096;

};



#endif
