
#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

class Encoder{
private:

	static uint32_t requestCounter;
	static uint32_t replyCounter;

	static uint16_t rawValue;

public:
	
	static void init();
	static void sendRequest();
	static void getReply();
	static int32_t getValue();


	/*
	wheels diameters^
	116mm/51mm=2.2745
	32768 * 2.2745 = 74531.137254901960784313725490196
	*/
//	static const int32_t FULL_ROTATION_TICKS = (32768);
	static const int32_t FULL_ROTATION_TICKS = (74531);



};



#endif
