#include "tlvwriter.h"

quint32 TLVWriter::txID = 0;

TLVWriter::TLVWriter(
    const quint8 tag,
    QByteArray &value,
    QObject *parent
    ) : QObject(parent){



    tagLenValueCrc_.append(tag);

    tagLenValueCrc_.append((char*)&txID,sizeof(txID));
    txID++;

    quint16 dataLen = (quint16)value.length();
    tagLenValueCrc_.append((char*)&dataLen,sizeof(dataLen));

    tagLenValueCrc_.append(value);

    quint32 crc = 0;
    for(int i=0;i<tagLenValueCrc_.length();i++){
        TLV::crc32(&crc,tagLenValueCrc_.at(i));
    }
    tagLenValueCrc_.append((char*)&crc,4);



    tagLenValueCrcStuffed_.append(SLIP_END);
    tagLenValueCrcStuffed_.append(SLIP_END);
    tagLenValueCrcStuffed_.append(SLIP_END);
    tagLenValueCrcStuffed_.append(SLIP_END);
    for(int i=0;i<tagLenValueCrc_.length();i++){

        if (tagLenValueCrc_.at(i)==(char)SLIP_END){
            tagLenValueCrcStuffed_.append(SLIP_ESC);
            tagLenValueCrcStuffed_.append(SLIP_ESC_END);
        }else if (tagLenValueCrc_.at(i)==(char)SLIP_ESC){
            tagLenValueCrcStuffed_.append(SLIP_ESC);
            tagLenValueCrcStuffed_.append(SLIP_ESC_ESC);
        }else{
            tagLenValueCrcStuffed_.append(tagLenValueCrc_.at(i));
        }
    }

    {
        reportLogger = new ReportLogger("TxMessages.txt");
        (reportLogger->stream) 
            << TLV::getTitle(tag) 
            << " "
            << QString::number((int)value.length()) 
            << "\n"
            << "raw:     " << QString::number(tagLenValueCrc_.length()) << "bytes " << tagLenValueCrc_.toHex() << "\n"
            << "stuffed: " << QString::number(tagLenValueCrcStuffed_.length()) << "bytes " << tagLenValueCrcStuffed_.toHex() << "\n"
            ;

        reportLogger->flush(true);
        delete reportLogger;
    }
}

//========================================================================================================
QByteArray* TLVWriter::getStuffedArray(){
    return new QByteArray(tagLenValueCrcStuffed_);
}
//========================================================================================================
