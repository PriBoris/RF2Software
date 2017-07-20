
#ifndef SERVO_STRAIN_GAUGE_H
#define SERVO_STRAIN_GAUGE_H

#include <stdint.h>


class StrainGauge{
private:

	static uint8_t spiPollByte(uint8_t txByte);

	static void csAssert();
	static void csDeassert();

	const static uint32_t BUFFER_LENGTH	= (512);
	static int32_t results[BUFFER_LENGTH];
	static uint32_t resultsPtr;
	static uint32_t resultsCount;
	static bool fault;

	static uint32_t counterExti;
	static uint8_t byteCounter;
	static uint8_t adcData[5];



public:
	static void init();

	static void readyInterruptHandler();
	static void spiInterruptHandler();

	static int32_t getFilteredResult(void);
	

};	


#endif //SERVO_STRAIN_GAUGE_H
