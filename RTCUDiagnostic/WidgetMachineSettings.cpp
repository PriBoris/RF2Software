
#include "WidgetMachineSettings.h"

#include <QDateTime>

#include "Utils.h"


//=================================================================================================
WidgetMachineSettings::~WidgetMachineSettings(){
    delete reportLogger;
}
//=================================================================================================
WidgetMachineSettings::WidgetMachineSettings(
        SerialPortTransceiver *serialPortTransceiver,
        QWidget *parent
        ) : QWidget(parent){
    
    serialPortTransceiver_ = serialPortTransceiver;
    rxMessageCounter = 0;

    reportLogger = new ReportLogger("ReportMachineSettings.txt");

    {
        lblRxMessageCounter = new QLabel;

        lblRxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));

        Utils::MessageCounterInitialize("Rx",rxMessageCounter,lblRxMessageCounter);

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


        wgtPositionMainMax->setReadOnly();
        wgtPositionMainMin->setReadOnly();
        wgtPositionAux1Max->setReadOnly();
        wgtPositionAux1Min->setReadOnly();
        wgtPositionAux2Max->setReadOnly();
        wgtPositionAux2Min->setReadOnly();
        wgtPositionAux3Max->setReadOnly();
        wgtPositionAux3Min->setReadOnly();
        wgtSpeedAbsMainMax->setReadOnly();
        wgtSpeedAbsMainPersonal->setReadOnly();
    }

    btnWriteSettings = new QPushButton("Send Machine Settings");
    btnWriteSettings->setFixedWidth(600);
    btnWriteSettings->setFont(QFont("Verdana",10,QFont::Bold,true));
    btnWriteSettings->setEnabled(false);


    loMain = new QVBoxLayout;

    loMain->addWidget(lblRxMessageCounter);
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
    loMain->addWidget(btnWriteSettings);
        loMain->addSpacing(10);

    loMain->addStretch(1);

    this->setLayout(loMain);

    connect(btnWriteSettings,SIGNAL(clicked(bool)),SLOT(slotWriteSettings()));

}
//============================================================================================================
void WidgetMachineSettings::slotWriteSettings(){

    if (serialPortTransceiver_->isPortOK()==true){


        TMachineSettings newMachineSettings;

        newMachineSettings.positionMainMax = wgtPositionMainMax->getWriteValue();
        newMachineSettings.positionMainMin = wgtPositionMainMin->getWriteValue();
        newMachineSettings.positionAux1Max = wgtPositionAux1Max->getWriteValue();
        newMachineSettings.positionAux1Min = wgtPositionAux1Min->getWriteValue();
        newMachineSettings.positionAux2Max = wgtPositionAux2Max->getWriteValue();
        newMachineSettings.positionAux2Min = wgtPositionAux2Min->getWriteValue();
        newMachineSettings.positionAux3Max = wgtPositionAux3Max->getWriteValue();
        newMachineSettings.positionAux3Min = wgtPositionAux3Min->getWriteValue();
        newMachineSettings.speedAbsMainMax = wgtSpeedAbsMainMax->getWriteValue();
        newMachineSettings.speedAbsMainPersonal = wgtSpeedAbsMainPersonal->getWriteValue();

        QByteArray valueArray;
        valueArray.append((char*)&newMachineSettings,sizeof(newMachineSettings));
        TLVWriter tlv(TLV::TAG_LoadMachineSettings,valueArray);
        QByteArray *txArray = tlv.getStuffedArray();
        serialPortTransceiver_->write(*txArray);
        delete txArray;
    }


}
//============================================================================================================
void WidgetMachineSettings::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){

    if (tag==TLV::TAG_ReportMachineSettings){

        Utils::MessageCounterIncrement("Rx",rxMessageCounter,lblRxMessageCounter,value);

        {
            QDateTime dateTime(QDateTime::currentDateTime());
            QString pcTimeStr = dateTime.toString("yyyy-MM-dd HH:mm:ss");
            (reportLogger->stream) << pcTimeStr << "\n";
        }


        if (value.length()==sizeof(TMachineSettings)){

            TMachineSettings machineSettings;
            memcpy(&machineSettings,value.data(),sizeof(TMachineSettings));

            wgtPositionMainMax->setReadValue(machineSettings.positionMainMax);
            wgtPositionMainMin->setReadValue(machineSettings.positionMainMin);
            wgtPositionAux1Max->setReadValue(machineSettings.positionAux1Max);
            wgtPositionAux1Min->setReadValue(machineSettings.positionAux1Min);
            wgtPositionAux2Max->setReadValue(machineSettings.positionAux2Max);
            wgtPositionAux2Min->setReadValue(machineSettings.positionAux2Min);
            wgtPositionAux3Max->setReadValue(machineSettings.positionAux3Max);
            wgtPositionAux3Min->setReadValue(machineSettings.positionAux3Min);
            wgtSpeedAbsMainMax->setReadValue(machineSettings.speedAbsMainMax);
            wgtSpeedAbsMainPersonal->setReadValue(machineSettings.speedAbsMainPersonal);

            (reportLogger->stream) << "positionMainMax=" << QString::number(machineSettings.positionMainMax) << "\n";
            (reportLogger->stream) << "positionMainMin=" << QString::number(machineSettings.positionMainMin) << "\n";
            (reportLogger->stream) << "positionAux1Max=" << QString::number(machineSettings.positionAux1Max) << "\n";
            (reportLogger->stream) << "positionAux1Min=" << QString::number(machineSettings.positionAux1Min) << "\n";
            (reportLogger->stream) << "positionAux2Max=" << QString::number(machineSettings.positionAux2Max) << "\n";
            (reportLogger->stream) << "positionAux2Min=" << QString::number(machineSettings.positionAux2Min) << "\n";
            (reportLogger->stream) << "positionAux3Max=" << QString::number(machineSettings.positionAux3Max) << "\n";
            (reportLogger->stream) << "positionAux3Min=" << QString::number(machineSettings.positionAux3Min) << "\n";
            (reportLogger->stream) << "speedAbsMainMax=" << QString::number(machineSettings.speedAbsMainMax) << "\n";
            (reportLogger->stream) << "speedAbsMainPersonal=" << QString::number(machineSettings.speedAbsMainPersonal) << "\n";


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


        }

        (reportLogger->stream) << "\n";
        reportLogger->flush();

    }





}
//============================================================================================================
