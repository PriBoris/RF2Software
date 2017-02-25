#include "WidgetExcerciseSettings.h"

#include <QDateTime>

#include "Utils.h"

//=================================================================================================
WidgetExcerciseSettings::~WidgetExcerciseSettings(){
	delete reportLogger;
}
//=================================================================================================
WidgetExcerciseSettings::WidgetExcerciseSettings(
		SerialPortTransceiver *serialPortTransceiver,
		QWidget *parent
		) : QWidget(parent)
{


	serialPortTransceiver_ = serialPortTransceiver;

	settings = new QSettings;

	reportLogger = new ReportLogger("ReportExcerciseSettings.txt");

	{
		lblRxMessageCounter = new QLabel;
		lblTxMessageCounter = new QLabel;

		lblRxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));
		lblTxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));

		Utils::MessageCounterInitialize("Rx",rxMessageCounter,lblRxMessageCounter);
		Utils::MessageCounterInitialize("Tx",txMessageCounter,lblTxMessageCounter);

	}

	wgtSetCount = new WidgetSettingsInteger("SetCount","ExcerciseSettings_SetCount",3);




	{
		edtPauseTime = new QLineEdit[MAX_SET_COUNT];
		edtFirstInterruptionTime = new QLineEdit[MAX_SET_COUNT];
		edtSecondInterruptionTime = new QLineEdit[MAX_SET_COUNT];
		edtStartPoint = new QLineEdit[MAX_SET_COUNT];
		edtSpeedAB = new QLineEdit[MAX_SET_COUNT];
		edtSpeedBA = new QLineEdit[MAX_SET_COUNT];
		edtRepsCount = new QLineEdit[MAX_SET_COUNT];

		for(int i=0;i<MAX_SET_COUNT;i++){

			edtPauseTime[i].setFixedWidth(75);
			edtFirstInterruptionTime[i].setFixedWidth(75);
			edtSecondInterruptionTime[i].setFixedWidth(75);
			edtStartPoint[i].setFixedWidth(75);
			edtSpeedAB[i].setFixedWidth(75);
			edtSpeedBA[i].setFixedWidth(75);
			edtRepsCount[i].setFixedWidth(75);

			edtPauseTime[i].setFont(QFont("Verdana",10,QFont::Normal,true));
			edtFirstInterruptionTime[i].setFont(QFont("Verdana",10,QFont::Normal,true));
			edtSecondInterruptionTime[i].setFont(QFont("Verdana",10,QFont::Normal,true));
			edtStartPoint[i].setFont(QFont("Verdana",10,QFont::Normal,true));
			edtSpeedAB[i].setFont(QFont("Verdana",10,QFont::Normal,true));
			edtSpeedBA[i].setFont(QFont("Verdana",10,QFont::Normal,true));
			edtRepsCount[i].setFont(QFont("Verdana",10,QFont::Normal,true));


		

		}


	}

	{
		lblPauseTime = new QLabel[MAX_SET_COUNT];
		lblFirstInterruptionTime = new QLabel[MAX_SET_COUNT];
		lblSecondInterruptionTime = new QLabel[MAX_SET_COUNT];
		lblStartPoint = new QLabel[MAX_SET_COUNT];
		lblSpeedAB = new QLabel[MAX_SET_COUNT];
		lblSpeedBA = new QLabel[MAX_SET_COUNT];
		lblRepsCount = new QLabel[MAX_SET_COUNT];

		for(int i=0;i<MAX_SET_COUNT;i++){

			lblPauseTime[i].setFixedWidth(75);
			lblFirstInterruptionTime[i].setFixedWidth(75);
			lblSecondInterruptionTime[i].setFixedWidth(75);
			lblStartPoint[i].setFixedWidth(75);
			lblSpeedAB[i].setFixedWidth(75);
			lblSpeedBA[i].setFixedWidth(75);
			lblRepsCount[i].setFixedWidth(75);

			lblPauseTime[i].setFont(QFont("Verdana",10,QFont::Normal,true));
			lblFirstInterruptionTime[i].setFont(QFont("Verdana",10,QFont::Normal,true));
			lblSecondInterruptionTime[i].setFont(QFont("Verdana",10,QFont::Normal,true));
			lblStartPoint[i].setFont(QFont("Verdana",10,QFont::Normal,true));
			lblSpeedAB[i].setFont(QFont("Verdana",10,QFont::Normal,true));
			lblSpeedBA[i].setFont(QFont("Verdana",10,QFont::Normal,true));
			lblRepsCount[i].setFont(QFont("Verdana",10,QFont::Normal,true));

		}

	}

	{
		lblPauseTimeTitle = new QLabel[2];
		lblFirstInterruptionTimeTitle = new QLabel[2];
		lblSecondInterruptionTimeTitle = new QLabel[2];
		lblStartPointTitle = new QLabel[2];
		lblSpeedABTitle = new QLabel[2];
		lblSpeedBATitle = new QLabel[2];
		lblRepsCountTitle = new QLabel[2];

		lblPauseTimeTitle[0].setText("PauseTime");
		lblFirstInterruptionTimeTitle[0].setText("FirstInterruptionTime");
		lblSecondInterruptionTimeTitle[0].setText("SecondInterruptionTime");
		lblStartPointTitle[0].setText("StartPoint");
		lblSpeedABTitle[0].setText("SpeedAB");
		lblSpeedBATitle[0].setText("SpeedBA");
		lblRepsCountTitle[0].setText("RepsCount");

		lblPauseTimeTitle[1].setText("PauseTime");
		lblFirstInterruptionTimeTitle[1].setText("FirstInterruptionTime");
		lblSecondInterruptionTimeTitle[1].setText("SecondInterruptionTime");
		lblStartPointTitle[1].setText("StartPoint");
		lblSpeedABTitle[1].setText("SpeedAB");
		lblSpeedBATitle[1].setText("SpeedBA");
		lblRepsCountTitle[1].setText("RepsCount");

		lblPauseTimeTitle[0].setFont(QFont("Verdana",10,QFont::Normal,true));
		lblFirstInterruptionTimeTitle[0].setFont(QFont("Verdana",10,QFont::Normal,true));
		lblSecondInterruptionTimeTitle[0].setFont(QFont("Verdana",10,QFont::Normal,true));
		lblStartPointTitle[0].setFont(QFont("Verdana",10,QFont::Normal,true));
		lblSpeedABTitle[0].setFont(QFont("Verdana",10,QFont::Normal,true));
		lblSpeedBATitle[0].setFont(QFont("Verdana",10,QFont::Normal,true));
		lblRepsCountTitle[0].setFont(QFont("Verdana",10,QFont::Normal,true));

		lblPauseTimeTitle[1].setFont(QFont("Verdana",10,QFont::Normal,true));
		lblFirstInterruptionTimeTitle[1].setFont(QFont("Verdana",10,QFont::Normal,true));
		lblSecondInterruptionTimeTitle[1].setFont(QFont("Verdana",10,QFont::Normal,true));
		lblStartPointTitle[1].setFont(QFont("Verdana",10,QFont::Normal,true));
		lblSpeedABTitle[1].setFont(QFont("Verdana",10,QFont::Normal,true));
		lblSpeedBATitle[1].setFont(QFont("Verdana",10,QFont::Normal,true));
		lblRepsCountTitle[1].setFont(QFont("Verdana",10,QFont::Normal,true));

	}

	loWriteTable = new QGridLayout;
	loReadTable = new QGridLayout;

	loWriteTable->addWidget(&lblPauseTimeTitle[0],0,0);
	loWriteTable->addWidget(&lblFirstInterruptionTimeTitle[0],1,0);
	loWriteTable->addWidget(&lblSecondInterruptionTimeTitle[0],2,0);
	loWriteTable->addWidget(&lblStartPointTitle[0],3,0);
	loWriteTable->addWidget(&lblSpeedABTitle[0],4,0);
	loWriteTable->addWidget(&lblSpeedBATitle[0],5,0);
	loWriteTable->addWidget(&lblRepsCountTitle[0],6,0);

	loReadTable->addWidget(&lblPauseTimeTitle[1],0,0);
	loReadTable->addWidget(&lblFirstInterruptionTimeTitle[1],1,0);
	loReadTable->addWidget(&lblSecondInterruptionTimeTitle[1],2,0);
	loReadTable->addWidget(&lblStartPointTitle[1],3,0);
	loReadTable->addWidget(&lblSpeedABTitle[1],4,0);
	loReadTable->addWidget(&lblSpeedBATitle[1],5,0);
	loReadTable->addWidget(&lblRepsCountTitle[1],6,0);

	for(int i=0;i<MAX_SET_COUNT;i++){

		loWriteTable->addWidget(&edtPauseTime[i],0,1+i);
		loWriteTable->addWidget(&edtFirstInterruptionTime[i],1,1+i);
		loWriteTable->addWidget(&edtSecondInterruptionTime[i],2,1+i);
		loWriteTable->addWidget(&edtStartPoint[i],3,1+i);
		loWriteTable->addWidget(&edtSpeedAB[i],4,1+i);
		loWriteTable->addWidget(&edtSpeedBA[i],5,1+i);
		loWriteTable->addWidget(&edtRepsCount[i],6,1+i);


		setSettings[i].pauseTime = settings->value("ExcerciseSettings_PauseTime"+QString::number(i),20000).toInt();
		setSettings[i].firstInterruptionTime = settings->value("ExcerciseSettings_FirstInterruptionTime"+QString::number(i),200).toInt();
		setSettings[i].secondInterruptionTime = settings->value("ExcerciseSettings_SecondInterruptionTime"+QString::number(i),200).toInt();
		setSettings[i].startPoint = settings->value("ExcerciseSettings_StartPoint"+QString::number(i),0).toInt();
		setSettings[i].speedAB = settings->value("ExcerciseSettings_SpeedAB"+QString::number(i),5000).toInt();
		setSettings[i].speedBA = settings->value("ExcerciseSettings_SpeedBA"+QString::number(i),5000).toInt();
		setSettings[i].repsCount = settings->value("ExcerciseSettings_RepsCount"+QString::number(i),10-i).toInt();

		edtPauseTime[i].setText(QString::number(setSettings[i].pauseTime));
		edtFirstInterruptionTime[i].setText(QString::number(setSettings[i].firstInterruptionTime));
		edtSecondInterruptionTime[i].setText(QString::number(setSettings[i].secondInterruptionTime));
		edtStartPoint[i].setText(QString::number(setSettings[i].startPoint));
		edtSpeedAB[i].setText(QString::number(setSettings[i].speedAB));
		edtSpeedBA[i].setText(QString::number(setSettings[i].speedBA));
		edtRepsCount[i].setText(QString::number(setSettings[i].repsCount));

		loReadTable->addWidget(&lblPauseTime[i],0,1+i);
		loReadTable->addWidget(&lblFirstInterruptionTime[i],1,1+i);
		loReadTable->addWidget(&lblSecondInterruptionTime[i],2,1+i);
		loReadTable->addWidget(&lblStartPoint[i],3,1+i);
		loReadTable->addWidget(&lblSpeedAB[i],4,1+i);
		loReadTable->addWidget(&lblSpeedBA[i],5,1+i);
		loReadTable->addWidget(&lblRepsCount[i],6,1+i);

		lblPauseTime[i].setText("unknown");
		lblFirstInterruptionTime[i].setText("unknown");
		lblSecondInterruptionTime[i].setText("unknown");
		lblStartPoint[i].setText("unknown");
		lblSpeedAB[i].setText("unknown");
		lblSpeedBA[i].setText("unknown");
		lblRepsCount[i].setText("unknown");
		lblPauseTime[i].setStyleSheet("border: 1px solid");
		lblFirstInterruptionTime[i].setStyleSheet("border: 1px solid");
		lblSecondInterruptionTime[i].setStyleSheet("border: 1px solid");
		lblStartPoint[i].setStyleSheet("border: 1px solid");
		lblSpeedAB[i].setStyleSheet("border: 1px solid");
		lblSpeedBA[i].setStyleSheet("border: 1px solid");
		lblRepsCount[i].setStyleSheet("border: 1px solid");

		connect(&edtPauseTime[i],SIGNAL(editingFinished()),SLOT(setTableEditingFinished()));
		connect(&edtFirstInterruptionTime[i],SIGNAL(editingFinished()),SLOT(setTableEditingFinished()));
		connect(&edtSecondInterruptionTime[i],SIGNAL(editingFinished()),SLOT(setTableEditingFinished()));
		connect(&edtStartPoint[i],SIGNAL(editingFinished()),SLOT(setTableEditingFinished()));
		connect(&edtSpeedAB[i],SIGNAL(editingFinished()),SLOT(setTableEditingFinished()));
		connect(&edtSpeedBA[i],SIGNAL(editingFinished()),SLOT(setTableEditingFinished()));
		connect(&edtRepsCount[i],SIGNAL(editingFinished()),SLOT(setTableEditingFinished()));

	}


	loWriteTable->setColumnStretch(1+MAX_SET_COUNT,1);
	loWriteTable->setRowStretch(5,1);

	loReadTable->setColumnStretch(1+MAX_SET_COUNT,1);
	loReadTable->setRowStretch(5,1);


	{
		btnWriteSettings = new QPushButton("Send Excercise Settings");
		btnWriteSettings->setFixedWidth(600);
		btnWriteSettings->setFont(QFont("Verdana",10,QFont::Bold,true));
	}


	loMain = new QVBoxLayout;

	loMain->addWidget(lblRxMessageCounter);
	loMain->addWidget(lblTxMessageCounter);
	loMain->addSpacing(10);

	loMain->addWidget(wgtSetCount);
	loMain->addSpacing(10);

	loMain->addLayout(loWriteTable);
	loMain->addSpacing(10);

	loMain->addLayout(loReadTable);
	loMain->addSpacing(10);

	loMain->addWidget(btnWriteSettings);
	loMain->addStretch(1);

	this->setLayout(loMain);

	connect(btnWriteSettings,SIGNAL(clicked(bool)),SLOT(slotWriteSettings()));

}
//============================================================================================================

void WidgetExcerciseSettings::slotWriteSettings(){

	if (serialPortTransceiver_->isPortOK()==true){

		qint32 setCount = wgtSetCount->getWriteValue();

		if ((setCount>0)&&(setCount<=MAX_SET_COUNT)){

			QByteArray valueArray;
			valueArray.append((char*)setSettings,setCount*sizeof(IsokineticSetSettings));
			TLVWriter tlv(TLV::TAG_LoadIsokineticExcerciseSettings,valueArray);
			QByteArray *txArray = tlv.getStuffedArray();
			serialPortTransceiver_->write(*txArray);
			delete txArray;

			Utils::MessageCounterIncrement("Tx",txMessageCounter,lblTxMessageCounter,valueArray);

		}else{

		}




	}
}
//============================================================================================================
void WidgetExcerciseSettings::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){

	if (tag==TLV::TAG_ReportIsokineticExcerciseSettings){
		
		Utils::MessageCounterIncrement("Rx",rxMessageCounter,lblRxMessageCounter,value);

		{
			QDateTime dateTime(QDateTime::currentDateTime());
			QString pcTimeStr = dateTime.toString("yyyy-MM-dd HH:mm:ss");
			(reportLogger->stream) << pcTimeStr << "\n";
		}


		qint32 setCount = value.length()/sizeof(IsokineticSetSettings);
		(reportLogger->stream) << "setCount=" << QString::number(setCount) << "\n";


		wgtSetCount->setReadValue(setCount);

		for(int i=0;i<MAX_SET_COUNT;i++){

			lblPauseTime[i].setText("---");
			lblFirstInterruptionTime[i].setText("---");
			lblSecondInterruptionTime[i].setText("---");
			lblStartPoint[i].setText("---");
			lblSpeedAB[i].setText("---");
			lblSpeedBA[i].setText("---");
			lblRepsCount[i].setText("---");
		}

		IsokineticSetSettings deviceSetSettings[MAX_SET_COUNT];
		memcpy(deviceSetSettings,value.data(),setCount*sizeof(IsokineticSetSettings));
		for(int setIndex=0;setIndex<setCount;setIndex++){

			(reportLogger->stream) << "setIndex=" << QString::number(setIndex) << "\n";

			lblPauseTime[setIndex].setText(QString::number(deviceSetSettings[setIndex].pauseTime));
			lblFirstInterruptionTime[setIndex].setText(QString::number(deviceSetSettings[setIndex].firstInterruptionTime));
			lblSecondInterruptionTime[setIndex].setText(QString::number(deviceSetSettings[setIndex].secondInterruptionTime));
			lblStartPoint[setIndex].setText(QString::number(deviceSetSettings[setIndex].startPoint));
			lblSpeedAB[setIndex].setText(QString::number(deviceSetSettings[setIndex].speedAB));
			lblSpeedBA[setIndex].setText(QString::number(deviceSetSettings[setIndex].speedBA));
			lblRepsCount[setIndex].setText(QString::number(deviceSetSettings[setIndex].repsCount));

			(reportLogger->stream) << "\t" << "pauseTime=" << QString::number(deviceSetSettings[setIndex].pauseTime) << "\n";
			(reportLogger->stream) << "\t" << "firstInterruptionTime=" << QString::number(deviceSetSettings[setIndex].firstInterruptionTime) << "\n";
			(reportLogger->stream) << "\t" << "secondInterruptionTime=" << QString::number(deviceSetSettings[setIndex].secondInterruptionTime) << "\n";
			(reportLogger->stream) << "\t" << "startPoint=" << QString::number(deviceSetSettings[setIndex].startPoint) << "\n";
			(reportLogger->stream) << "\t" << "speedAB=" << QString::number(deviceSetSettings[setIndex].speedAB) << "\n";
			(reportLogger->stream) << "\t" << "speedBA=" << QString::number(deviceSetSettings[setIndex].speedBA) << "\n";
			(reportLogger->stream) << "\t" << "repsCount=" << QString::number(deviceSetSettings[setIndex].repsCount) << "\n";

		}

		(reportLogger->stream) << "\n";
		reportLogger->flush(true);

	}

}
//============================================================================================================
void WidgetExcerciseSettings::setTableEditingFinished(){
	
	for(int setIndex=0;setIndex<MAX_SET_COUNT;setIndex++){

		setSettings[setIndex].pauseTime = edtPauseTime[setIndex].text().toInt();
		setSettings[setIndex].firstInterruptionTime = edtFirstInterruptionTime[setIndex].text().toInt();
		setSettings[setIndex].secondInterruptionTime = edtSecondInterruptionTime[setIndex].text().toInt();
		setSettings[setIndex].startPoint = edtStartPoint[setIndex].text().toInt();
		setSettings[setIndex].speedAB = edtSpeedAB[setIndex].text().toInt();
		setSettings[setIndex].speedBA = edtSpeedBA[setIndex].text().toInt();
		setSettings[setIndex].repsCount = edtRepsCount[setIndex].text().toInt();

		settings->setValue("ExcerciseSettings_PauseTime"+QString::number(setIndex),setSettings[setIndex].pauseTime);
		settings->setValue("ExcerciseSettings_FirstInterruptionTime"+QString::number(setIndex),setSettings[setIndex].firstInterruptionTime);
		settings->setValue("ExcerciseSettings_SecondInterruptionTime"+QString::number(setIndex),setSettings[setIndex].secondInterruptionTime);
		settings->setValue("ExcerciseSettings_StartPoint"+QString::number(setIndex),setSettings[setIndex].startPoint);
		settings->setValue("ExcerciseSettings_SpeedAB"+QString::number(setIndex),setSettings[setIndex].speedAB);
		settings->setValue("ExcerciseSettings_SpeedBA"+QString::number(setIndex),setSettings[setIndex].speedBA);
		settings->setValue("ExcerciseSettings_RepsCount"+QString::number(setIndex),setSettings[setIndex].repsCount);

	}


}
//============================================================================================================



