#ifndef WIDGETSETTINGSPOSITION_H
#define WIDGETSETTINGSPOSITION_H

#include <QWidget>
#include <QDebug>
#include <QLabel>

#include <QPushButton>
#include <QBoxLayout>



#include "serialporttransceiver.h"
#include "TLV.h"
#include "tlvwriter.h"


#include "WidgetSettingsInteger.h"


struct PersonalSettings{

    qint32 positionMainA;
    qint32	positionMainB;
    qint32	positionMainParking;
    qint32	positionAux1;
    qint32	positionAux2;
    qint32	positionAux3;
    qint32	speedAbsMainParking;
    qint32	speedRelMainTestEccentric;
    qint32	speedRelMainTestConcentric;
    qint32 forceCurveAB[21];
    qint32 forceCurveBA[21];

};


class WidgetPersonalSettings : public QWidget{
    
    Q_OBJECT

    QLabel *lblRxMessageCounter;
    QLabel *lblTxMessageCounter;

    WidgetSettingsInteger *wgtPositionMainA;
    WidgetSettingsInteger *wgtPositionMainB;
    WidgetSettingsInteger *wgtPositionParking;
    WidgetSettingsInteger *wgtPositionAux1;
    WidgetSettingsInteger *wgtPositionAux2;
    WidgetSettingsInteger *wgtPositionAux3;
    WidgetSettingsInteger *wgtSpeedAbsMainParking;
    WidgetSettingsInteger *wgtSpeedRelMainTestEccentric;
    WidgetSettingsInteger *wgtSpeedRelMainTestConcentric;

    QPushButton *btnWriteSettings;

    QVBoxLayout *loMain;

    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;
    quint32 txMessageCounter;



public:
    explicit WidgetPersonalSettings(
            SerialPortTransceiver *serialPortTransceiver,
            QWidget *parent = 0);


signals:

public slots:

    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);
    void slotWriteSettings();


};

#endif // WIDGETSETTINGSPOSITION_H
