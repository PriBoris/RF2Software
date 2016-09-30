#include "widgetnfc.h"


WidgetNFC::WidgetNFC(
      SerialPortTransceiver *serialPortTransceiver,
      QString name,
      bool bbWidget,
      QWidget *parent
      ) : Widget(parent)
{
    serialPortTransceiver_ = serialPortTransceiver;
    name_ = name;
    bbWidget_ = bbWidget;
    rxMessageCounter = 0;


    lblRxMessageCounter = new QLabel("lblRxMessageCounter");
    lblUID = new QLabel("lblUID");


    loMain = new QVBoxLayout;
    loMain->addWidget(lblRxMessageCounter);
    loMain->addWidget(lblUID);
    loMain->addStretch(1);

    this->setLayout(loMain);



}

void WidgetNFC::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value)
{
    if (tag==TLV::TAG_RfidProximity)
    {
        rxMessageCounter++;

        lblRxMessageCounter->setText(
                    "Сообщений: "+QString::number(rxMessageCounter)+" "
                    +"(длина последнего="
                    +QString::number(value.length())
                    +")"
                    );


        qint32 uidLength = 0;
        memcpy(&uidLength,(qint32*)(value.data()),sizeof(uidLength));

        QByteArray uid = value.mid(4,uidLength);
        lblUID-> setText("UID: "+uid.toHex()+" (len="+QString::number(uidLength)+")");
   }



}
//============================================================================================================
