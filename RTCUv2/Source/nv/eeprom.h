


#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>


namespace NV{

class Eeprom{

private:

	static void csAssert();
	static void csDeassert();


public:	
	static void init();

	
};

};



#endif

