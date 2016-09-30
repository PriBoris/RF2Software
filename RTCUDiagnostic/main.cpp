#include "widgetMain.h"
#include "mainscrollarea.h"

#include <QApplication>


#include <QtSerialPort/QSerialPort>
#include <QScrollArea>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("REFORCE");
    QCoreApplication::setApplicationName("RTCUDiagnostic");




    WidgetMain *mainWidget = new WidgetMain;
    MainScrollArea *scrollArea=new MainScrollArea;
    scrollArea->setAttribute( Qt::WA_DeleteOnClose );
    mainWidget->setAttribute( Qt::WA_DeleteOnClose );

    scrollArea->setWidget(mainWidget);
    scrollArea->show();

    return a.exec();
}
