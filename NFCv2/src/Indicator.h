
#ifndef INDICATOR_H
#define INDICATOR_H

#include <stdint.h>

class Indicator{
private:

	static uint32_t downCounter;

public:
	static void start();
	static void tick();
	static void flash();


};


#endif
