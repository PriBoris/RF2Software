#include "WidgetPersonalSettings.h"

#include <QDateTime>

#include "Utils.h"
#include "RTCU.h"

//=================================================================================================
WidgetPersonalSettings::~WidgetPersonalSettings(){
	delete reportLogger;
}
//============================================================================================================
WidgetPersonalSettings::WidgetPersonalSettings(
		SerialPortTransceiver *serialPortTransceiver,
		QWidget *parent
		) : QWidget(parent)
{
	serialPortTransceiver_ = serialPortTransceiver;

	reportLogger = new ReportLogger("ReportPersonalSettings.txt");

	{
		lblRxMessageCounter = new QLabel;
		lblTxMessageCounter = new QLabel;

		lblRxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));
		lblTxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));

		Utils::MessageCounterInitialize("Rx",rxMessageCounter,lblRxMessageCounter);
		Utils::MessageCounterInitialize("Tx",txMessageCounter,lblTxMessageCounter);

	}

	wgtPositionMainA = new WidgetSettingsInteger("PositionMainA","PersonalSettings_PositionMainA",1000);
	wgtPositionMainB = new WidgetSettingsInteger("PositionMainB","PersonalSettings_PositionMainB",5000);
	wgtPositionParking = new WidgetSettingsInteger("PositionParking","PersonalSettings_PositionParking",4000);
	wgtPositionAux1 = new WidgetSettingsInteger("PositionAux1","PersonalSettings_PositionAux1",2000);
	wgtPositionAux2 = new WidgetSettingsInteger("PositionAux2","PersonalSettings_PositionAux2",2100);
	wgtPositionAux3 = new WidgetSettingsInteger("PositionAux3","PersonalSettings_PositionAux3",2200);
	wgtSpeedAbsMainParking = new WidgetSettingsInteger("SpeedAbsMainParking","PersonalSettings_SpeedAbsMainParking",15000);
	wgtSpeedRelMainTestEccentric = new WidgetSettingsInteger("SpeedRelMainTestEccentric","PersonalSettings_SpeedRelMainTestEccentric",6000);
	wgtSpeedRelMainTestConcentric = new WidgetSettingsInteger("SpeedRelMainTestConcentric","PersonalSettings_SpeedRelMainTestConcentric",6000);









	{
		btnWriteSettings = new QPushButton("Send Personal Settings");
		btnWriteSettings->setFixedWidth(600);
		btnWriteSettings->setFont(QFont("Verdana",10,QFont::Bold,true));
	}



	loMain = new QVBoxLayout;

	loMain->addWidget(lblRxMessageCounter);
	loMain->addWidget(lblTxMessageCounter);
	loMain->addSpacing(10);

	loMain->addWidget(wgtPositionMainA);
	loMain->addWidget(wgtPositionMainB);
	loMain->addWidget(wgtPositionParking);
	loMain->addWidget(wgtPositionAux1);
	loMain->addWidget(wgtPositionAux2);
	loMain->addWidget(wgtPositionAux3);
	loMain->addWidget(wgtSpeedAbsMainParking);
	loMain->addWidget(wgtSpeedRelMainTestEccentric);
	loMain->addWidget(wgtSpeedRelMainTestConcentric);
	loMain->addSpacing(10);

	loMain->addWidget(btnWriteSettings);
	loMain->addStretch(1);

	this->setLayout(loMain);

	connect(btnWriteSettings,SIGNAL(clicked(bool)),SLOT(slotWriteSettings()));


}
//============================================================================================================
void WidgetPersonalSettings::slotWriteSettings(){


	if (serialPortTransceiver_->isPortOK()==true){

		RTCU::TPersonalSettings personalSettings;

		personalSettings.positionMainA = wgtPositionMainA->getWriteValue();
		personalSettings.positionMainB = wgtPositionMainB->getWriteValue();
		personalSettings.positionMainParking = wgtPositionParking->getWriteValue();
		personalSettings.positionAux1 = wgtPositionAux1->getWriteValue();
		personalSettings.positionAux2 = wgtPositionAux2->getWriteValue();
		personalSettings.positionAux3 = wgtPositionAux3->getWriteValue();
		personalSettings.speedAbsMainParking = wgtSpeedAbsMainParking->getWriteValue();
		personalSettings.speedRelMainTestEccentric = wgtSpeedRelMainTestEccentric->getWriteValue();
		personalSettings.speedRelMainTestConcentric = wgtSpeedRelMainTestConcentric->getWriteValue();
		memset(personalSettings.forceCurveAB,0,sizeof(personalSettings.forceCurveAB));
		memset(personalSettings.forceCurveBA,0,sizeof(personalSettings.forceCurveBA));

		QByteArray valueArray;
		valueArray.append((char*)&personalSettings,sizeof(personalSettings));

		TLVWriter tlv(TLV::TAG_LoadPersonalSettings,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;

		Utils::MessageCounterIncrement("Tx",txMessageCounter,lblTxMessageCounter,valueArray);

	}


}
//============================================================================================================
void WidgetPersonalSettings::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){



	if (tag==TLV::TAG_ReportPersonalSettings){

		Utils::MessageCounterIncrement("Rx",rxMessageCounter,lblRxMessageCounter,value);

		{
			QDateTime dateTime(QDateTime::currentDateTime());
			QString pcTimeStr = dateTime.toString("yyyy-MM-dd HH:mm:ss");
			(reportLogger->stream) << pcTimeStr << "\n";
		}

		if (value.length()==sizeof(RTCU::TPersonalSettings)){

			RTCU::TPersonalSettings newPersonalSettings;
			memcpy(&newPersonalSettings,value.data(),sizeof(RTCU::TPersonalSettings));

			wgtPositionMainA->setReadValue(newPersonalSettings.positionMainA);
			wgtPositionMainB->setReadValue(newPersonalSettings.positionMainB);
			wgtPositionParking->setReadValue(newPersonalSettings.positionMainParking);
			wgtPositionAux1->setReadValue(newPersonalSettings.positionAux1);
			wgtPositionAux2->setReadValue(newPersonalSettings.positionAux2);
			wgtPositionAux3->setReadValue(newPersonalSettings.positionAux3);
			wgtSpeedAbsMainParking->setReadValue(newPersonalSettings.speedAbsMainParking);
			wgtSpeedRelMainTestEccentric->setReadValue(newPersonalSettings.speedRelMainTestEccentric);
			wgtSpeedRelMainTestConcentric->setReadValue(newPersonalSettings.speedRelMainTestConcentric);

			(reportLogger->stream) << "positionMainA=" << QString::number(newPersonalSettings.positionMainA) << "\n";
			(reportLogger->stream) << "positionMainB=" << QString::number(newPersonalSettings.positionMainB) << "\n";
			(reportLogger->stream) << "positionMainParking=" << QString::number(newPersonalSettings.positionMainParking) << "\n";
			(reportLogger->stream) << "positionAux1=" << QString::number(newPersonalSettings.positionAux1) << "\n";
			(reportLogger->stream) << "positionAux2=" << QString::number(newPersonalSettings.positionAux2) << "\n";
			(reportLogger->stream) << "positionAux3=" << QString::number(newPersonalSettings.positionAux3) << "\n";
			(reportLogger->stream) << "speedAbsMainParking=" << QString::number(newPersonalSettings.speedAbsMainParking) << "\n";
			(reportLogger->stream) << "speedRelMainTestEccentric=" << QString::number(newPersonalSettings.speedRelMainTestEccentric) << "\n";
			(reportLogger->stream) << "speedRelMainTestConcentric=" << QString::number(newPersonalSettings.speedRelMainTestConcentric) << "\n";


		} else {

			wgtPositionMainA->setUnknownReadValue();
			wgtPositionMainB->setUnknownReadValue();
			wgtPositionParking->setUnknownReadValue();
			wgtPositionAux1->setUnknownReadValue();
			wgtPositionAux2->setUnknownReadValue();
			wgtPositionAux3->setUnknownReadValue();
			wgtSpeedAbsMainParking->setUnknownReadValue();
			wgtSpeedRelMainTestEccentric->setUnknownReadValue();
			wgtSpeedRelMainTestConcentric->setUnknownReadValue();



		}


		(reportLogger->stream) << "\n";
		reportLogger->flush(true);



	}

}
//============================================================================================================
