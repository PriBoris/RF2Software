#ifndef WIDGETCONSOLE_H
#define WIDGETCONSOLE_H

#include <QWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QTextEdit>
#include <QTimer>
#include <QFile>


#include "widget.h"
#include "serialporttransceiver.h"


class WidgetConsole : public Widget
{
    Q_OBJECT
    QLabel *lblRxMessageCounter;

    QTextEdit *edtReceived;

    QVBoxLayout *loMain;

    //QTimer *debugTimer;

    SerialPortTransceiver *serialPortTransceiver_;
    quint32 rxMessageCounter;
    QString name_;
    bool bbWidget_;

    QString getRandomString() const;
    QFile *logFile;

public:
    explicit WidgetConsole(SerialPortTransceiver *serialPortTransceiver,QString name,bool bbWidget,QWidget *parent = 0);
    QString getName(){return name_;}
    bool isBBWidget(){return bbWidget_;}
    ~WidgetConsole();

public slots:
    void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);
    void debugTimerTimeout(void);




};

#endif // WIDGETCONSOLE_H
