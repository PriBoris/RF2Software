#include "widgetExcerciseSettings.h"

WidgetExcerciseSettings::WidgetExcerciseSettings(
        SerialPortTransceiver *serialPortTransceiver,
        QWidget *parent
        ) : QWidget(parent)
{


    serialPortTransceiver_ = serialPortTransceiver;
    rxMessageCounter = 0;
    settings = new QSettings;

    lblRxMessageCounter = new QLabel("lblRxMessageCounter");

    wgtSetCount = new WidgetSettingsInteger("SetCount","ExcerciseSettings_SetCount",3);


    loSetTable = new QGridLayout;

    edtPauseTime = new QLineEdit[MAX_SET_COUNT];
    edtFirstInterruptionTime = new QLineEdit[MAX_SET_COUNT];
    edtSecondInterruptionTime = new QLineEdit[MAX_SET_COUNT];
    edtStartPoint = new QLineEdit[MAX_SET_COUNT];
    edtSpeedAB = new QLineEdit[MAX_SET_COUNT];
    edtSpeedBA = new QLineEdit[MAX_SET_COUNT];
    edtRepsCount = new QLineEdit[MAX_SET_COUNT];

    lblPauseTime = new QLabel[MAX_SET_COUNT];
    lblFirstInterruptionTime = new QLabel[MAX_SET_COUNT];
    lblSecondInterruptionTime = new QLabel[MAX_SET_COUNT];
    lblStartPoint = new QLabel[MAX_SET_COUNT];
    lblSpeedAB = new QLabel[MAX_SET_COUNT];
    lblSpeedBA = new QLabel[MAX_SET_COUNT];
    lblRepsCount = new QLabel[MAX_SET_COUNT];

    loSetTable->addWidget(new QLabel("lblPauseTime"),0,0);
    loSetTable->addWidget(new QLabel("lblFirstInterruptionTime"),1,0);
    loSetTable->addWidget(new QLabel("lblSecondInterruptionTime"),2,0);
    loSetTable->addWidget(new QLabel("lblStartPoint"),3,0);
    loSetTable->addWidget(new QLabel("lblSpeedAB"),4,0);
    loSetTable->addWidget(new QLabel("lblSpeedBA"),5,0);
    loSetTable->addWidget(new QLabel("lblRepsCount"),6,0);
    for(int i=0;i<MAX_SET_COUNT;i++)
    {
        loSetTable->addWidget(&edtPauseTime[i],0,1+i);
        loSetTable->addWidget(&edtFirstInterruptionTime[i],1,1+i);
        loSetTable->addWidget(&edtSecondInterruptionTime[i],2,1+i);
        loSetTable->addWidget(&edtStartPoint[i],3,1+i);
        loSetTable->addWidget(&edtSpeedAB[i],4,1+i);
        loSetTable->addWidget(&edtSpeedBA[i],5,1+i);
        loSetTable->addWidget(&edtRepsCount[i],6,1+i);

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

        loSetTable->addWidget(&lblPauseTime[i],0,1+MAX_SET_COUNT+i);
        loSetTable->addWidget(&lblFirstInterruptionTime[i],1,1+MAX_SET_COUNT+i);
        loSetTable->addWidget(&lblSecondInterruptionTime[i],2,1+MAX_SET_COUNT+i);
        loSetTable->addWidget(&lblStartPoint[i],3,1+MAX_SET_COUNT+i);
        loSetTable->addWidget(&lblSpeedAB[i],4,1+MAX_SET_COUNT+i);
        loSetTable->addWidget(&lblSpeedBA[i],5,1+MAX_SET_COUNT+i);
        loSetTable->addWidget(&lblRepsCount[i],6,1+MAX_SET_COUNT+i);

        lblPauseTime[i].setText("...");
        lblFirstInterruptionTime[i].setText("...");
        lblSecondInterruptionTime[i].setText("...");
        lblStartPoint[i].setText("...");
        lblSpeedAB[i].setText("...");
        lblSpeedBA[i].setText("...");
        lblRepsCount[i].setText("...");
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

    loSetTable->setColumnStretch(1+MAX_SET_COUNT*2,1);
    loSetTable->setRowStretch(5,1);



    btnWriteSettings = new QPushButton("послать настройки");

    loMain = new QVBoxLayout;
    loMain->addWidget(lblRxMessageCounter);
    loMain->addWidget(wgtSetCount);
    loMain->addLayout(loSetTable);
    loMain->addWidget(btnWriteSettings);
    loMain->addStretch(1);

    this->setLayout(loMain);

    connect(btnWriteSettings,SIGNAL(clicked(bool)),SLOT(slotWriteSettings()));

}
//============================================================================================================

void WidgetExcerciseSettings::slotWriteSettings()
{
    qDebug() << "WidgetExcerciseSettings::slotWriteSettings";

    qDebug()
            << "setSettings[0]: "
            << QString::number(setSettings[0].pauseTime) << " "
            << QString::number(setSettings[0].firstInterruptionTime) << " "
            << QString::number(setSettings[0].secondInterruptionTime) << " "
            << QString::number(setSettings[0].startPoint) << " "
            << QString::number(setSettings[0].speedAB) << " "
            << QString::number(setSettings[0].speedBA) << " "
            << QString::number(setSettings[0].repsCount) << " "
        ;


    if (serialPortTransceiver_->isPortOK()==true)
    {
        qint32 setCount = wgtSetCount->getWriteValue();
        if ((setCount>0)&&(setCount<=MAX_SET_COUNT))
        {
            QByteArray valueArray;
            valueArray.append((char*)setSettings,setCount*sizeof(IsokineticSetSettings));
            TLVWriter tlv(TLV::TAG_LoadIsokineticExcerciseSettings,valueArray);
            QByteArray *txArray = tlv.getStuffedArray();
            serialPortTransceiver_->write(*txArray);
            delete txArray;
        }
        else
        {
             qDebug() << "WidgetExcerciseSettings::slotWriteSettings error, bad setCount";
        }




    }
}
//============================================================================================================

void WidgetExcerciseSettings::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value)
{
    if (tag==TLV::TAG_ReportIsokineticExcerciseSettings)
    {
        rxMessageCounter++;
        lblRxMessageCounter->setText(
                    "Сообщений: "+QString::number(rxMessageCounter)+" "
                    +"(длина последнего = "
                    +QString::number(value.length())
                    +" байт)"
                    );

        qint32 setCount = value.length()/sizeof(IsokineticSetSettings);
        wgtSetCount->setReadValue(setCount);

        for(int i=0;i<MAX_SET_COUNT;i++)
        {
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
        for(int setIndex=0;setIndex<setCount;setIndex++)
        {
            lblPauseTime[setIndex].setText(QString::number(deviceSetSettings[setIndex].pauseTime));
            lblFirstInterruptionTime[setIndex].setText(QString::number(deviceSetSettings[setIndex].firstInterruptionTime));
            lblSecondInterruptionTime[setIndex].setText(QString::number(deviceSetSettings[setIndex].secondInterruptionTime));
            lblStartPoint[setIndex].setText(QString::number(deviceSetSettings[setIndex].startPoint));
            lblSpeedAB[setIndex].setText(QString::number(deviceSetSettings[setIndex].speedAB));
            lblSpeedBA[setIndex].setText(QString::number(deviceSetSettings[setIndex].speedBA));
            lblRepsCount[setIndex].setText(QString::number(deviceSetSettings[setIndex].repsCount));
        }


    }

}
//============================================================================================================
void WidgetExcerciseSettings::setTableEditingFinished()
{
    //qDebug() << "WidgetExcerciseSettings::setTableEditingFinished";
    for(int setIndex=0;setIndex<MAX_SET_COUNT;setIndex++)
    {

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

    qDebug()
            << "setSettings[0]: "
            << QString::number(setSettings[0].pauseTime) << " "
            << QString::number(setSettings[0].firstInterruptionTime) << " "
            << QString::number(setSettings[0].secondInterruptionTime) << " "
            << QString::number(setSettings[0].startPoint) << " "
            << QString::number(setSettings[0].speedAB) << " "
            << QString::number(setSettings[0].speedBA) << " "
            << QString::number(setSettings[0].repsCount) << " "
        ;

}
//============================================================================================================



