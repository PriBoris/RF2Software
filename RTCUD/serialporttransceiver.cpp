#include "serialporttransceiver.h"


//========================================================================================================
SerialPortTransceiver::SerialPortTransceiver(QSerialPort *serialPort,QObject *parent) : QObject(parent)
{

    serialPort_ = serialPort;

    connect(serialPort_, SIGNAL(bytesWritten(qint64)), SLOT(handleBytesWritten(qint64)));
    connect(serialPort_, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serialPort_, SIGNAL(readyRead()), SLOT(handleReadyRead()));

}
//========================================================================================================
void SerialPortTransceiver::handleBytesWritten(qint64 bytes)
{
    bytesWritten_ += bytes;

}
//========================================================================================================
void SerialPortTransceiver::handleError(QSerialPort::SerialPortError serialPortError)
{
    qDebug() << "SerialPortTransceiver::handleError";
}
//========================================================================================================
void SerialPortTransceiver::write(const QByteArray &writeData)
{
   writeData_ = writeData;

    qint64 bytesWritten = serialPort_->write(writeData);

    if (bytesWritten == -1) {
        //qDebug() << QObject::tr("Failed to write the data to port %1, error: %2").arg(serialPort_->portName()).arg(serialPort_->errorString());
    } else if (bytesWritten != writeData_.size()) {
        //qDebug() << QObject::tr("Failed to write all the data to port %1, error: %2").arg(serialPort_->portName()).arg(serialPort_->errorString());
    }

}
//========================================================================================================
void SerialPortTransceiver::handleReadyRead()
{
    readData_.append(serialPort_->readAll());
    emit newReadData(readData_);
}
//========================================================================================================


