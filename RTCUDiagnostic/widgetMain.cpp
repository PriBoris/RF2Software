#include "widgetMain.h"

//===============================================================================================================
WidgetMain::WidgetMain(QWidget *parent) : QWidget(parent)
{

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


    fontRegular = new QFont("Verdana", 10, QFont::Normal);
    fontSelect = new QFont("Verdana", 10, QFont::Bold);



    lo = new QVBoxLayout;


    //------------------------------

    lblPortStatus = new QLabel("");
    btnTabMode = new QPushButton("Mode");
    btnTabPersonalSettings = new QPushButton("PersonalSettings");
    btnTabMachineSettings = new QPushButton("MachineSettings");
    btnTabExcerciseSettings = new QPushButton("ExcerciseSettings");
    btnTabNFC = new QPushButton("NFC");
    btnTabConsole = new QPushButton("Console");
    loTabs = new QHBoxLayout;

    lblPortStatus->setFixedWidth(150);
    btnTabMode->setFixedWidth(150);
    btnTabPersonalSettings->setFixedWidth(150);
    btnTabMachineSettings->setFixedWidth(150);
    btnTabExcerciseSettings->setFixedWidth(150);
    btnTabNFC->setFixedWidth(150);
    btnTabConsole->setFixedWidth(150);


    loTabs->addWidget(lblPortStatus);
    loTabs->addWidget(btnTabMode);
    loTabs->addWidget(btnTabMachineSettings);
    loTabs->addWidget(btnTabPersonalSettings);
    loTabs->addWidget(btnTabExcerciseSettings);
    loTabs->addWidget(btnTabNFC);
    loTabs->addWidget(btnTabConsole);
    loTabs->addStretch(1);

    if (serialPortOpened==true){
        lblPortStatus->setText(serialPort->portName()+" OK");
    }else{
        lblPortStatus->setText(serialPort->portName()+" FAIL");
    }



    {
        QSignalMapper* signalMapper = new QSignalMapper (this) ;
        connect (btnTabMode, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
        connect (btnTabPersonalSettings, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
        connect (btnTabMachineSettings, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
        connect (btnTabExcerciseSettings, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
        connect (btnTabNFC, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
        connect (btnTabConsole, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;

        signalMapper -> setMapping (btnTabMode, TAB_Mode) ;
        signalMapper -> setMapping (btnTabMachineSettings, TAB_MachineSettings) ;
        signalMapper -> setMapping (btnTabPersonalSettings, TAB_PersonalSettings) ;
        signalMapper -> setMapping (btnTabExcerciseSettings, TAB_ExcerciseSettings) ;
        signalMapper -> setMapping (btnTabNFC, TAB_NFC) ;
        signalMapper -> setMapping (btnTabConsole, TAB_CONSOLE) ;

        connect (signalMapper, SIGNAL(mapped(int)), this, SLOT(slotTabClicked(int))) ;


    }


    lo->addLayout(loTabs);


    //------------------------------

    widgetHmiMode = new WidgetMode(serialPortTransceiver,"Mode",false);
    widgetSettingsPosition = new WidgetPersonalSettings(serialPortTransceiver,"PersonalSettings",false);
    widgetMachineSettings = new WidgetMachineSettings(serialPortTransceiver,"MachineSettings",false);
    widgetExcerciseSettings = new WidgetExcerciseSettings(serialPortTransceiver,"ExcerciseSettings",false);
    widgetNFC = new WidgetNFC(serialPortTransceiver,"NFC",false);
    widgetConsole = new WidgetConsole(serialPortTransceiver,"Console",false);

    widgetArray.append(widgetHmiMode);
    widgetArray.append(widgetMachineSettings);
    widgetArray.append(widgetSettingsPosition);
    widgetArray.append(widgetExcerciseSettings);
    widgetArray.append(widgetNFC);
    widgetArray.append(widgetConsole);

    for(int i=0;i<widgetArray.length();i++)
    {
        widgetGroupBoxArray.append(new QGroupBox(widgetArray.at(i)->getName()));

        widgetGroupBoxArray.at(i)->setStyleSheet("QGroupBox { font-weight: bold; font: 12pt;} ");

        widgetGroupBoxLayoutArray.append(new QVBoxLayout);

        widgetGroupBoxLayoutArray.at(i)->addWidget(widgetArray.at(i));
        widgetGroupBoxLayoutArray.at(i)->addStretch(1);
        widgetGroupBoxArray.at(i)->setLayout(widgetGroupBoxLayoutArray.at(i));

        lo->addWidget(widgetGroupBoxArray.at(i));

    }
    lo->addStretch(1);
    this->setLayout(lo);

    connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetHmiMode,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
    connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetSettingsPosition,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
    connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetMachineSettings,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
    connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetExcerciseSettings,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
    connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetNFC,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));
    connect(tlvReader,SIGNAL(newMessageReceived(quint8,quint32,QByteArray&)),widgetConsole,SLOT(newMessageReceived(quint8,quint32,QByteArray&)));




    {
        QSettings *settings = new QSettings();
        int savedTabIndex = settings->value("/Settings/tabIndex", TAB_Mode).toInt();
        slotTabClicked(savedTabIndex);
        delete settings;
    }




}
//===============================================================================================================
void WidgetMain::slotTabClicked(int tabIndex){

    switch(tabIndex){
    case TAB_Mode:

        btnTabMode->setFont(*fontSelect);
        btnTabPersonalSettings->setFont(*fontRegular);
        btnTabMachineSettings->setFont(*fontRegular);
        btnTabExcerciseSettings->setFont(*fontRegular);
        btnTabNFC->setFont(*fontRegular);
        btnTabConsole->setFont(*fontRegular);


        widgetHmiMode->setVisible(true);
        widgetSettingsPosition->setVisible(false);
        widgetMachineSettings->setVisible(false);
        widgetExcerciseSettings->setVisible(false);
        widgetNFC->setVisible(false);
        widgetConsole->setVisible(false);

        break;
    case TAB_PersonalSettings:

        btnTabMode->setFont(*fontRegular);
        btnTabPersonalSettings->setFont(*fontSelect);
        btnTabMachineSettings->setFont(*fontRegular);
        btnTabExcerciseSettings->setFont(*fontRegular);
        btnTabNFC->setFont(*fontRegular);
        btnTabConsole->setFont(*fontRegular);

        widgetHmiMode->setVisible(false);
        widgetSettingsPosition->setVisible(true);
        widgetMachineSettings->setVisible(false);
        widgetExcerciseSettings->setVisible(false);
        widgetNFC->setVisible(false);
        widgetConsole->setVisible(false);

        break;
    case TAB_MachineSettings:

        btnTabMode->setFont(*fontRegular);
        btnTabPersonalSettings->setFont(*fontRegular);
        btnTabMachineSettings->setFont(*fontSelect);
        btnTabExcerciseSettings->setFont(*fontRegular);
        btnTabNFC->setFont(*fontRegular);
        btnTabConsole->setFont(*fontRegular);

        widgetHmiMode->setVisible(false);
        widgetSettingsPosition->setVisible(false);
        widgetMachineSettings->setVisible(true);
        widgetExcerciseSettings->setVisible(false);
        widgetNFC->setVisible(false);
        widgetConsole->setVisible(false);

        break;
    case TAB_ExcerciseSettings:

        btnTabMode->setFont(*fontRegular);
        btnTabPersonalSettings->setFont(*fontRegular);
        btnTabMachineSettings->setFont(*fontRegular);
        btnTabExcerciseSettings->setFont(*fontSelect);
        btnTabNFC->setFont(*fontRegular);
        btnTabConsole->setFont(*fontRegular);

        widgetHmiMode->setVisible(false);
        widgetSettingsPosition->setVisible(false);
        widgetMachineSettings->setVisible(false);
        widgetExcerciseSettings->setVisible(true);
        widgetNFC->setVisible(false);
        widgetConsole->setVisible(false);
        break;
    case TAB_NFC:

        btnTabMode->setFont(*fontRegular);
        btnTabPersonalSettings->setFont(*fontRegular);
        btnTabMachineSettings->setFont(*fontRegular);
        btnTabExcerciseSettings->setFont(*fontRegular);
        btnTabNFC->setFont(*fontSelect);
        btnTabConsole->setFont(*fontRegular);

        widgetHmiMode->setVisible(false);
        widgetSettingsPosition->setVisible(false);
        widgetMachineSettings->setVisible(false);
        widgetExcerciseSettings->setVisible(false);
        widgetNFC->setVisible(true);
        widgetConsole->setVisible(false);
        break;
    case TAB_CONSOLE:

        btnTabMode->setFont(*fontRegular);
        btnTabPersonalSettings->setFont(*fontRegular);
        btnTabMachineSettings->setFont(*fontRegular);
        btnTabExcerciseSettings->setFont(*fontRegular);
        btnTabNFC->setFont(*fontRegular);
        btnTabConsole->setFont(*fontSelect);

        widgetHmiMode->setVisible(false);
        widgetSettingsPosition->setVisible(false);
        widgetMachineSettings->setVisible(false);
        widgetExcerciseSettings->setVisible(false);
        widgetNFC->setVisible(false);
        widgetConsole->setVisible(true);
        break;
    }

    {
        QSettings *settings = new QSettings();
        settings->setValue("/Settings/tabIndex",tabIndex);
        delete settings;
    }


}
//===============================================================================================================
