#ifndef WIDGETSERVOMODE_H
#define WIDGETSERVOMODE_H

#include <QWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QPair>

#include "serialporttransceiver.h"
#include "TLV.h"
#include "ReportLogger.h"
#include "RTCU.h"

#include "qcustomplot.h"


class WidgetServoMode : public QWidget{
    Q_OBJECT

    QLabel *lblRxMessageCounter;
    QLabel *lblMainTickID;
    QLabel *lblDateTime;
    QLabel *lblMode;
    QLabel *lblSubmode;

    QLabel *lblServoFrequencyPositive;
    QLabel *lblServoFrequencyNegative;
    QLabel *lblPosition;

    QLabel *lblServoCommand;

    QCustomPlot* plotPositionVsTime;
    QVector<double> plotPositionData,plotTimeData;
    QVector<double> plotPersonalAData,plotPersonalBData;
    QVector<double> plotPositionMaxData,plotPositionMinData;
    double plotTime;
    static const int PLOT_MAX_DURATION_SECONDS = (3*60);

    void updatePlotRange(
        QVector<double> &plotData, 
        QPair<double,double> &range
        );

    bool personalSettingsValid;
    RTCU::TPersonalSettings personalSettings;

    bool machineSettingsExtendedValid;
    RTCU::TMachineSettingsExtended machineSettingsExtended;





    QVBoxLayout *loMain;

    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;
    ReportLogger *reportLogger;


public:
    explicit WidgetServoMode(
            SerialPortTransceiver *serialPortTransceiver,
            QWidget *parent = 0);
    ~WidgetServoMode();

signals:

public slots:

    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);

};

#endif // WIDGETSERVOMODE_H
