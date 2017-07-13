

#include "crc32.h"


uint32_t crc32Table[256];
bool crc32TableCreated = false;
void make_crc_table(void)
{
    uint32_t c;
    int32_t n, k;
    for (n = 0; n < 256; n++)
    {
        c = (uint32_t) n;
        for (k = 0; k < 8; k++)
        {
            if (c & 1)
            {
                c = 0xedb88320L ^ (c >> 1);
            }
            else
            {
                c = c >> 1;
            }
        }
        crc32Table[n] = c;
    }
    crc32TableCreated = true;
}


void crc32(uint32_t *pdwCrc32,uint8_t bNewByte)
{
    uint32_t c = (*pdwCrc32) ^ 0xffffffffL;
    if (crc32TableCreated==false)
	{
        make_crc_table();
	}
    c = crc32Table[(c ^ (uint32_t)bNewByte) & 0xff] ^ (c >> 8);
	(*pdwCrc32) = (c ^ 0xffffffffL);
}


