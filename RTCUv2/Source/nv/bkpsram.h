


#ifndef BKPSRAM_H
#define BKPSRAM_H

#include <stdint.h>


namespace NV{

class BkpSram{
	
private:


public:	
	
	static void init();
	static uint32_t settingsBlock0Addr();
	static uint32_t settingsBlock1Addr();
	static uint32_t odometerBlock0Addr();
	static uint32_t odometerBlock1Addr();
	
};

};


#endif

