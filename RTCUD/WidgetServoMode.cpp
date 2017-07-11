#include "WidgetServoMode.h"

#include <QDateTime>

#include "Utils.h"


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
		lblRxMessageCounter = new QLabel;
		lblRxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));
		Utils::MessageCounterInitialize("Rx",rxMessageCounter,lblRxMessageCounter);
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
		lblTemperature = new QLabel("lblTemperature");
		lblTemperature->setFont(QFont("Verdana",10,QFont::Normal,false));
	}


	{
		lblServoCommand = new QLabel("lblServoCommand");
		lblServoCommand->setFont(QFont("Verdana",10,QFont::Bold,false));
	}

	{
		lblBytesCountHMI = new QLabel("lblBytesCountHMI");
		lblBytesCountNFC = new QLabel("lblBytesCountNFC");
		lblBytesCountHMI->setFont(QFont("Verdana",10,QFont::Normal,false));
		lblBytesCountNFC->setFont(QFont("Verdana",10,QFont::Normal,false));

	}


	{
		plotPositionVsTime = new QCustomPlot;
		plotPositionVsTime->addGraph();
		plotPositionVsTime->addGraph();
		plotPositionVsTime->addGraph();
		plotPositionVsTime->addGraph();
		plotPositionVsTime->addGraph();
		plotPositionVsTime->graph(0)->setPen(QPen(Qt::red));
		plotPositionVsTime->graph(1)->setPen(QPen(Qt::blue));
		plotPositionVsTime->graph(2)->setPen(QPen(Qt::blue));
		plotPositionVsTime->graph(3)->setPen(QPen(Qt::green));
		plotPositionVsTime->graph(4)->setPen(QPen(Qt::green));
		plotPositionVsTime->yAxis->setVisible(true);
		plotPositionVsTime->setFixedSize(1000,500);
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
	loMain->addWidget(lblTemperature);

	loMain->addWidget(lblServoCommand);

	loMain->addWidget(lblBytesCountHMI);
	loMain->addWidget(lblBytesCountNFC);



	loMain->addWidget(plotPositionVsTime);
	loMain->addStretch(1);

	this->setLayout(loMain);

	personalSettingsValid = false;
	machineSettingsExtendedValid = false;

}
//=================================================================================================
void WidgetServoMode::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){

	if (tag==TLV::TAG_ReportServoMode){

		Utils::MessageCounterIncrement("Rx",rxMessageCounter,lblRxMessageCounter,value);

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

			if (personalSettingsValid==true){
				plotPersonalAData.append(personalSettings.positionMainA);
				plotPersonalBData.append(personalSettings.positionMainB);
			}else{
				plotPersonalAData.append(position);
				plotPersonalBData.append(position);
			}

			if (machineSettingsExtendedValid==true){
				plotPositionMaxData.append(machineSettingsExtended.positionMainMax);
				plotPositionMinData.append(machineSettingsExtended.positionMainMin);
			}else{
				plotPositionMaxData.append(position);
				plotPositionMinData.append(position);
			}

			QPair<double,double> plotPositionRange;
			plotPositionRange.first = plotPositionRange.second = plotPositionData.at(0);
			updatePlotRange(plotPositionData,plotPositionRange);
			updatePlotRange(plotPersonalAData,plotPositionRange);
			updatePlotRange(plotPersonalBData,plotPositionRange);
			updatePlotRange(plotPositionMaxData,plotPositionRange);
			updatePlotRange(plotPositionMinData,plotPositionRange);

			QPair<double,double> plotTimeRange;
			plotTimeRange.first = plotTimeRange.second = plotTimeData.at(0);
			updatePlotRange(plotTimeData,plotTimeRange);

			plotPositionVsTime->xAxis->setRange(plotTimeRange.first , plotTimeRange.second);
			plotPositionVsTime->yAxis->setRange(plotPositionRange.first-100, plotPositionRange.second+100);
			plotPositionVsTime->graph(0)->setData(plotTimeData, plotPositionData);
			plotPositionVsTime->graph(1)->setData(plotTimeData, plotPersonalAData);
			plotPositionVsTime->graph(2)->setData(plotTimeData, plotPersonalBData);
			plotPositionVsTime->graph(3)->setData(plotTimeData, plotPositionMinData);
			plotPositionVsTime->graph(4)->setData(plotTimeData, plotPositionMaxData);
			plotPositionVsTime->replot();


			if (plotPositionData.length()>(10*PLOT_MAX_DURATION_SECONDS)){
				plotPositionData.pop_front();
				plotPersonalAData.pop_front();
				plotPersonalBData.pop_front();
				plotPositionMinData.pop_front();
				plotPositionMaxData.pop_front();
				plotTimeData.pop_front();
			}

		}

		{
			qint32 heatsinkTemperature = (double)TLV::getInt32(value,26);
			qint32 internalTemperature = (double)TLV::getInt32(value,30);
			qint32 motorTemperature = (double)TLV::getInt32(value,34);

			QString temperatureStr = 
				QString::number(heatsinkTemperature)+
				" / "+
				QString::number(internalTemperature)+
				" / "+
				QString::number(motorTemperature)+
                " oC"
				;
			lblTemperature->setText("Temperature: "+temperatureStr);
			(reportLogger->stream) << temperatureStr << ";";

		}


		{
			qint32 servoCommand = TLV::getInt32(value,38);
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
		}

		{
			quint32 bytesCountToHMI = TLV::getUint32(value,42);;
			quint32 bytesCountFromHMI = TLV::getUint32(value,46);;
			quint32 bytesCountToNFC = TLV::getUint32(value,50);;
			quint32 bytesCountFromNFC = TLV::getUint32(value,54);;

			QString bytesCountHMIStr = 
				" >" + QString::number(bytesCountToHMI) +
				" <" + QString::number(bytesCountFromHMI) +
				"";
			QString bytesCountNFCStr = 
				" >" + QString::number(bytesCountToNFC) +
				" <" + QString::number(bytesCountFromNFC) +
				"";

			lblBytesCountHMI->setText("HMI bytes count:" + bytesCountHMIStr);
			lblBytesCountNFC->setText("NFC bytes count:" + bytesCountNFCStr);
			(reportLogger->stream) << "HMI:" << bytesCountHMIStr <<";";
			(reportLogger->stream) << "NFC:" << bytesCountNFCStr <<";";


		}


		(reportLogger->stream) << "\n";
        reportLogger->flush(false);
	}

	//--------------------------------------------------------------------------
	if (tag==TLV::TAG_ReportPersonalSettings){

		if (value.length()==sizeof(RTCU::TPersonalSettings)){

			memcpy(&personalSettings,value.data(),sizeof(RTCU::TPersonalSettings));
			personalSettingsValid = true;

		}else{

		   personalSettingsValid = false;

		}

	}
	//--------------------------------------------------------------------------
	if (tag==TLV::TAG_ReportMachineSettingsExtended){

		if (value.length()==sizeof(RTCU::TMachineSettingsExtended)){

			memcpy(&machineSettingsExtended,value.data(),sizeof(RTCU::TMachineSettingsExtended));
			machineSettingsExtendedValid = true;

		}else{

		   machineSettingsExtendedValid = false;

		}

	}
	//--------------------------------------------------------------------------


}

//=================================================================================================
void WidgetServoMode::updatePlotRange(
	QVector<double> &plotData, 
	QPair<double,double> &range){

	QVectorIterator<double> it(plotData);  
	while(it.hasNext()){
		double nextValue = it.next();
		if (nextValue>range.second){
			range.second = nextValue;
		}
		if (nextValue<range.first){
			range.first = nextValue;
		}
	}
}
//=================================================================================================

