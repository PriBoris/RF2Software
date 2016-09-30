#include "widgetPersonalSettings.h"


//============================================================================================================
WidgetPersonalSettings::WidgetPersonalSettings(
        SerialPortTransceiver *serialPortTransceiver,
        QString name,
        bool bbWidget,
        QWidget *parent
        ) : Widget(parent)
{
    serialPortTransceiver_ = serialPortTransceiver;
    name_ = name;
    bbWidget_ = bbWidget;
    rxMessageCounter = 0;


    lblRxMessageCounter = new QLabel("lblRxMessageCounter");

    wgtPositionMainA = new WidgetSettingsInteger("PositionMainA","SettingsPosition_PositionMainA",1000);
    wgtPositionMainB = new WidgetSettingsInteger("PositionMainB","SettingsPosition_PositionMainB",5000);
    wgtPositionParking = new WidgetSettingsInteger("PositionParking","SettingsPosition_PositionParking",4000);
    wgtPositionAux1 = new WidgetSettingsInteger("PositionAux1","SettingsPosition_PositionAux1",2000);
    wgtPositionAux2 = new WidgetSettingsInteger("PositionAux2","SettingsPosition_PositionAux2",2100);
    wgtPositionAux3 = new WidgetSettingsInteger("PositionAux3","SettingsPosition_PositionAux3",2200);
    wgtSpeedAbsMainParking = new WidgetSettingsInteger("SpeedAbsMainParking","SettingsPosition_SpeedAbsMainParking",5000);
    wgtSpeedRelMainTestEccentric = new WidgetSettingsInteger("SpeedRelMainTestEccentric","SettingsPosition_SpeedAbsMainParking",3000);
    wgtSpeedRelMainTestConcentric = new WidgetSettingsInteger("SpeedRelMainTestConcentric","SettingsPosition_SpeedAbsMainParking",3000);









    btnWriteSettings = new QPushButton("послать настройки");

    loMain = new QVBoxLayout;
    loMain->addWidget(lblRxMessageCounter);
    loMain->addWidget(wgtPositionMainA);
    loMain->addWidget(wgtPositionMainB);
    loMain->addWidget(wgtPositionParking);
    loMain->addWidget(wgtPositionAux1);
    loMain->addWidget(wgtPositionAux2);
    loMain->addWidget(wgtPositionAux3);
    loMain->addWidget(wgtSpeedAbsMainParking);
    loMain->addWidget(wgtSpeedRelMainTestEccentric);
    loMain->addWidget(wgtSpeedRelMainTestConcentric);
    loMain->addWidget(btnWriteSettings);
    loMain->addStretch(1);

    this->setLayout(loMain);

    connect(btnWriteSettings,SIGNAL(clicked(bool)),SLOT(slotWriteSettings()));


}
//============================================================================================================
void WidgetPersonalSettings::slotWriteSettings(){
    //qDebug() << "WidgetSettingsPosition::slotWriteSettings";
    if (serialPortTransceiver_->isPortOK()==true)
    {
        PersonalSettings personalSettings;

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
    }


}
//============================================================================================================
void WidgetPersonalSettings::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){



    if (tag==TLV::TAG_ReportPersonalSettings){

        rxMessageCounter++;
        lblRxMessageCounter->setText(
                    "Сообщений: "+QString::number(rxMessageCounter)+" "
                    +"(длина последнего = "
                    +QString::number(value.length())
                    +" байт)"
                    );


        if (value.length()==sizeof(PersonalSettings)){

            PersonalSettings newPersonalSettings;
            memcpy(&newPersonalSettings,value.data(),sizeof(PersonalSettings));

            wgtPositionMainA->setReadValue(newPersonalSettings.positionMainA);
            wgtPositionMainB->setReadValue(newPersonalSettings.positionMainB);
            wgtPositionParking->setReadValue(newPersonalSettings.positionMainParking);
            wgtPositionAux1->setReadValue(newPersonalSettings.positionAux1);
            wgtPositionAux2->setReadValue(newPersonalSettings.positionAux2);
            wgtPositionAux3->setReadValue(newPersonalSettings.positionAux3);
            wgtSpeedAbsMainParking->setReadValue(newPersonalSettings.speedAbsMainParking);
            wgtSpeedRelMainTestEccentric->setReadValue(newPersonalSettings.speedRelMainTestEccentric);
            wgtSpeedRelMainTestConcentric->setReadValue(newPersonalSettings.speedRelMainTestConcentric);

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





    }

}
//============================================================================================================
