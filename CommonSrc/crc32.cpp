

#include "crc32.h"


uint32_t Crc32::table[256];

//=================================================================================================
void Crc32::init(void){
	
    for (int32_t n = 0; n < 256; n++){
        uint32_t c = (uint32_t) n;
        for (uint32_t k = 0; k < 8; k++){
            if (c & 1){
                c = 0xedb88320L ^ (c >> 1);
            }else{
                c = c >> 1;
            }
        }
        table[n] = c;
    }
}
//=================================================================================================
void Crc32::appendByte(uint8_t newByte,uint32_t &crcValue){
    uint32_t c = crcValue ^ 0xffffffffL;
    c = table[(c ^ (uint32_t)newByte) & 0xff] ^ (c >> 8);
	crcValue = (c ^ 0xffffffffL);
}
//=================================================================================================
void Crc32::appendWord(uint16_t newWord,uint32_t &crcValue){
    uint8_t* p = (uint8_t*)&newWord;
    appendByte(*p++,crcValue);
    appendByte(*p++,crcValue);
}
//=================================================================================================
void Crc32::appendDword(uint32_t newDword,uint32_t &crcValue){
	uint8_t* p = (uint8_t*)&newDword;
	appendByte(*p++,crcValue);
	appendByte(*p++,crcValue);
	appendByte(*p++,crcValue);
	appendByte(*p++,crcValue);
}
//=================================================================================================
uint32_t Crc32::getInitValue(){
	return CRC32_SEED;
}
//=================================================================================================

