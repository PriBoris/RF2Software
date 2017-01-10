#include "WidgetServoMode.h"


#include "RTCU.h"


//=================================================================================================
WidgetServoMode::~WidgetServoMode(){
}
//=================================================================================================
WidgetServoMode::WidgetServoMode(
	SerialPortTransceiver *serialPortTransceiver,
	QWidget *parent) : QWidget(parent){


    serialPortTransceiver_ = serialPortTransceiver;

    {
        rxMessageCounter = 0;
        lblRxMessageCounter = new QLabel("lblRxMessageCounter");
        lblRxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));
    }

    {
        lblMainTickID = new QLabel("lblMainTickID");
        lblMainTickID->setFont(QFont("Verdana",10,QFont::Normal,false));
    }


    {
        lblDateTime = new QLabel("lblDateTime");
        lblDateTime->setFont(QFont("Verdana",10,QFont::Normal,false));
    }

    {
        lblMode = new QLabel("lblMode");
        lblMode->setFont(QFont("Verdana",10,QFont::Normal,false));
    }

    {
        lblSubmode = new QLabel("lblSubmode");
        lblSubmode->setFont(QFont("Verdana",10,QFont::Normal,false));
    }



    loMain = new QVBoxLayout;
    loMain->addWidget(lblRxMessageCounter);
    loMain->addWidget(lblMainTickID);
    loMain->addWidget(lblDateTime);
    loMain->addWidget(lblMode);
    loMain->addWidget(lblSubmode);
    loMain->addStretch(1);

    this->setLayout(loMain);


}
//=================================================================================================
void WidgetServoMode::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){

    if (tag==TLV::TAG_ReportServoMode){

        {
            rxMessageCounter++;
            lblRxMessageCounter->setText(
                        "Сообщений: "+QString::number(rxMessageCounter)+" "
                        +"(длина последнего="
                        +QString::number(value.length())
                        +")"
                        );
        }

        {
            quint8 modeBefore = value.at(10);
            quint8 modeAfter = value.at(12);

            QString modeStr;
            if (modeBefore == modeAfter){
                modeStr = RTCU::Mode::getTitle(modeBefore);
            }else{
                modeStr = RTCU::Mode::getTitle(modeBefore)+" >>> "+RTCU::Mode::getTitle(modeAfter);
            }

            lblMode->setText("Режим: "+modeStr);


        }

    }


}
//=================================================================================================


