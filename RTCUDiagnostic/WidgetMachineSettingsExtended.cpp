#include "WidgetMachineSettingsExtended.h"

#include <QDateTime>

#include "Utils.h"


//=================================================================================================
WidgetMachineSettingsExtended::~WidgetMachineSettingsExtended(){
	delete reportLogger;
}
//=================================================================================================
WidgetMachineSettingsExtended::WidgetMachineSettingsExtended(
		SerialPortTransceiver *serialPortTransceiver,
		QWidget *parent
		) : QWidget(parent){

	serialPortTransceiver_ = serialPortTransceiver;

	reportLogger = new ReportLogger("ReportMachineSettingsExtended.txt");

	{

		lblRxMessageCounter = new QLabel;
		lblTxMessageCounter = new QLabel;

		lblRxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));
		lblTxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));

		Utils::MessageCounterInitialize("Rx",rxMessageCounter,lblRxMessageCounter);
		Utils::MessageCounterInitialize("Tx",txMessageCounter,lblTxMessageCounter);

	}


	{
		wgtPositionMainMax = new WidgetSettingsInteger("positionMainMax","MachineSettings_PositionMainMax",99);
		wgtPositionMainMin = new WidgetSettingsInteger("positionMainMin","MachineSettings_PositionMainMin",0);
		wgtPositionAux1Max = new WidgetSettingsInteger("positionAux1Max","MachineSettings_PositionAux1Max",99);
		wgtPositionAux1Min = new WidgetSettingsInteger("positionAux1Min","MachineSettings_PositionAux1Min",0);
		wgtPositionAux2Max = new WidgetSettingsInteger("positionAux2Max","MachineSettings_PositionAux2Max",99);
		wgtPositionAux2Min = new WidgetSettingsInteger("positionAux2Min","MachineSettings_PositionAux2Min",0);
		wgtPositionAux3Max = new WidgetSettingsInteger("positionAux3Max","MachineSettings_PositionAux3Max",99);
		wgtPositionAux3Min = new WidgetSettingsInteger("positionAux3Min","MachineSettings_PositionAux3Min",0);
		wgtSpeedAbsMainMax = new WidgetSettingsInteger("speedAbsMainMax","MachineSettings_SpeedAbsMainMax",1000);
		wgtSpeedAbsMainPersonal = new WidgetSettingsInteger("speedAbsMainPersonal","MachineSettings_SpeedAbsMainPersonal",50);

		wgtEncoderBitCount = new WidgetSettingsInteger("encoderBitCount","MachineSettingsExtended_encoderBitCount",15);
		wgtEncoderDirection = new WidgetSettingsInteger("encoderDirection","MachineSettingsExtended_encoderDirection",1);
		wgtEncoderOffset = new WidgetSettingsInteger("encoderOffset","MachineSettingsExtended_encoderOffset",10000);
		wgtForceSensorOffset = new WidgetSettingsInteger("forceSensorOffset","MachineSettingsExtended_forceSensorOffset",20000);
		wgtForceSensorGain = new WidgetSettingsFloat("forceSensorGain","MachineSettingsExtended_forceSensorGain",4000.0f);



		wgtEncoderOffset->setValidator(new QIntValidator(-1000000,1000000));
		wgtEncoderDirection->setValidator(new QIntValidator(-1,1));
		wgtEncoderBitCount->setValidator(new QIntValidator(10,16));
	}

	{
		btnWriteSettings = new QPushButton("Send Extended Machine Settings");
		btnWriteSettings->setFixedWidth(600);
		btnWriteSettings->setFont(QFont("Verdana",10,QFont::Bold,true));
	}


	{
		loMain = new QVBoxLayout;
		loMain->addWidget(lblRxMessageCounter);
		loMain->addWidget(lblTxMessageCounter);
		   loMain->addSpacing(10);

		loMain->addWidget(wgtPositionMainMin);
		loMain->addWidget(wgtPositionMainMax);
			loMain->addSpacing(10);
		loMain->addWidget(wgtPositionAux1Min);
		loMain->addWidget(wgtPositionAux1Max);
			loMain->addSpacing(10);
		loMain->addWidget(wgtPositionAux2Min);
		loMain->addWidget(wgtPositionAux2Max);
			loMain->addSpacing(10);
		loMain->addWidget(wgtPositionAux3Min);
		loMain->addWidget(wgtPositionAux3Max);
		loMain->addSpacing(10);
		loMain->addWidget(wgtSpeedAbsMainMax);
		loMain->addWidget(wgtSpeedAbsMainPersonal);
		loMain->addSpacing(10);
		loMain->addWidget(wgtEncoderBitCount);
		loMain->addWidget(wgtEncoderDirection);
		loMain->addWidget(wgtEncoderOffset);
			loMain->addSpacing(10);
		loMain->addWidget(wgtForceSensorOffset);
		loMain->addWidget(wgtForceSensorGain);
			loMain->addSpacing(10);
		loMain->addWidget(btnWriteSettings);
			loMain->addSpacing(10);


		loMain->addWidget(btnWriteSettings);

		loMain->addStretch(1);
		this->setLayout(loMain);
	}



	connect(btnWriteSettings,SIGNAL(clicked(bool)),SLOT(slotWriteSettings()));

}
//=================================================================================================
void WidgetMachineSettingsExtended::slotWriteSettings(){

	if (serialPortTransceiver_->isPortOK()==true){


		TMachineSettingsExtended newMachineSettingsExtended;

		newMachineSettingsExtended.positionMainMax = wgtPositionMainMax->getWriteValue();
		newMachineSettingsExtended.positionMainMin = wgtPositionMainMin->getWriteValue();
		newMachineSettingsExtended.positionAux1Max = wgtPositionAux1Max->getWriteValue();
		newMachineSettingsExtended.positionAux1Min = wgtPositionAux1Min->getWriteValue();
		newMachineSettingsExtended.positionAux2Max = wgtPositionAux2Max->getWriteValue();
		newMachineSettingsExtended.positionAux2Min = wgtPositionAux2Min->getWriteValue();
		newMachineSettingsExtended.positionAux3Max = wgtPositionAux3Max->getWriteValue();
		newMachineSettingsExtended.positionAux3Min = wgtPositionAux3Min->getWriteValue();
		newMachineSettingsExtended.speedAbsMainMax = wgtSpeedAbsMainMax->getWriteValue();
		newMachineSettingsExtended.speedAbsMainPersonal = wgtSpeedAbsMainPersonal->getWriteValue();

		newMachineSettingsExtended.encoderBitCount = wgtEncoderBitCount->getWriteValue();
		newMachineSettingsExtended.encoderDirection = wgtEncoderDirection->getWriteValue();
		newMachineSettingsExtended.encoderOffset = wgtEncoderOffset->getWriteValue();
		newMachineSettingsExtended.forceSensorOffset = wgtForceSensorOffset->getWriteValue();
		newMachineSettingsExtended.forceSensorGain = wgtForceSensorGain->getWriteValue();


		QByteArray valueArray;
		valueArray.append((char*)&newMachineSettingsExtended,sizeof(newMachineSettingsExtended));
		TLVWriter tlv(TLV::TAG_LoadMachineSettingsExtended,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;

		Utils::MessageCounterIncrement("Tx",txMessageCounter,lblTxMessageCounter,valueArray);
		
	}

}
//=================================================================================================
void WidgetMachineSettingsExtended::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){

	if (tag==TLV::TAG_ReportMachineSettingsExtended){

		Utils::MessageCounterIncrement("Rx",rxMessageCounter,lblRxMessageCounter,value);

		{
			QDateTime dateTime(QDateTime::currentDateTime());
			QString pcTimeStr = dateTime.toString("yyyy-MM-dd HH:mm:ss");
			(reportLogger->stream) << pcTimeStr << "\n";
		}


		if (value.length()==sizeof(TMachineSettingsExtended)){

			TMachineSettingsExtended machineSettingsExtended;
			memcpy(&machineSettingsExtended,value.data(),sizeof(TMachineSettingsExtended));


			wgtPositionMainMax->setReadValue(machineSettingsExtended.positionMainMax);
			wgtPositionMainMin->setReadValue(machineSettingsExtended.positionMainMin);
			wgtPositionAux1Max->setReadValue(machineSettingsExtended.positionAux1Max);
			wgtPositionAux1Min->setReadValue(machineSettingsExtended.positionAux1Min);
			wgtPositionAux2Max->setReadValue(machineSettingsExtended.positionAux2Max);
			wgtPositionAux2Min->setReadValue(machineSettingsExtended.positionAux2Min);
			wgtPositionAux3Max->setReadValue(machineSettingsExtended.positionAux3Max);
			wgtPositionAux3Min->setReadValue(machineSettingsExtended.positionAux3Min);
			wgtSpeedAbsMainMax->setReadValue(machineSettingsExtended.speedAbsMainMax);
			wgtSpeedAbsMainPersonal->setReadValue(machineSettingsExtended.speedAbsMainPersonal);

			wgtEncoderBitCount->setReadValue(machineSettingsExtended.encoderBitCount);
			wgtEncoderDirection->setReadValue(machineSettingsExtended.encoderDirection);
			wgtEncoderOffset->setReadValue(machineSettingsExtended.encoderOffset);
			wgtForceSensorOffset->setReadValue(machineSettingsExtended.forceSensorOffset);
			wgtForceSensorGain->setReadValue(machineSettingsExtended.forceSensorGain);



			(reportLogger->stream) << "positionMainMax=" << QString::number(machineSettingsExtended.positionMainMax) << "\n";
			(reportLogger->stream) << "positionMainMin=" << QString::number(machineSettingsExtended.positionMainMin) << "\n";
			(reportLogger->stream) << "positionAux1Max=" << QString::number(machineSettingsExtended.positionAux1Max) << "\n";
			(reportLogger->stream) << "positionAux1Min=" << QString::number(machineSettingsExtended.positionAux1Min) << "\n";
			(reportLogger->stream) << "positionAux2Max=" << QString::number(machineSettingsExtended.positionAux2Max) << "\n";
			(reportLogger->stream) << "positionAux2Min=" << QString::number(machineSettingsExtended.positionAux2Min) << "\n";
			(reportLogger->stream) << "positionAux3Max=" << QString::number(machineSettingsExtended.positionAux3Max) << "\n";
			(reportLogger->stream) << "positionAux3Min=" << QString::number(machineSettingsExtended.positionAux3Min) << "\n";
			(reportLogger->stream) << "speedAbsMainMax=" << QString::number(machineSettingsExtended.speedAbsMainMax) << "\n";
			(reportLogger->stream) << "speedAbsMainPersonal=" << QString::number(machineSettingsExtended.speedAbsMainPersonal) << "\n";
			(reportLogger->stream) << "encoderBitCount=" << QString::number(machineSettingsExtended.encoderBitCount) << "\n";
			(reportLogger->stream) << "encoderDirection=" << QString::number(machineSettingsExtended.encoderDirection) << "\n";
			(reportLogger->stream) << "encoderOffset=" << QString::number(machineSettingsExtended.encoderOffset) << "\n";
			(reportLogger->stream) << "forceSensorOffset=" << QString::number(machineSettingsExtended.forceSensorOffset) << "\n";
			(reportLogger->stream) << "forceSensorGain=" << QString::number(machineSettingsExtended.forceSensorGain) << "\n";



		}else{

			wgtPositionMainMax->setUnknownReadValue();
			wgtPositionMainMin->setUnknownReadValue();
			wgtPositionAux1Max->setUnknownReadValue();
			wgtPositionAux1Min->setUnknownReadValue();
			wgtPositionAux2Max->setUnknownReadValue();
			wgtPositionAux2Min->setUnknownReadValue();
			wgtPositionAux3Max->setUnknownReadValue();
			wgtPositionAux3Min->setUnknownReadValue();
			wgtSpeedAbsMainMax->setUnknownReadValue();
			wgtSpeedAbsMainPersonal->setUnknownReadValue();
			wgtEncoderBitCount->setUnknownReadValue();
			wgtEncoderDirection->setUnknownReadValue();
			wgtEncoderOffset->setUnknownReadValue();
			wgtForceSensorOffset->setUnknownReadValue();
			wgtForceSensorGain->setUnknownReadValue();

		}

		(reportLogger->stream) << "\n";
		reportLogger->flush();


	}
}
//=================================================================================================
