#ifndef WIDGETCONSOLE_H
#define WIDGETCONSOLE_H

#include <QWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QTextEdit>


#include "serialporttransceiver.h"
#include "ReportLogger.h"


class WidgetConsole : public QWidget{

    Q_OBJECT
    QLabel *lblRxMessageCounter;

    QTextEdit *edtReceived;

    QVBoxLayout *loMain;

    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;


    ReportLogger *reportLogger;


public:
    explicit WidgetConsole(
            SerialPortTransceiver *serialPortTransceiver,
            QWidget *parent = 0
            );
    ~WidgetConsole();

public slots:
    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);

};

#endif // WIDGETCONSOLE_H
