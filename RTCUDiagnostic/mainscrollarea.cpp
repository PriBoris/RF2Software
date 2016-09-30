#include "mainscrollarea.h"

#include <QSettings>
#include <QDebug>

MainScrollArea::MainScrollArea(QWidget *parent) : QScrollArea(parent)
{

    restoreGeometry();
    qDebug() << "MainScrollArea ctor()";

}

MainScrollArea::~MainScrollArea(){


    saveGeometry();
    qDebug() << "MainScrollArea dtor()";

}

void MainScrollArea::saveGeometry(){

    //qDebug() << "MainWindow::saveGeometry()";


    QPoint position = this->pos();
    QRect rect = this->geometry();
    QSettings settings;
    settings.setValue("/Settings/windowX",QString::number(rect.x()));
    settings.setValue("/Settings/windowY",QString::number(rect.y()));
    settings.setValue("/Settings/windowMaximized",((this->windowState() & Qt::WindowMaximized)!=0));

    settings.setValue("/Settings/windowWidth",QString::number(rect.width()));
    settings.setValue("/Settings/windowHeight",QString::number(rect.height()));




}

void MainScrollArea::restoreGeometry(){

    QSettings settings;
    if (settings.value("/Settings/windowMaximized",true).toBool()==true){
        this->setWindowState(Qt::WindowMaximized);
    } else {
        QPoint position(
            settings.value("/Settings/windowX", 100).toInt(),
            settings.value("/Settings/windowY", 100).toInt()
            );
        this->move(position);
        this->resize(
            settings.value("/Settings/windowWidth", 300).toInt(),
            settings.value("/Settings/windowHeight", 300).toInt()
            );
    }

}
