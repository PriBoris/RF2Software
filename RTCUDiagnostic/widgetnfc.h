#ifndef WIDGETNFC_H
#define WIDGETNFC_H

#include <QWidget>
#include <QDebug>
#include <QLabel>
#include <QBoxLayout>
#include <QSignalMapper>

#include "widget.h"
#include "serialporttransceiver.h"
#include "tlv.h"
#include "tlvwriter.h"



class WidgetNFC : public Widget
{
    Q_OBJECT

    QLabel *lblRxMessageCounter;
    QLabel *lblUID;

    QVBoxLayout *loMain;

    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;
    QString name_;
    bool bbWidget_;


public:
    explicit WidgetNFC(SerialPortTransceiver *serialPortTransceiver,QString name,bool bbWidget,QWidget *parent = 0);
    QString getName(){return name_;}
    bool isBBWidget(){return bbWidget_;}

signals:

public slots:

    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);

};

#endif // WIDGETNFC_H
