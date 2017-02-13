#include "WidgetMain.h"

#include <QScrollArea>

#include "RTCU.h"
#include "TLV.h"


//===============================================================================================================
WidgetMain::WidgetMain(QWidget *parent) : QWidget(parent){

	serialPort = new QSerialPort();
	serialPort->setPortName("COM5");
	serialPort->setBaudRate(QSerialPort::Baud115200);
	bool serialPortOpened = serialPort->open(QIODevice::ReadWrite);
	if (false==serialPortOpened){
		qDebug() << "FATAL: unable to open serial port" << serialPort->portName();
	}
	serialPortTransceiver = new SerialPortTransceiver(serialPort);
	tlvReader = new TLVReader();
	connect(serialPortTransceiver,SIGNAL(newReadData(QByteArray&)),tlvReader,SLOT(processReadData(QByteArray&)));

	RTCU::init();
	TLV::init();


	fontRegular = new QFont("Verdana", 10, QFont::Normal);
	fontSelect = new QFont("Verdana", 10, QFont::Black);



	lo = new QVBoxLayout;


	//------------------------------

	{
		btnTabMode = new QPushButton("Mode");
		btnTabServoMode = new QPushButton("ServoMode");
		btnTabPersonalSettings = new QPushButton("PersonalSettings");
		btnTabMachineSettings = new QPushButton("MachineSettings");
		btnTabExcerciseSettings = new QPushButton("ExcerciseSettings");
		btnTabGenericSetSettings = new QPushButton("GenericSetSettings");
		btnTabNFC = new QPushButton("NFC");
		btnTabConsole = new QPushButton("Console");

		btnTabMode->setFixedWidth(150);
		btnTabServoMode->setFixedWidth(150);
		btnTabPersonalSettings->setFixedWidth(150);
		btnTabMachineSettings->setFixedWidth(150);
		btnTabExcerciseSettings->setFixedWidth(150);
		btnTabGenericSetSettings->setFixedWidth(150);
		btnTabNFC->setFixedWidth(150);
		btnTabConsole->setFixedWidth(150);

		loTabs = new QGridLayout;

		loTabs->addWidget(btnTabMode,0,0);
		loTabs->addWidget(btnTabServoMode,1,0);
		loTabs->addWidget(btnTabMachineSettings,0,1);
		loTabs->addWidget(btnTabPersonalSettings,1,1);
		loTabs->addWidget(btnTabExcerciseSettings,0,2);
		loTabs->addWidget(btnTabGenericSetSettings,1,2);
		loTabs->addWidget(btnTabNFC,0,3);
		loTabs->addWidget(btnTabConsole,0,4);

	}



	{
		lblPortStatus = new QLabel("");
		lblPortStatus->setFixedWidth(150);
        lblPortStatus->setFont(QFont("Verdana",8,QFont::Normal,false));

		lblPortRxStats = new QLabel("0 rx msgs");
		lblPortRxStats->setFixedWidth(150);
        lblPortRxStats->setFont(QFont("Verdana",8,QFont::Normal,false));

		lblPortRxErrors = new QLabel("0 rx errors");
		lblPortRxErrors->setFixedWidth(150);
        lblPortRxErrors->setFont(QFont("Verdana",8,QFont::Normal,false));

		lblMode = new QLabel("???");
		lblMode->setFixedWidth(150);
        lblMode->setFont(QFont("Verdana",8,QFont::Normal,false));


		loPort = new QVBoxLayout;
		loPort->addWidget(lblPortStatus);
		loPort->addWidget(lblPortRxStats);
		loPort->addWidget(lblPortRxErrors);
		loPort->addWidget(lblMode);

		rxMessageCounter = 0;
		rxMessageMaxLength = 0;
		rxErrorCounter = 0;
	}


	{
		loHeader = new QHBoxLayout;

		loHeader->addLayout(loTabs);
		loHeader->addLayout(loPort);
		loHeader->addStretch(1);
	}

	if (serialPortOpened==true){
		lblPortStatus->setText(serialPort->portName()+" OK");
	}else{
		lblPortStatus->setText(serialPort->portName()+" FAIL");
	}



	{
		QSignalMapper* signalMapper = new QSignalMapper (this) ;
		connect (btnTabMode, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
		connect (btnTabServoMode, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
		connect (btnTabPersonalSettings, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
		connect (btnTabMachineSettings, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
		connect (btnTabExcerciseSettings, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
		connect (btnTabGenericSetSettings, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
		connect (btnTabNFC, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
		connect (btnTabConsole, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;

		signalMapper -> setMapping (btnTabMode, TAB_Mode) ;
		signalMapper -> setMapping (btnTabServoMode, TAB_ServoMode) ;
		signalMapper -> setMapping (btnTabMachineSettings, TAB_MachineSettings) ;
		signalMapper -> setMapping (btnTabPersonalSettings, TAB_PersonalSettings) ;
		signalMapper -> setMapping (btnTabExcerciseSettings, TAB_ExcerciseSettings) ;
		signalMapper -> setMapping (btnTabGenericSetSettings, TAB_GenericSetSettings) ;
		signalMapper -> setMapping (btnTabNFC, TAB_NFC) ;
		signalMapper -> setMapping (btnTabConsole, TAB_CONSOLE) ;

		connect (signalMapper, SIGNAL(mapped(int)), this, SLOT(slotTabClicked(int))) ;


	}


	lo->addLayout(loHeader);


	loContainer = new QVBoxLayout;


	//------------------------------

	widgetMode = new WidgetMode(serialPortTransceiver);
	widgetServoMode = new WidgetServoMode(serialPortTransceiver);
	widgetSettingsPosition = new WidgetPersonalSettings(serialPortTransceiver);
	widgetMachineSettings = new WidgetMachineSettings(serialPortTransceiver);
	widgetMachineSettingsExtended = new WidgetMachineSettingsExtended(serialPortTransceiver);
	widgetExcerciseSettings = new WidgetExcerciseSettings(serialPortTransceiver);
	widgetGenericSetSettings = new WidgetGenericSetSettings(serialPortTransceiver);
	widgetNFC = new WidgetNFC(serialPortTransceiver);
	widgetConsole = new WidgetConsole(serialPortTransceiver);

	widgetArray.append(widgetMode);
	widgetArray.append(widgetServoMode);
	widgetArray.append(widgetMachineSettings);
	widgetArray.append(widgetMachineSettingsExtended);
	widgetArray.append(widgetSettingsPosition);
	widgetArray.append(widgetExcerciseSettings);
	widgetArray.append(widgetGenericSetSettings);
	widgetArray.append(widgetNFC);
	widgetArray.append(widgetConsole);

	for(int i=0;i<widgetArray.length();i++){
		 loContainer->addWidget(widgetArray.at(i));
	}
	loContainer->addStretch(1);

	container = new QWidget;
	container->setLayout(loContainer);
	scrollArea=new QScrollArea;
	scrollArea->setWidget(container);
	lo->addWidget(scrollArea);







	this->setLayout(lo);
	restoreGeometry();


	connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetMode,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
	connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetServoMode,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
	connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetSettingsPosition,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
	connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetMachineSettings,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
	connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetMachineSettingsExtended,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
	connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetExcerciseSettings,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
	connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetGenericSetSettings,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
	connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetNFC,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
	connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetConsole,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));

	connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
	connect(tlvReader,SIGNAL(errorDetected(void)),SLOT(errorDetected(void)));




	{
		QSettings *settings = new QSettings();
		int savedTabIndex = settings->value("/Settings/tabIndex", TAB_Mode).toInt();
		slotTabClicked(savedTabIndex);
		delete settings;
	}




}
//===============================================================================================================
WidgetMain::~WidgetMain(){
	saveGeometry();
}
//===============================================================================================================
void WidgetMain::slotTabClicked(int tabIndex){

	switch(tabIndex){
	case TAB_Mode:

		btnTabMode->setFont(*fontSelect);
		btnTabServoMode->setFont(*fontRegular);
		btnTabPersonalSettings->setFont(*fontRegular);
		btnTabMachineSettings->setFont(*fontRegular);
		btnTabExcerciseSettings->setFont(*fontRegular);
		btnTabGenericSetSettings->setFont(*fontRegular);
		btnTabNFC->setFont(*fontRegular);
		btnTabConsole->setFont(*fontRegular);


		widgetMode->setVisible(true);
		widgetServoMode->setVisible(false);
		widgetSettingsPosition->setVisible(false);
		widgetMachineSettings->setVisible(false);
		widgetMachineSettingsExtended->setVisible(false);
		widgetExcerciseSettings->setVisible(false);
		widgetGenericSetSettings->setVisible(false);
		widgetNFC->setVisible(false);
		widgetConsole->setVisible(false);

		break;

	case TAB_ServoMode:

		btnTabMode->setFont(*fontRegular);
		btnTabServoMode->setFont(*fontSelect);
		btnTabPersonalSettings->setFont(*fontRegular);
		btnTabMachineSettings->setFont(*fontRegular);
		btnTabExcerciseSettings->setFont(*fontRegular);
		btnTabGenericSetSettings->setFont(*fontRegular);
		btnTabNFC->setFont(*fontRegular);
		btnTabConsole->setFont(*fontRegular);


		widgetMode->setVisible(false);
		widgetServoMode->setVisible(true);
		widgetSettingsPosition->setVisible(false);
		widgetMachineSettings->setVisible(false);
		widgetMachineSettingsExtended->setVisible(false);
		widgetExcerciseSettings->setVisible(false);
		widgetGenericSetSettings->setVisible(false);
		widgetNFC->setVisible(false);
		widgetConsole->setVisible(false);

		break;
	case TAB_PersonalSettings:

		btnTabMode->setFont(*fontRegular);
		btnTabServoMode->setFont(*fontRegular);
		btnTabPersonalSettings->setFont(*fontSelect);
		btnTabMachineSettings->setFont(*fontRegular);
		btnTabExcerciseSettings->setFont(*fontRegular);
		btnTabGenericSetSettings->setFont(*fontRegular);
		btnTabNFC->setFont(*fontRegular);
		btnTabConsole->setFont(*fontRegular);

		widgetMode->setVisible(false);
		widgetServoMode->setVisible(false);
		widgetSettingsPosition->setVisible(true);
		widgetMachineSettings->setVisible(false);
		widgetMachineSettingsExtended->setVisible(false);
		widgetExcerciseSettings->setVisible(false);
		widgetGenericSetSettings->setVisible(false);
		widgetNFC->setVisible(false);
		widgetConsole->setVisible(false);

		break;
	case TAB_MachineSettings:

		btnTabMode->setFont(*fontRegular);
		btnTabServoMode->setFont(*fontRegular);
		btnTabPersonalSettings->setFont(*fontRegular);
		btnTabMachineSettings->setFont(*fontSelect);
		btnTabExcerciseSettings->setFont(*fontRegular);
		btnTabGenericSetSettings->setFont(*fontRegular);
		btnTabNFC->setFont(*fontRegular);
		btnTabConsole->setFont(*fontRegular);

		widgetMode->setVisible(false);
		widgetServoMode->setVisible(false);
		widgetSettingsPosition->setVisible(false);
		widgetMachineSettings->setVisible(true);
		widgetMachineSettingsExtended->setVisible(true);
		widgetExcerciseSettings->setVisible(false);
		widgetGenericSetSettings->setVisible(false);
		widgetNFC->setVisible(false);
		widgetConsole->setVisible(false);

		break;
	case TAB_ExcerciseSettings:

		btnTabMode->setFont(*fontRegular);
		btnTabServoMode->setFont(*fontRegular);
		btnTabPersonalSettings->setFont(*fontRegular);
		btnTabMachineSettings->setFont(*fontRegular);
		btnTabExcerciseSettings->setFont(*fontSelect);
		btnTabGenericSetSettings->setFont(*fontRegular);
		btnTabNFC->setFont(*fontRegular);
		btnTabConsole->setFont(*fontRegular);

		widgetMode->setVisible(false);
		widgetServoMode->setVisible(false);
		widgetSettingsPosition->setVisible(false);
		widgetMachineSettings->setVisible(false);
		widgetMachineSettingsExtended->setVisible(false);
		widgetExcerciseSettings->setVisible(true);
		widgetGenericSetSettings->setVisible(false);
		widgetNFC->setVisible(false);
		widgetConsole->setVisible(false);
		break;

   case TAB_GenericSetSettings:

		btnTabMode->setFont(*fontRegular);
		btnTabServoMode->setFont(*fontRegular);
		btnTabPersonalSettings->setFont(*fontRegular);
		btnTabMachineSettings->setFont(*fontRegular);
		btnTabExcerciseSettings->setFont(*fontRegular);
		btnTabGenericSetSettings->setFont(*fontSelect);
		btnTabNFC->setFont(*fontRegular);
		btnTabConsole->setFont(*fontRegular);

		widgetMode->setVisible(false);
		widgetServoMode->setVisible(false);
		widgetSettingsPosition->setVisible(false);
		widgetMachineSettings->setVisible(false);
		widgetMachineSettingsExtended->setVisible(false);
		widgetExcerciseSettings->setVisible(false);
		widgetGenericSetSettings->setVisible(true);
		widgetNFC->setVisible(false);
		widgetConsole->setVisible(false);
		break;


	case TAB_NFC:

		btnTabMode->setFont(*fontRegular);
		btnTabServoMode->setFont(*fontRegular);
		btnTabPersonalSettings->setFont(*fontRegular);
		btnTabMachineSettings->setFont(*fontRegular);
		btnTabExcerciseSettings->setFont(*fontRegular);
		btnTabGenericSetSettings->setFont(*fontRegular);
		btnTabNFC->setFont(*fontSelect);
		btnTabConsole->setFont(*fontRegular);

		widgetMode->setVisible(false);
		widgetServoMode->setVisible(false);
		widgetSettingsPosition->setVisible(false);
		widgetMachineSettings->setVisible(false);
		widgetMachineSettingsExtended->setVisible(false);
		widgetExcerciseSettings->setVisible(false);
		widgetGenericSetSettings->setVisible(false);
		widgetNFC->setVisible(true);
		widgetConsole->setVisible(false);
		break;

	case TAB_CONSOLE:

		btnTabMode->setFont(*fontRegular);
		btnTabServoMode->setFont(*fontRegular);
		btnTabPersonalSettings->setFont(*fontRegular);
		btnTabMachineSettings->setFont(*fontRegular);
		widgetMachineSettingsExtended->setVisible(false);
		btnTabExcerciseSettings->setFont(*fontRegular);
		btnTabGenericSetSettings->setFont(*fontRegular);
		btnTabNFC->setFont(*fontRegular);
		btnTabConsole->setFont(*fontSelect);

		widgetMode->setVisible(false);
		widgetServoMode->setVisible(false);
		widgetSettingsPosition->setVisible(false);
		widgetMachineSettings->setVisible(false);
		widgetExcerciseSettings->setVisible(false);
		widgetGenericSetSettings->setVisible(false);
		widgetNFC->setVisible(false);
		widgetConsole->setVisible(true);
		break;
	}


	{
		QSettings *settings = new QSettings();
		settings->setValue("/Settings/tabIndex",tabIndex);
		delete settings;
	}
	scrollArea->verticalScrollBar()->setMaximum(0);
	scrollArea->horizontalScrollBar()->setMaximum(0);
}
//===============================================================================================================
void WidgetMain::saveGeometry(){

	QPoint position = this->pos();
	QRect rect = this->geometry();
	QSettings settings;
	settings.setValue("/Settings/windowX",QString::number(rect.x()));
	settings.setValue("/Settings/windowY",QString::number(rect.y()));
	settings.setValue("/Settings/windowMaximized",((this->windowState() & Qt::WindowMaximized)!=0));

	settings.setValue("/Settings/windowWidth",QString::number(rect.width()));
	settings.setValue("/Settings/windowHeight",QString::number(rect.height()));

}
//===============================================================================================================
void WidgetMain::restoreGeometry(){

	QSettings settings;
	if (settings.value("/Settings/windowMaximized",true).toBool()==true){
		this->setWindowState(Qt::WindowMaximized);
	} else {
		QPoint position(
			settings.value("/Settings/windowX", 100).toInt(),
			settings.value("/Settings/windowY", 100).toInt()
			);
		this->move(position);
		this->resize(
			settings.value("/Settings/windowWidth", 300).toInt(),
			settings.value("/Settings/windowHeight", 300).toInt()
			);
	}

}
//===============================================================================================================
void WidgetMain::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){
	
	rxMessageCounter++;
	if (value.length() > rxMessageMaxLength){
		rxMessageMaxLength = value.length();
	}
	lblPortRxStats->setText(
		QString::number(rxMessageCounter)+" rx msgs"
		);

	if (tag==TLV::TAG_ReportCurrentMode){
		uint8_t mode = value.at(0);
		lblMode->setText(RTCU::Mode::getTitle(mode));
	}

}
//===============================================================================================================
void WidgetMain::errorDetected(void){

	rxErrorCounter++;
	lblPortRxErrors->setText(
		QString::number(rxErrorCounter)+" rx errors"
		);

}
//===============================================================================================================


