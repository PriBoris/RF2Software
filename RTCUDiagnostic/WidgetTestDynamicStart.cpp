#include "WidgetTestDynamicStart.h"

WidgetTestDynamicStart::WidgetTestDynamicStart(
        QString legend,
        QString registryName,
        qint32 defaultValue,
        QWidget *parent
        ) : QWidget(parent)
{


    legend_ = legend;
    registryName_ = registryName;
    defaultValue_ = defaultValue;




    btnStart = new QPushButton(legend);
    btnStart->setFont(QFont("Verdana",10,QFont::Bold,true));

    lblPause = new QLabel("пауза перед тестом:");
    lblPause->setFont(QFont("Verdana",10,QFont::Normal,false));

    edtPause = new QLineEdit();
    edtPause->setFont(QFont("Verdana",10,QFont::Normal,false));

    lo = new QHBoxLayout;

    btnStart->setFixedWidth(200);
    edtPause->setFixedWidth(150);

    lo->addWidget(btnStart);
    lo->addWidget(lblPause);
    lo->addWidget(edtPause);
    lo->addStretch(1);
    this->setLayout(lo);

    settings = new QSettings();
    pauseValue_ = settings->value(registryName_, defaultValue_).toInt();
    edtPause->setText(QString::number(pauseValue_));
    connect(edtPause,SIGNAL(editingFinished()),SLOT(pauseEditingFinished()));

    connect(btnStart,SIGNAL(clicked()),SLOT(startButtonClicked()));



}


void WidgetTestDynamicStart::pauseEditingFinished()
{
    //qDebug() << "WidgetTestStart::pauseEditingFinished";
    pauseValue_ = edtPause->text().toInt();
    settings->setValue(registryName_,pauseValue_);

}

void WidgetTestDynamicStart::startButtonClicked()
{
    //qDebug() << "WidgetTestStart::startButtonClicked";
    emit startButtonClicked(pauseValue_);



}

