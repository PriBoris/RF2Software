#include "WidgetTestStaticStart.h"

WidgetTestStaticStart::WidgetTestStaticStart(
        QString legend,
        QString registryNamePause,
        qint32 defaultValuePause,
        QString registryNameDuration,
        qint32 defaultValueDuration,
        QString registryNamePositionRel,
        qint32 defaultValuePositionRel,
        QWidget *parent
        ) : QWidget(parent)
{


    legend_ = legend;
    registryNamePause_ = registryNamePause;
    defaultValuePause_ = defaultValuePause;
    registryNameDuration_ = registryNameDuration;
    defaultValueDuration_ = defaultValueDuration;
    registryNamePositionRel_ = registryNamePositionRel;
    defaultValuePositionRel_ = defaultValuePositionRel;

    btnStart = new QPushButton(legend);
    btnStart->setFont(QFont("Verdana",10,QFont::Bold,true));


    lblPause = new QLabel("пауза перед тестом:");
    lblPause->setFont(QFont("Verdana",10,QFont::Normal,false));

    edtPause = new QLineEdit();
    edtPause->setFont(QFont("Verdana",10,QFont::Normal,false));

    lblDuration = new QLabel("длительность теста:");
    lblDuration->setFont(QFont("Verdana",10,QFont::Normal,false));

    edtDuration = new QLineEdit();
    edtDuration->setFont(QFont("Verdana",10,QFont::Normal,false));

    lblPositionRel = new QLabel("положение теста (%*100):");
    lblPositionRel->setFont(QFont("Verdana",10,QFont::Normal,false));

    edtPositionRel = new QLineEdit();
    edtPositionRel->setFont(QFont("Verdana",10,QFont::Normal,false));


    lo = new QHBoxLayout;

    btnStart->setFixedWidth(200);
    edtPause->setFixedWidth(150);
    edtDuration->setFixedWidth(150);
    edtPositionRel->setFixedWidth(150);

    lo->addWidget(btnStart);
    lo->addWidget(lblPause);
    lo->addWidget(edtPause);
    lo->addWidget(lblDuration);
    lo->addWidget(edtDuration);
    lo->addWidget(lblPositionRel);
    lo->addWidget(edtPositionRel);
    lo->addStretch(1);
    this->setLayout(lo);

    settings = new QSettings();
    pauseValue_ = settings->value(registryNamePause_, defaultValuePause_).toInt();
    durationValue_ = settings->value(registryNameDuration_, defaultValueDuration_).toInt();
    positionRelValue_ = settings->value(registryNamePositionRel_, defaultValuePositionRel_).toInt();

    edtPause->setText(QString::number(pauseValue_));
    edtDuration->setText(QString::number(durationValue_));
    edtPositionRel->setText(QString::number(positionRelValue_));

    connect(edtPause,SIGNAL(editingFinished()),SLOT(pauseEditingFinished()));
    connect(edtDuration,SIGNAL(editingFinished()),SLOT(durationEditingFinished()));
    connect(edtPositionRel,SIGNAL(editingFinished()),SLOT(positionRelEditingFinished()));

    connect(btnStart,SIGNAL(clicked()),SLOT(startButtonClicked()));

}

void WidgetTestStaticStart::pauseEditingFinished(){
    pauseValue_ = edtPause->text().toInt();
    settings->setValue(registryNamePause_,pauseValue_);
}

void WidgetTestStaticStart::durationEditingFinished(){

    durationValue_ = edtDuration->text().toInt();
    settings->setValue(registryNameDuration_,durationValue_);
}

void WidgetTestStaticStart::positionRelEditingFinished(){

    positionRelValue_ = edtPositionRel->text().toInt();
    settings->setValue(registryNamePositionRel_,positionRelValue_);
}



void WidgetTestStaticStart::startButtonClicked(){

    QByteArray testSettings;

    testSettings.append((const char*)&pauseValue_,sizeof(pauseValue_));
    testSettings.append((const char*)&durationValue_,sizeof(durationValue_));
    testSettings.append((const char*)&positionRelValue_,sizeof(positionRelValue_));

    emit startButtonClicked(testSettings);

}



