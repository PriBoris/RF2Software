#include "tlvreader.h"


//========================================================================================================
TLVReader::TLVReader(QObject *parent)
{
    stuffState = STUFF_Waiting;
    protocolState = PROTOCOL_TagExpected;


}
//========================================================================================================
void TLVReader::processReadData(QByteArray &readData)
{
    static long readCount = 0;
    readCount += readData.length();

    //qDebug() << "TLVReader::processReadData" << QString::number(readCount);

    while(1)
    {
        quint8 newByte = readData.at(0);

        if (newByte==TLV::SLIP_END)
        {
            stuffState = STUFF_Waiting;
            protocolState = PROTOCOL_TagExpected;
        }
        else
        {
            switch(stuffState)
            {
                //------------------------------------------------
                default:
                    stuffState = STUFF_Waiting;
                //------------------------------------------------
                case STUFF_Waiting:
                    if (newByte==TLV::SLIP_ESC)
                    {
                        stuffState = STUFF_EscByte;
                    }
                    else
                    {
                        stuffState = STUFF_NewByte;
                    }
                    break;
                //------------------------------------------------
                case STUFF_EscByte:
                    if (newByte==TLV::SLIP_ESC_END)
                    {
                        stuffState = STUFF_NewByte;
                        newByte = TLV::SLIP_END;
                        //qDebug() << "TLVReader::SLIP_END";
                    }
                    else if (newByte==TLV::SLIP_ESC_ESC)
                    {
                        stuffState = STUFF_NewByte;
                        newByte = TLV::SLIP_ESC;
                        //qDebug() << "TLVReader::SLIP_ESC";
                    }
                    else
                    {
                        stuffState = STUFF_Waiting;
                        protocolState = PROTOCOL_TagExpected; //stuff error
                        //qDebug() << "TLVReader::STUFF_ERROR";
                    }
                    break;
                //------------------------------------------------
            }
        }

        if (stuffState==STUFF_NewByte)
        {
            stuffState = STUFF_Waiting;

            switch(protocolState)
            {
                //---protocolState---------------------------------------------------------------
                default:
                    protocolState = PROTOCOL_TagExpected;
                    break;
                //---protocolState---------------------------------------------------------------
                case PROTOCOL_TagExpected:
                    crcCalculated = 0;
                    TLV::crc32(&crcCalculated,newByte);
                    tag = newByte;
                    protocolState = PROTOCOL_IDByte1Expected;
                    msgID = 0;
                    break;
                //---protocolState---------------------------------------------------------------
                case PROTOCOL_IDByte1Expected:
                    TLV::crc32(&crcCalculated,newByte);
                    protocolState = PROTOCOL_IDByte2Expected;
                    msgID |= ((quint32)newByte)<<0;
                    break;
                //---protocolState---------------------------------------------------------------
                case PROTOCOL_IDByte2Expected:
                    TLV::crc32(&crcCalculated,newByte);
                    protocolState = PROTOCOL_IDByte3Expected;
                    msgID |= ((quint32)newByte)<<8;
                    break;
                //---protocolState---------------------------------------------------------------
                case PROTOCOL_IDByte3Expected:
                    TLV::crc32(&crcCalculated,newByte);
                    protocolState = PROTOCOL_IDByte4Expected;
                    msgID |= ((quint32)newByte)<<16;
                    break;
                //---protocolState---------------------------------------------------------------
                case PROTOCOL_IDByte4Expected:
                    TLV::crc32(&crcCalculated,newByte);
                    protocolState = PROTOCOL_LenByte1Expected;
                    msgID |= ((quint32)newByte)<<24;
                    break;
                //---protocolState---------------------------------------------------------------
                case PROTOCOL_LenByte1Expected:
                    TLV::crc32(&crcCalculated,newByte);
                    lengthExpected = (quint16)newByte;
                    protocolState = PROTOCOL_LenByte2Expected;
                    break;
                //---protocolState---------------------------------------------------------------
                case PROTOCOL_LenByte2Expected:
                    TLV::crc32(&crcCalculated,newByte);
                    lengthExpected |= ((quint16)newByte)<<8;
                    value.clear();
                    if (lengthExpected==0)
                    {
                        protocolState = PROTOCOL_CrcByte1Expected;
                    }
                    else
                    {
                        protocolState = PROTOCOL_ValueByteExpected;
                    }
                    break;
                //---protocolState---------------------------------------------------------------
                case PROTOCOL_ValueByteExpected:
                    TLV::crc32(&crcCalculated,newByte);
                    value.append(newByte);
                    if (value.length()==lengthExpected)
                    {
                        protocolState = PROTOCOL_CrcByte1Expected;
                    }
                    break;
                //---protocolState---------------------------------------------------------------
                case PROTOCOL_CrcByte1Expected:
                    crcExpected = (quint32)newByte;
                    protocolState = PROTOCOL_CrcByte2Expected;

                    break;
                //---protocolState---------------------------------------------------------------
                case PROTOCOL_CrcByte2Expected:
                        crcExpected |= ((quint32)newByte)<<8;
                            protocolState = PROTOCOL_CrcByte3Expected;
                    break;
                //---protocolState---------------------------------------------------------------
                case PROTOCOL_CrcByte3Expected:
                        crcExpected |= ((quint32)newByte)<<16;
                            protocolState = PROTOCOL_CrcByte4Expected;
                    break;
                //---protocolState---------------------------------------------------------------
                case PROTOCOL_CrcByte4Expected:
                        crcExpected |= ((quint32)newByte)<<24;
                            protocolState = PROTOCOL_TagExpected;

                        if (crcExpected==crcCalculated)
                        {
                            emit newMessageReceived(tag,msgID,value);
                            /*
                            qDebug()
                                    << "TLV reader: "
                                    << "length=" << QString::number(value.length()) << ", "
                                    << "tag=" << QString::number(tag) << ", "
                                    ;
                            */
                        }
                        else
                        {

                            qDebug() << "ERROR: crc fail" << lengthExpected << QString::number(crcExpected,16) << QString::number(crcCalculated,16);
                        }




                    break;
                //---protocolState---------------------------------------------------------------
            }


        }



        readData.remove(0,1);//TODO: use some other more efficient container
        if (readData.length()==0)
        {
            break;
        }

    }

}
//========================================================================================================
