


#ifndef RTC_H
#define RTC_H

#include <stdint.h>




namespace NV{


class RealTimeClock{

private:

	static uint32_t prevTRValue;

	static uint8_t bcdByteToDec(uint8_t bcd);

public:	

	typedef struct 
	{
		uint8_t year;//00-99
		uint8_t month;//01-12
		uint8_t date;//01-31
		uint8_t hours;//00-23
		uint8_t minutes;//00–59
		uint8_t seconds;//00–59
	} TCurrentDateTime;//TODO: packed
	
	static void init();
	static void poll();

	static void getCurrentDateTime(TCurrentDateTime &currentDateTime);



	
};

};




#endif

