#ifndef TLVREADER_H
#define TLVREADER_H

#include <QObject>
#include "tlv.h"

class TLVReader : public QObject,TLV
{
    Q_OBJECT

    enum StuffState
    {
        STUFF_Waiting,
        STUFF_EscByte,
        STUFF_NewByte
    };


    enum ProtocolState
    {
        PROTOCOL_TagExpected,//TODO: insert stopper state
        PROTOCOL_IDByte1Expected,
        PROTOCOL_IDByte2Expected,
        PROTOCOL_IDByte3Expected,
        PROTOCOL_IDByte4Expected,
        PROTOCOL_LenByte1Expected,
        PROTOCOL_LenByte2Expected,
        PROTOCOL_ValueByteExpected,
        PROTOCOL_CrcByte1Expected,
        PROTOCOL_CrcByte2Expected,
        PROTOCOL_CrcByte3Expected,
        PROTOCOL_CrcByte4Expected,
    };

    StuffState stuffState;
    ProtocolState protocolState;

    quint8 tag;
    quint16 lengthExpected;
    quint32 crcExpected;
    quint32 crcCalculated;
    QByteArray value;
    quint32 msgID;




public:
    explicit TLVReader(QObject *parent = 0);

signals:

    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);


public slots:

    void processReadData(QByteArray &readData);

};

#endif // TLVREADER_H
