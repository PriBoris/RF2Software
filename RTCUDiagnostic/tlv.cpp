#include "tlv.h"

bool TLV::crcTableCreated = false;
quint32 TLV::crcTable_[256];



//========================================================================================================
void TLV::createCrcTable(void)
{
    quint32 c;
    quint32 n, k;
    for (n = 0; n < 256; n++)
    {
        c = (quint32) n;
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
        crcTable_[n] = c;
    }
    crcTableCreated = true;
}
//========================================================================================================
void TLV::crc32(quint32 *pdwCrc32,quint8 bNewByte)
{
    quint32 c = (*pdwCrc32) ^ 0xffffffffL;
    if (crcTableCreated==false)
    {
        createCrcTable();
    }
    c = crcTable_[(c ^ (quint32)bNewByte) & 0xff] ^ (c >> 8);
    (*pdwCrc32) = (c ^ 0xffffffffL);
}

