#include "WidgetMachineSettingsExtended.h"

WidgetMachineSettingsExtended::WidgetMachineSettingsExtended(
        SerialPortTransceiver *serialPortTransceiver,
        QWidget *parent
        ) : QWidget(parent){

    serialPortTransceiver_ = serialPortTransceiver;
    rxMessageCounter = 0;

    lblRxMessageCounter = new QLabel("lblRxMessageCounter");
    lblRxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));

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
        wgtForceSensorDirection = new WidgetSettingsInteger("forceSensorDirection","MachineSettingsExtended_forceSensorDirection",1);
        wgtForceSensorOffset = new WidgetSettingsInteger("forceSensorOffset","MachineSettingsExtended_forceSensorOffset",20000);
        wgtForceSensorGain = new WidgetSettingsFloat("forceSensorGain","MachineSettingsExtended_forceSensorGain",4000.0f);

    }

    btnWriteSettings = new QPushButton("Send Extended Machine Settings");
    btnWriteSettings->setFixedWidth(600);
    btnWriteSettings->setFont(QFont("Verdana",10,QFont::Bold,true));



    {
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
        loMain->addWidget(wgtEncoderBitCount);
        loMain->addWidget(wgtEncoderDirection);
        loMain->addWidget(wgtEncoderOffset);
            loMain->addSpacing(10);
        loMain->addWidget(wgtForceSensorDirection);
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
        newMachineSettingsExtended.forceSensorDirection = wgtForceSensorDirection->getWriteValue();
        newMachineSettingsExtended.forceSensorOffset = wgtForceSensorOffset->getWriteValue();
        newMachineSettingsExtended.forceSensorGain = wgtForceSensorGain->getWriteValue();


        QByteArray valueArray;
        valueArray.append((char*)&newMachineSettingsExtended,sizeof(newMachineSettingsExtended));
        TLVWriter tlv(TLV::TAG_LoadMachineSettingsExtended,valueArray);
        QByteArray *txArray = tlv.getStuffedArray();
        serialPortTransceiver_->write(*txArray);
        delete txArray;
    }

}
//=================================================================================================
void WidgetMachineSettingsExtended::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){

    if (tag==TLV::TAG_ReportMachineSettingsExtended){

        rxMessageCounter++;
        lblRxMessageCounter->setText(
                    "Сообщений: "+QString::number(rxMessageCounter)+" "
                    +"(длина последнего = "
                    +QString::number(value.length())
                    +" байт)"
                    );

        if (value.length()==sizeof(TMachineSettingsExtended)){


        }else{

        	
        }

    }
}
//=================================================================================================
