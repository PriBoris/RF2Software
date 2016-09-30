#ifndef WIDGETMACHINESETTINGS_H
#define WIDGETMACHINESETTINGS_H

#include <QWidget>
#include <QDebug>
#include <QLabel>

#include <QPushButton>
#include <QBoxLayout>



#include "widget.h"
#include "serialporttransceiver.h"
#include "tlv.h"
#include "tlvwriter.h"


#include "widgetSettingsInteger.h"

struct TMachineSettings
{
    qint32	positionMainMax;
    qint32	positionMainMin;
    qint32	positionAux1Max;
    qint32	positionAux1Min;
    qint32	positionAux2Max;
    qint32	positionAux2Min;
    qint32	positionAux3Max;
    qint32	positionAux3Min;
    qint32 speedAbsMainMax;
    qint32 speedAbsMainPersonal;

};


class WidgetMachineSettings : public Widget
{


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
    QString name_;
    bool bbWidget_;

public:
    explicit WidgetMachineSettings(
            SerialPortTransceiver *serialPortTransceiver,
            QString name,
            bool bbWidget,
            QWidget *parent = 0
            );
    QString getName(){return name_;}
    bool isBBWidget(){return bbWidget_;}

signals:

public slots:

    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);
    void slotWriteSettings();

};

#endif // WIDGETMACHINESETTINGS_H
