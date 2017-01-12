#ifndef WIDGETHMIMODE_H
#define WIDGETHMIMODE_H

#include <QWidget>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QSignalMapper>
#include <QTimer>

#include "serialporttransceiver.h"
#include "TLV.h"
#include "tlvwriter.h"
#include "personalbutton.h"
#include "WidgetTestDynamicStart.h"
#include "WidgetTestStaticStart.h"
#include "ReportLogger.h"
#include "qcustomplot.h"


class WidgetMode : public QWidget
{
    Q_OBJECT

    ReportLogger *reportLogger;

    QLabel *lblRxMessageCounter;
    QLabel *lblDateTime;
    QLabel *lblMode;
    QLabel *lblPhase;

    /*---------------------------------------------------*/
    QLabel *lblError;
    QLabel *lblPosition;
    QLabel *lblPositionRel;
    QLabel *lblTimeToTest;
    QLabel *lblForceValue;
    QLabel *lblSetIndex;
    QLabel *lblRepIndex;
    QLabel *lblRepDirection;
    QLabel *lblTimeToSet;
    QPushButton *btnEnableServo;
    QPushButton *btnParking;
    QPushButton *btnPersonalStart;
    QPushButton *btnPersonalExit;
    WidgetTestDynamicStart *btnTestConcentricStart;
    WidgetTestDynamicStart *btnTestEccentricStart;
    WidgetTestStaticStart *btnTestStaticStart;
    QWidget *wgtPersonal;
    QPushButton *btnResetError;
    QPushButton *btnExcerciseIsokineticStart;
    QPushButton *btnCancel;
    QLabel *lblOdometer;
    /*---------------------------------------------------*/



    QHBoxLayout *loPersonal;
    PersonalButton *btnPersonalMainPlus;
    PersonalButton *btnPersonalMainMinus;
    PersonalButton *btnPersonalAux1Plus;
    PersonalButton *btnPersonalAux1Minus;
    PersonalButton *btnPersonalAux2Plus;
    PersonalButton *btnPersonalAux2Minus;
    //PersonalButton *btnPersonalAux3Plus;
    //PersonalButton *btnPersonalAux3Minus;
    QSignalMapper *signalMapperPressed;
    QSignalMapper *signalMapperReleased;
    QSignalMapper *signalMapperHold;


    QCustomPlot* plotForceVsPosition;
    QCustomPlot* plotForceVsTime;
    QPushButton* btnPlotForceReset;
    QVector<double> plotX,plotY,plotT;
    double getMax(QVector<double> &data);
    double getMin(QVector<double> &data);
    double plotTime;



    QVBoxLayout *loMain;

    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;



    static const quint8 ERROR_ServoInternal = 0;
    static const quint8 ERROR_ServoConnectionLost = 1;
    static const quint8 ERROR_ServoVoltageLost = 2;
    static const quint8 ERROR_ServoControlLost = 3;
    static const quint8 ERROR_RtcuLogicError=4;
    static const quint8 ERROR_AuxMoved=5;
    static const quint8 ERROR_HmiParametersError=6;
    static const quint8 ERROR_StrainGaugeLost=7;
    static const quint8 ERROR_EmergencyStop=8;
    static const quint8 ERROR_PositionMainSensorLost=0x09;
    static const quint8 ERROR_RfidReaderLost = 0x0A;

    static const quint8 ERROR_Unknown=0xFF;


    class Phase{
    public:
        static const qint32 HOMING=0;
        static const qint32 PAUSE=1;
        static const qint32 TEST_CONCENTRIC=2;
        static const qint32 TEST_ECCENTRIC=3;
        static const qint32 ISOKINETIC_FIRSTMOVE=4;
        static const qint32 ISOKINETIC_FIRSTINTERRUPTION=5;
        static const qint32 ISOKINETIC_SECONDMOVE=6;
        static const qint32 ISOKINETIC_SECONDINTERRUPTION=7;
        static const qint32 TEST_STATIC=8;
        static const qint32 PAUSE1=9;
        static const qint32 PAUSE2=10;
        static const qint32 GENERIC_MOVE=11;
    };








public:
    explicit WidgetMode(
            SerialPortTransceiver *serialPortTransceiver,
            QWidget *parent = 0);
    ~WidgetMode();


signals:
    void personalPressed(int btnIndex);
    void personalReleased(int btnIndex);
    void personalHold(int btnIndex);

public slots:

    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);
    void slotEnableServo();
    void slotParking();
    void slotPersonalStart();
    void slotPersonalExit();

    void slotPersonalPressed(int btnIndex);
    void slotPersonalReleased(int btnIndex);
    void slotPersonalHold(int btnIndex);

    void slotTestConcentricStart(qint32 pauseBeforeTest);
    void slotTestEccentricStart(qint32 pauseBeforeTest);
    void slotTestStaticStart(QByteArray &testSettings);

    void slotResetError();
    void slotExcerciseIsokineticStart();
    void slotCancel();

    void slotPlotForceReset();


};

#endif // WIDGETHMIMODE_H
