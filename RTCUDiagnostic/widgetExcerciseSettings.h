#ifndef WIDGETEXCERCISESETTINGS_H
#define WIDGETEXCERCISESETTINGS_H

#include <QWidget>
#include <QDebug>
#include <QLabel>

#include <QPushButton>
#include <QBoxLayout>

#include <QGridLayout>
#include <QSettings>

#include "widget.h"
#include "serialporttransceiver.h"
#include "tlv.h"
#include "tlvwriter.h"

#include "widgetSettingsInteger.h"

#define MAX_SET_COUNT   (10)

struct IsokineticSetSettings
{
    qint32 pauseTime;
    qint32 firstInterruptionTime;
    qint32 secondInterruptionTime;
    qint32	startPoint;
    qint32	speedAB;
    qint32	speedBA;
    qint32	repsCount;
};

class WidgetExcerciseSettings : public Widget
{
    Q_OBJECT
    QLabel *lblRxMessageCounter;

    WidgetSettingsInteger *wgtSetCount;


    QGridLayout *loSetTable;
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

    IsokineticSetSettings setSettings[MAX_SET_COUNT];

    QPushButton *btnWriteSettings;

    QVBoxLayout *loMain;

    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;
    QString name_;
    bool bbWidget_;
    QSettings *settings;

public:
    explicit WidgetExcerciseSettings(
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

    void setTableEditingFinished();

};

#endif // WIDGETEXCERCISESETTINGS_H
