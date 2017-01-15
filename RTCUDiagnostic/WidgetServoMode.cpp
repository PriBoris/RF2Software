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
        lblPosition = new QLabel("lblPosition");
        lblPosition->setFont(QFont("Verdana",10,QFont::Normal,false));
    }

    {
        lblServoCommand = new QLabel("");
        lblServoCommand->setFont(QFont("Verdana",10,QFont::Bold,false));
    }

    {
        plotPositionVsTime = new QCustomPlot;
        plotPositionVsTime->addGraph();
        plotPositionVsTime->graph(0)->setPen(QPen(Qt::red));
        plotPositionVsTime->yAxis->setVisible(true);
        plotPositionVsTime->setFixedSize(900,300);
        plotPositionVsTime->xAxis->setRange(0, 1);
        plotPositionVsTime->yAxis->setLabel("position []");
        plotPositionVsTime->xAxis->setLabel("time [s]");

        plotTime = 0.0;
    }

    loMain = new QVBoxLayout;
    loMain->addWidget(lblRxMessageCounter);
    loMain->addWidget(lblMainTickID);
    loMain->addWidget(lblDateTime);
    loMain->addWidget(lblMode);
    loMain->addWidget(lblSubmode);
    loMain->addWidget(lblServoFrequencyPositive);
    loMain->addWidget(lblServoFrequencyNegative);
    loMain->addWidget(lblPosition);
    loMain->addWidget(lblServoCommand);
    loMain->addWidget(plotPositionVsTime);
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

        {
            qint32 position = TLV::getInt32(value,22);
            QString positionStr = QString::number(position);
            lblPosition->setText("Position: "+positionStr);
            (reportLogger->stream) << positionStr << ";";
        }

        {
            double position = (double)TLV::getInt32(value,22);
            plotPositionData.append(position);
            plotTime += 0.1;
            plotTimeData.append(plotTime);

            double maxPosition = getMaxPosition(plotPositionData,1000.0)+1.0;
            double minPosition = getMinPosition(plotPositionData,0.0)-1.0;
            double maxTime = getMaxPosition(plotTimeData,1.0);
            double minTime = getMinPosition(plotTimeData,0.0);

            plotPositionVsTime->xAxis->setRange(minTime, maxTime);
            plotPositionVsTime->yAxis->setRange(minPosition, maxPosition);
            plotPositionVsTime->graph(0)->setData(plotTimeData, plotPositionData);
            plotPositionVsTime->replot();

            if (plotPositionData.length()>(10*60*2)){
                plotPositionData.pop_front();
                plotTimeData.pop_front();
            }

        }


        if (value.length()==26){
            lblServoCommand->setText("");
        }else if (value.length()==30){
            qint32 servoCommand = TLV::getInt32(value,26);
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
double WidgetServoMode::getMaxPosition(QVector<double> &positionData, double defaultPosition){
    if (positionData.length()==0){
        return defaultPosition;
    }else{
        double result = positionData.at(0);
        for(int i=1;i<positionData.length();i++){
            if (positionData.at(i)>result){
                result = positionData.at(i);
            }
        }
        return result;
   }

}
//=================================================================================================
double WidgetServoMode::getMinPosition(QVector<double> &positionData, double defaultPosition){
    if (positionData.length()==0){
        return defaultPosition;
    }else{
        double result = positionData.at(0);
        for(int i=1;i<positionData.length();i++){
            if (positionData.at(i)<result){
                result = positionData.at(i);
            }
        }
        return result;
    }

}
//=================================================================================================
 

