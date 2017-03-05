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

		wgtForceSensorGain = new WidgetSettingsFloat("forceSensorGain","MachineSettingsExtended_forceSensorGain",0.000244141f);
		wgtForceSensorOffset0 = new WidgetSettingsFloat("forceSensorOffset0","MachineSettingsExtended_forceSensorOffset0",0.0f);
		wgtForceSensorOffset1 = new WidgetSettingsFloat("forceSensorOffset1","MachineSettingsExtended_forceSensorOffset1",0.0f);
		wgtForceSensorOffset2 = new WidgetSettingsFloat("forceSensorOffset2","MachineSettingsExtended_forceSensorOffset2",0.0f);

		wgtMinAbsServoFrequency = new WidgetSettingsFloat("minAbsServoFrequency","MachineSettingsExtended_minAbsServoFrequency",2.0f);
		wgtMaxAbsServoFrequency = new WidgetSettingsFloat("maxAbsServoFrequency","MachineSettingsExtended_maxAbsServoFrequency",30.0f);
		wgtConcentricAccelerationLaw = new WidgetSettingsInteger("concentricAccelerationLaw","MachineSettingsExtended_concentricAccelerationLaw",2.0);
		wgtConcentricDecelerationLaw = new WidgetSettingsInteger("concentricDecelerationLaw","MachineSettingsExtended_concentricDecelerationLaw",2.0);
		wgtEccentricAccelerationLaw = new WidgetSettingsInteger("eccentricAccelerationLaw","MachineSettingsExtended_eccentricAccelerationLaw",2.0);
		wgtEccentricDecelerationLaw = new WidgetSettingsInteger("eccentricDecelerationLaw","MachineSettingsExtended_eccentricDecelerationLaw",2.0);

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
		loMain->addWidget(wgtForceSensorGain);
		loMain->addWidget(wgtForceSensorOffset0);
		loMain->addWidget(wgtForceSensorOffset1);
		loMain->addWidget(wgtForceSensorOffset2);
			loMain->addSpacing(10);

		loMain->addWidget(wgtMinAbsServoFrequency);
		loMain->addWidget(wgtMaxAbsServoFrequency);
			loMain->addSpacing(10);
		loMain->addWidget(wgtConcentricAccelerationLaw);
		loMain->addWidget(wgtConcentricDecelerationLaw);
		loMain->addWidget(wgtEccentricAccelerationLaw);
		loMain->addWidget(wgtEccentricDecelerationLaw);
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


        RTCU::TMachineSettingsExtended newMachineSettingsExtended;

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

		newMachineSettingsExtended.forceSensorGain = wgtForceSensorGain->getWriteValue();
		newMachineSettingsExtended.forceSensorOffset0 = wgtForceSensorOffset0->getWriteValue();
		newMachineSettingsExtended.forceSensorOffset1 = wgtForceSensorOffset1->getWriteValue();
		newMachineSettingsExtended.forceSensorOffset2 = wgtForceSensorOffset2->getWriteValue();

		newMachineSettingsExtended.minAbsServoFrequency = wgtMinAbsServoFrequency->getWriteValue();
		newMachineSettingsExtended.maxAbsServoFrequency = wgtMaxAbsServoFrequency->getWriteValue();
		newMachineSettingsExtended.concentricAccelerationLaw = wgtConcentricAccelerationLaw->getWriteValue();
		newMachineSettingsExtended.concentricDecelerationLaw = wgtConcentricDecelerationLaw->getWriteValue();
		newMachineSettingsExtended.eccentricAccelerationLaw = wgtEccentricAccelerationLaw->getWriteValue();
		newMachineSettingsExtended.eccentricDecelerationLaw = wgtEccentricDecelerationLaw->getWriteValue();

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


        if (value.length()==sizeof(RTCU::TMachineSettingsExtended)){

            RTCU::TMachineSettingsExtended machineSettingsExtended;
            memcpy(&machineSettingsExtended,value.data(),sizeof(RTCU::TMachineSettingsExtended));


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

			wgtForceSensorGain->setReadValue(machineSettingsExtended.forceSensorGain);
			wgtForceSensorOffset0->setReadValue(machineSettingsExtended.forceSensorOffset0);
			wgtForceSensorOffset1->setReadValue(machineSettingsExtended.forceSensorOffset1);
			wgtForceSensorOffset2->setReadValue(machineSettingsExtended.forceSensorOffset2);

			wgtMinAbsServoFrequency->setReadValue(machineSettingsExtended.minAbsServoFrequency);
			wgtMaxAbsServoFrequency->setReadValue(machineSettingsExtended.maxAbsServoFrequency);
			wgtConcentricAccelerationLaw->setReadValue(machineSettingsExtended.concentricAccelerationLaw);
			wgtConcentricDecelerationLaw->setReadValue(machineSettingsExtended.concentricDecelerationLaw);
			wgtEccentricAccelerationLaw->setReadValue(machineSettingsExtended.eccentricAccelerationLaw);
			wgtEccentricDecelerationLaw->setReadValue(machineSettingsExtended.eccentricDecelerationLaw);

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
			(reportLogger->stream) << "forceSensorGain=" << QString::number(machineSettingsExtended.forceSensorGain) << "\n";
			(reportLogger->stream) << "forceSensorOffset0=" << QString::number(machineSettingsExtended.forceSensorOffset0) << "\n";
			(reportLogger->stream) << "forceSensorOffset1=" << QString::number(machineSettingsExtended.forceSensorOffset1) << "\n";
			(reportLogger->stream) << "forceSensorOffset2=" << QString::number(machineSettingsExtended.forceSensorOffset2) << "\n";
			(reportLogger->stream) << "minAbsServoFrequency=" << QString::number(machineSettingsExtended.minAbsServoFrequency) << "\n";
			(reportLogger->stream) << "maxAbsServoFrequency=" << QString::number(machineSettingsExtended.maxAbsServoFrequency) << "\n";
			(reportLogger->stream) << "concentricAccelerationLaw=" << QString::number(machineSettingsExtended.concentricAccelerationLaw) << "\n";
			(reportLogger->stream) << "concentricDecelerationLaw=" << QString::number(machineSettingsExtended.concentricDecelerationLaw) << "\n";
			(reportLogger->stream) << "eccentricAccelerationLaw=" << QString::number(machineSettingsExtended.eccentricAccelerationLaw) << "\n";
			(reportLogger->stream) << "eccentricDecelerationLaw=" << QString::number(machineSettingsExtended.eccentricDecelerationLaw) << "\n";



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

			wgtForceSensorGain->setUnknownReadValue();
			wgtForceSensorOffset0->setUnknownReadValue();
			wgtForceSensorOffset1->setUnknownReadValue();
			wgtForceSensorOffset2->setUnknownReadValue();

			wgtMinAbsServoFrequency->setUnknownReadValue();
			wgtMaxAbsServoFrequency->setUnknownReadValue();
			wgtConcentricAccelerationLaw->setUnknownReadValue();
			wgtConcentricDecelerationLaw->setUnknownReadValue();
			wgtEccentricAccelerationLaw->setUnknownReadValue();
			wgtEccentricDecelerationLaw->setUnknownReadValue();

		}

		(reportLogger->stream) << "\n";
        reportLogger->flush(true);


	}
}
//=================================================================================================
