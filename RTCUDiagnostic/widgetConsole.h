#ifndef WIDGETCONSOLE_H
#define WIDGETCONSOLE_H

#include <QWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QTextEdit>
#include <QTimer>
#include <QFile>


#include "serialporttransceiver.h"


class WidgetConsole : public QWidget
{
    Q_OBJECT
    QLabel *lblRxMessageCounter;

    QTextEdit *edtReceived;

    QVBoxLayout *loMain;


    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;

    QString getRandomString() const;
    QFile *logFile;

public:
    explicit WidgetConsole(
            SerialPortTransceiver *serialPortTransceiver,
            QWidget *parent = 0
            );
    ~WidgetConsole();

public slots:
    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);
    void debugTimerTimeout(void);




};

#endif // WIDGETCONSOLE_H
