

#include "WidgetConsole.h"

#include <QDebug>
#include <QDateTime>

#include "TLV.h"

#include "Utils.h"

//==============================================================================================================
WidgetConsole::WidgetConsole(
        SerialPortTransceiver *serialPortTransceiver,
        QWidget *parent
        ) : QWidget(parent)
{

    serialPortTransceiver_ = serialPortTransceiver;

    {
        reportLogger = new ReportLogger("RtcuDebugMessage.txt");
        (reportLogger->stream) << "\n";    
    }

    {
        lblRxMessageCounter = new QLabel;
        lblRxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));
        Utils::MessageCounterInitialize("Rx",rxMessageCounter,lblRxMessageCounter);
    }


    edtReceived = new QTextEdit;
    edtReceived->setReadOnly(true);
    edtReceived->setStyleSheet("font: 9pt \"Courier\";");
    edtReceived->setFixedHeight(400);



    loMain = new QVBoxLayout;
    loMain->addWidget(lblRxMessageCounter);
    loMain->addWidget(edtReceived);
    loMain->addStretch(1);

    this->setLayout(loMain);


}
//==============================================================================================================
void WidgetConsole::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){

    if (tag==TLV::TAG_RtcuDebugMessage){

        Utils::MessageCounterIncrement("Rx",rxMessageCounter,lblRxMessageCounter,value);

        QDateTime now = QDateTime::currentDateTime();
        QString nowStr = now.toString("yyyy/MM/dd hh:mm:ss.zzz");
        QString consoleStr = nowStr + ": " + QString(value) + "\n";;

        edtReceived->moveCursor (QTextCursor::End);
        edtReceived->insertPlainText (consoleStr);
        edtReceived->moveCursor (QTextCursor::End);

        (reportLogger->stream) << consoleStr;

        reportLogger->flush(true);

    }



}
//==============================================================================================================
WidgetConsole::~WidgetConsole(){

    delete reportLogger;
}
//==============================================================================================================






