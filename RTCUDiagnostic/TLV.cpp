#include "TLV.h"

bool TLV::crcTableCreated = false;
quint32 TLV::crcTable_[256];



//========================================================================================================
void TLV::createCrcTable(void){
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
void TLV::crc32(quint32 *pdwCrc32,quint8 bNewByte){
    quint32 c = (*pdwCrc32) ^ 0xffffffffL;
    if (crcTableCreated==false)
    {
        createCrcTable();
    }
    c = crcTable_[(c ^ (quint32)bNewByte) & 0xff] ^ (c >> 8);
    (*pdwCrc32) = (c ^ 0xffffffffL);
}
//========================================================================================================
qint32 TLV::getInt32(QByteArray &array,int offset){
    qint32 result;
    memcpy(&result,&array.data()[offset],sizeof(result));
    return result;
}
//========================================================================================================
quint32 TLV::getUint32(QByteArray &array,int offset){
    quint32 result;
    memcpy(&result,&array.data()[offset],sizeof(result));
    return result;
}
//========================================================================================================
float TLV::getFloat(QByteArray &array,int offset){
    float result;
    memcpy(&result,&array.data()[offset],sizeof(result));
    return result;
}
//========================================================================================================
QString TLV::getDateTimeStr(QByteArray &array,int offset,bool showDate){


    QString timeStr = 
        QString("%1").arg(array.at(offset+3),2,10,QChar('0'))+
        ":"+
        QString("%1").arg(array.at(offset+4),2,10,QChar('0'))+
        ":"+
        QString("%1").arg(array.at(offset+5),2,10,QChar('0'))
        ;
    if (showDate = true){
        QString dateStr = 
            QString("%1").arg(2000+(quint32)array.at(offset+0),4,10,QChar('0'))+
            "-"+
            QString("%1").arg(array.at(offset+1),2,10,QChar('0'))+
            "-"+
            QString("%1").arg(array.at(offset+2),2,10,QChar('0'));
        return dateStr+" "+timeStr;
    }else{
        return timeStr;
    }

}
//========================================================================================================

