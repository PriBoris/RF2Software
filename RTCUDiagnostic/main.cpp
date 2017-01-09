#include "WidgetMain.h"

#include <QApplication>


#include <QtSerialPort/QSerialPort>
#include <QScrollArea>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("REFORCE");
    QCoreApplication::setApplicationName("RTCUDiagnostic");

    WidgetMain *widgetMain = new WidgetMain;

    widgetMain->show();

    return a.exec();
}
