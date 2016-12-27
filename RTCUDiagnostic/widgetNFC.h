#ifndef WIDGETNFC_H
#define WIDGETNFC_H

#include <QWidget>
#include <QDebug>
#include <QLabel>
#include <QBoxLayout>
#include <QSignalMapper>

#include "serialporttransceiver.h"
#include "tlv.h"
#include "tlvwriter.h"



class WidgetNFC : public QWidget
{
    Q_OBJECT

    QLabel *lblRxMessageCounter;
    QLabel *lblUID;

    QVBoxLayout *loMain;

    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;


public:
    explicit WidgetNFC(
            SerialPortTransceiver *serialPortTransceiver,
            QWidget *parent = 0
            );

signals:

public slots:

    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);

};

#endif // WIDGETNFC_H
