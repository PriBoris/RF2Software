

#ifndef STRAIN_GAUGE_H
#define STRAIN_GAUGE_H

#include <stdint.h>


class StrainGauge{
private:	
	
	static void syncAssert();
	static void syncDeassert();
	static void cs1Assert();
	static void cs1Deassert();
	static void cs2Assert();
	static void cs2Deassert();

	static uint8_t pollByte(uint8_t txByte);

	static uint32_t readyCounter;
	static uint32_t valueCounter;
	static uint_fast8_t byteCounter1;
	static bool fault;

	static void setMisoMode();
	static void setRdyMode();

	static uint8_t adcBytes[5];
	static const uint32_t valueBufferLength = 512;
	static int32_t valueBuffer[valueBufferLength];
	static uint32_t valueBufferPtr;
	static const uint32_t filterLength = 64;//16;

	static const int32_t VALUE_OFFSET = 13000;

public:
	static void init();

	static void rdyInterrupt();
	static void misoInterrupt();
	static bool getFault();
	static int32_t getFilteredValue(void);

};




#endif

