#include "WidgetGenericSetSettings.h"

WidgetGenericSetSettings::WidgetGenericSetSettings(
	SerialPortTransceiver *serialPortTransceiver,
	QWidget *parent
	) : QWidget(parent){

    serialPortTransceiver_ = serialPortTransceiver;

    {
        rxMessageCounter = 0;
        lblRxMessageCounter = new QLabel("lblRxMessageCounter");
        lblRxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));
    }





    loMain = new QVBoxLayout;

    loMain->addWidget(lblRxMessageCounter);
    loMain->addSpacing(10);

    loMain->addStretch(1);

    this->setLayout(loMain);


}

void WidgetGenericSetSettings::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){

    if (tag==TLV::TAG_ReportGenericSetSettings){
        
        rxMessageCounter++;
        lblRxMessageCounter->setText(
                    "Сообщений: "+QString::number(rxMessageCounter)+" "
                    +"(длина последнего = "
                    +QString::number(value.length())
                    +" байт)"
                    );


    }

}