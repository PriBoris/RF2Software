#ifndef WIDGETMACHINESETTINGSEXTENDED_H
#define WIDGETMACHINESETTINGSEXTENDED_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>

#include "serialporttransceiver.h"
#include "TLV.h"
#include "tlvwriter.h"


#include "WidgetSettingsInteger.h"
#include "WidgetSettingsFloat.h"
#include "ReportLogger.h"


struct TMachineSettingsExtended{

	qint32 positionMainMax;
	qint32 positionMainMin;
	qint32 positionAux1Max;
	qint32 positionAux1Min;
	qint32 positionAux2Max;
	qint32 positionAux2Min;
	qint32 positionAux3Max;
	qint32 positionAux3Min;
	qint32 speedAbsMainMax;
	qint32 speedAbsMainPersonal;
	qint32 encoderBitCount;
	qint32 encoderDirection;
	qint32 encoderOffset;
	qint32 forceSensorOffset;
	float forceSensorGain;

};

class WidgetMachineSettingsExtended : public QWidget
{
    Q_OBJECT

    QLabel *lblRxMessageCounter;
    QLabel *lblTxMessageCounter;

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
	WidgetSettingsInteger *wgtEncoderBitCount;
	WidgetSettingsInteger *wgtEncoderDirection;
	WidgetSettingsInteger *wgtEncoderOffset;
	WidgetSettingsInteger *wgtForceSensorOffset;
	WidgetSettingsFloat *wgtForceSensorGain;



    QPushButton *btnWriteSettings;

    QVBoxLayout *loMain;

    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;
    quint32 txMessageCounter;

    ReportLogger *reportLogger;

public:
    explicit WidgetMachineSettingsExtended(
            SerialPortTransceiver *serialPortTransceiver,
            QWidget *parent = 0
            );
    ~WidgetMachineSettingsExtended();
    
signals:

public slots:

    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);
    void slotWriteSettings();

};

#endif // WIDGETMACHINESETTINGSEXTENDED_H
