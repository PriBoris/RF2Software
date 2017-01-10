#ifndef WIDGETSERVOMODE_H
#define WIDGETSERVOMODE_H

#include <QWidget>
#include <QLabel>
#include <QBoxLayout>

#include "serialporttransceiver.h"
#include "tlv.h"


class WidgetServoMode : public QWidget{
    Q_OBJECT

    QLabel *lblRxMessageCounter;
    QLabel *lblMainTickID;
    QLabel *lblDateTime;
    QLabel *lblMode;
    QLabel *lblSubmode;

    QVBoxLayout *loMain;

    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;


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
