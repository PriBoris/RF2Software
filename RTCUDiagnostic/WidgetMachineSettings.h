#ifndef WIDGETMACHINESETTINGS_H
#define WIDGETMACHINESETTINGS_H

#include <QWidget>
#include <QDebug>
#include <QLabel>

#include <QPushButton>
#include <QBoxLayout>



#include "serialporttransceiver.h"
#include "TLV.h"
#include "tlvwriter.h"

#include "WidgetSettingsInteger.h"
#include "ReportLogger.h"
#include "RTCU.h"




class WidgetMachineSettings : public QWidget{


    Q_OBJECT

    QLabel *lblRxMessageCounter;
    
    WidgetSettingsInteger *wgtPositionMainMax;
    WidgetSettingsInteger *wgtPositionMainMin;
    WidgetSettingsInteger *wgtPositionAux1Max;
    WidgetSettingsInteger *wgtPositionAux1Min;
    WidgetSettingsInteger *wgtPositionAux2Max;
    WidgetSettingsInteger *wgtPositionAux2Min;
    WidgetSettingsInteger *wgtPositionAux3Max;
    WidgetSettingsInteger *wgtPositionAux3Min;
    WidgetSettingsInteger *wgtSpeedAbsMainMax;
    WidgetSettingsInteger *wgtSpeedAbsMainPersonal;


    QPushButton *btnWriteSettings;

    QVBoxLayout *loMain;

    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;

    ReportLogger *reportLogger;

public:
    explicit WidgetMachineSettings(
            SerialPortTransceiver *serialPortTransceiver,
            QWidget *parent = 0
            );
    ~WidgetMachineSettings();

signals:

public slots:

    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);
    void slotWriteSettings();

};

#endif // WIDGETMACHINESETTINGS_H
