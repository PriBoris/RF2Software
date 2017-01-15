#include "WidgetServoMode.h"

#include <QDateTime>

#include "RTCU.h"


//=================================================================================================
WidgetServoMode::~WidgetServoMode(){
    delete reportLogger;
}
//=================================================================================================
WidgetServoMode::WidgetServoMode(
	SerialPortTransceiver *serialPortTransceiver,
	QWidget *parent) : QWidget(parent){


    serialPortTransceiver_ = serialPortTransceiver;

    reportLogger = new ReportLogger("ReportServoMode.txt");


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

    {
        lblServoFrequencyPositive = new QLabel("lblServoFrequencyPositive");
        lblServoFrequencyPositive->setFont(QFont("Verdana",10,QFont::Normal,false));

        lblServoFrequencyNegative = new QLabel("lblServoFrequencyPositive");
        lblServoFrequencyNegative->setFont(QFont("Verdana",10,QFont::Normal,false));
    }

    {
        lblServoCommand = new QLabel("");
        lblServoCommand->setFont(QFont("Verdana",10,QFont::Bold,false));
    }



    loMain = new QVBoxLayout;
    loMain->addWidget(lblRxMessageCounter);
    loMain->addWidget(lblMainTickID);
    loMain->addWidget(lblDateTime);
    loMain->addWidget(lblMode);
    loMain->addWidget(lblSubmode);
    loMain->addWidget(lblServoFrequencyPositive);
    loMain->addWidget(lblServoFrequencyNegative);
    loMain->addWidget(lblServoCommand);
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
            quint32 mainTickID = TLV::getUint32(value,0);
            QString mainTickIDStr = QString::number(mainTickID);
            lblMainTickID->setText("MainTickID: "+mainTickIDStr);
            (reportLogger->stream) << "tickID=" << QString::number(mainTickID) << ";";


        }

        {
            QString machineTimeStr = TLV::getDateTimeStr(value,4,true);
            lblDateTime->setText("Machine time: "+machineTimeStr);
        }


        {
			QDateTime dateTime(QDateTime::currentDateTime());
			QString pcTimeStr = dateTime.toString("(HH:mm:ss.zzz)");
			QString machineTimeStr = TLV::getDateTimeStr(value,4,false);
			(reportLogger->stream) << machineTimeStr << pcTimeStr << ";";
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

            lblMode->setText("Mode: "+modeStr);
			(reportLogger->stream) << modeStr << ";";
        }

        {
            quint8 submodeBefore = value.at(11);
            quint8 submodeAfter = value.at(13);

            QString submodeStr;
            if (submodeBefore == submodeAfter){
                submodeStr = RTCU::Submode::getTitle(submodeBefore);
            }else{
                submodeStr = RTCU::Submode::getTitle(submodeBefore)+" >>> "+RTCU::Submode::getTitle(submodeAfter);
            }

            lblSubmode->setText("Submode: "+submodeStr);
			(reportLogger->stream) << submodeStr << ";";
        }


        {
            float servoFrequencyPositive = TLV::getFloat(value,14);
            float servoFrequencyNegative = TLV::getFloat(value,18);

            QString servoFrequencyPositiveStr = QString::number(servoFrequencyPositive,'f',2)+"Hz";
            QString servoFrequencyNegativeStr = QString::number(servoFrequencyNegative,'f',2)+"Hz";

            lblServoFrequencyPositive->setText("Servo F(+): "+servoFrequencyPositiveStr);
            lblServoFrequencyNegative->setText("Servo F(-): "+servoFrequencyNegativeStr);
			(reportLogger->stream) << servoFrequencyPositiveStr << ";";
			(reportLogger->stream) << servoFrequencyNegativeStr << ";";
 
        }

        if (value.length()==22){
            lblServoCommand->setText("");
        }else if (value.length()==26){
            qint32 servoCommand = TLV::getInt32(value,22);
            QString servoCommandStr;
            switch(servoCommand){
            default:
                servoCommandStr = QString::number(servoCommand) + "?";
                break;
            case 0:
                servoCommandStr = "STOP";
                break;
            case 1:
                servoCommandStr = ">>>>";
                break;
            case -1:
                servoCommandStr = "<<<<";
                break;
            }
            lblServoCommand->setText("COMMAND:"+servoCommandStr);
            (reportLogger->stream) << "COMMAND:" << servoCommandStr <<";";
        }else{
            lblServoCommand->setText("");
        }


        (reportLogger->stream) << "\n";
        reportLogger->flush();
    }


}
//=================================================================================================


