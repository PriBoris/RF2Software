


#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>



class Crc32{
private:

	static const uint32_t CRC32_SEED = 0x00000000;
	static uint32_t table[256];

public:
	static void init();
	static void appendByte(uint8_t newByte,uint32_t &crcValue);
	static void appendDword(uint32_t newDword,uint32_t &crcValue);
	static uint32_t getInitValue();
	
};



#endif // CRC32_H

