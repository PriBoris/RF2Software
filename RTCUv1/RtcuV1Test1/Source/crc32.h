


#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>

extern void crc32(uint32_t *pdwCrc32,uint8_t bNewByte);
const uint32_t CRC32_SEED = 0x00000000;


#endif // CRC32_H

