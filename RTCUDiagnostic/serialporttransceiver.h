#ifndef SERIALPORTTRANSCEIVER_H
#define SERIALPORTTRANSCEIVER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QtSerialPort/QSerialPort>

class SerialPortTransceiver : public QObject
{
    Q_OBJECT

    QSerialPort     *serialPort_;
    QByteArray      writeData_;
    QByteArray      readData_;
    QTextStream     standardOutput_;
    qint64          bytesWritten_;
    //QTimer          txTimer_;
    //QTimer          rxTimer_;

public:
    explicit SerialPortTransceiver(QSerialPort *serialPort,QObject *parent = 0);
    void write(const QByteArray &writeData);
    bool isPortOK(){return serialPort_->isOpen();};


private slots:

    void handleBytesWritten(qint64 bytes);
    //void handleTxTimeout();
    void handleError(QSerialPort::SerialPortError error);
    void handleReadyRead();
    //void handleRxTimeout();


signals:

    void newReadData(QByteArray &readData);

public slots:
};

#endif // SERIALPORTTRANSCEIVER_H
