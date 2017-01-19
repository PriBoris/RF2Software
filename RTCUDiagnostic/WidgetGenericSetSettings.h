#ifndef WIDGETGENERICSETSETTINGS_H
#define WIDGETGENERICSETSETTINGS_H

#include <QWidget>
#include <QLabel>
#include <QBoxLayout>

#include "serialporttransceiver.h"
#include "TLV.h"
#include "tlvwriter.h"



class WidgetGenericSetSettings : public QWidget
{
    Q_OBJECT

    QLabel *lblRxMessageCounter;




    QVBoxLayout *loMain;

    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;

public:
    explicit WidgetGenericSetSettings(
    	SerialPortTransceiver *serialPortTransceiver,
    	QWidget *parent = 0
    	);

signals:

public slots:
    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);


};

#endif // WIDGETGENERICSETSETTINGS_H
