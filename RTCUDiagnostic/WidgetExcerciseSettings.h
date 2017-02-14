#ifndef WIDGETEXCERCISESETTINGS_H
#define WIDGETEXCERCISESETTINGS_H

#include <QWidget>
#include <QLabel>

#include <QPushButton>
#include <QBoxLayout>

#include <QGridLayout>
#include <QSettings>

#include "serialporttransceiver.h"
#include "TLV.h"
#include "tlvwriter.h"

#include "WidgetSettingsInteger.h"
#include "ReportLogger.h"


#define MAX_SET_COUNT   (10)

struct IsokineticSetSettings{

    qint32 pauseTime;
    qint32 firstInterruptionTime;
    qint32 secondInterruptionTime;
    qint32	startPoint;
    qint32	speedAB;
    qint32	speedBA;
    qint32	repsCount;
};

class WidgetExcerciseSettings : public QWidget{

    Q_OBJECT
    QLabel *lblRxMessageCounter;
    QLabel *lblTxMessageCounter;

    WidgetSettingsInteger *wgtSetCount;



    QLabel *lblPauseTimeTitle;
    QLabel *lblFirstInterruptionTimeTitle;
    QLabel *lblSecondInterruptionTimeTitle;
    QLabel *lblStartPointTitle;
    QLabel *lblSpeedABTitle;
    QLabel *lblSpeedBATitle;
    QLabel *lblRepsCountTitle;

    QLineEdit *edtPauseTime;
    QLineEdit *edtFirstInterruptionTime;
    QLineEdit *edtSecondInterruptionTime;
    QLineEdit *edtStartPoint;
    QLineEdit *edtSpeedAB;
    QLineEdit *edtSpeedBA;
    QLineEdit *edtRepsCount;

    QLabel *lblPauseTime;
    QLabel *lblFirstInterruptionTime;
    QLabel *lblSecondInterruptionTime;
    QLabel *lblStartPoint;
    QLabel *lblSpeedAB;
    QLabel *lblSpeedBA;
    QLabel *lblRepsCount;

    QGridLayout *loWriteTable;
    QGridLayout *loReadTable;

    IsokineticSetSettings setSettings[MAX_SET_COUNT];

    QPushButton *btnWriteSettings;

    QVBoxLayout *loMain;

    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;
    quint32 txMessageCounter;
    QSettings *settings;

    ReportLogger *reportLogger;

public:
    explicit WidgetExcerciseSettings(
            SerialPortTransceiver *serialPortTransceiver,
            QWidget *parent = 0
            );
    ~WidgetExcerciseSettings();

signals:

public slots:

    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);
    void slotWriteSettings();

    void setTableEditingFinished();

};

#endif // WIDGETEXCERCISESETTINGS_H
