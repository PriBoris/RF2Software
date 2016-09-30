

#include "widgetConsole.h"

#include <QDebug>
#include <QDateTime>

#include "tlv.h"

//==============================================================================================================
WidgetConsole::WidgetConsole(
        SerialPortTransceiver *serialPortTransceiver,
        QString name,
        bool bbWidget,
        QWidget *parent
        ) : Widget(parent)
{

    serialPortTransceiver_ = serialPortTransceiver;
    rxMessageCounter = 0;
    name_ = name;
    bbWidget_ = bbWidget;

    {
        logFile = new QFile("RtcuDebugMessagesLog.txt");
        logFile->open(QIODevice::Append);

        QString separator = "\n\n";
        logFile->write(separator.toLocal8Bit());

    }



    lblRxMessageCounter = new QLabel("lblRxMessageCounter");

    edtReceived = new QTextEdit;
    edtReceived->setReadOnly(true);
    edtReceived->setStyleSheet("font: 9pt \"Courier\";");
    edtReceived->setFixedHeight(400);



    loMain = new QVBoxLayout;
    loMain->addWidget(lblRxMessageCounter);
    loMain->addWidget(edtReceived);
    loMain->addStretch(1);

    this->setLayout(loMain);

    //debugTimer = new QTimer(this);
    //connect(debugTimer, SIGNAL(timeout()), this, SLOT(debugTimerTimeout()));
    //debugTimer->start(300);

}
//==============================================================================================================
void WidgetConsole::debugTimerTimeout(void){

    //qDebug() << "WidgetConsole::debugTimerTimeout";

    QByteArray messageData = getRandomString().toLocal8Bit();
    newMessageReceived((quint8)TLV::TAG_RtcuDebugMessage,0,messageData);

}
//==============================================================================================================
QString WidgetConsole::getRandomString() const
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   const int randomStringLength = 12; // assuming you want random strings of 12 characters

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   //randomString.append('\n');
   return randomString;
}
//==============================================================================================================
void WidgetConsole::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){

    if (tag==TLV::TAG_RtcuDebugMessage){

        rxMessageCounter++;
        lblRxMessageCounter->setText(
                    "Сообщений: "+QString::number(rxMessageCounter)+" "
                    +"(длина последнего="
                    +QString::number(value.length())
                    +")"
                    );

        QDateTime now = QDateTime::currentDateTime();
        QString nowStr = now.toString("yyyy/MM/dd hh:mm:ss.zzz");
        QString consoleStr = nowStr + ": " + QString(value) + "\n";;

        edtReceived->moveCursor (QTextCursor::End);
        edtReceived->insertPlainText (consoleStr);
        edtReceived->moveCursor (QTextCursor::End);

        if (logFile->isOpen()){

            logFile->write(consoleStr.toLocal8Bit());



        }




        //qDebug() << "WidgetConsole::newMessageReceived";


    }



}
//==============================================================================================================
WidgetConsole::~WidgetConsole(){

    qDebug() << "WidgetConsole::~WidgetConsole";
    logFile->close();
}
//==============================================================================================================






