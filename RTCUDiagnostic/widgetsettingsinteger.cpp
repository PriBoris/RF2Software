#include "widgetSettingsInteger.h"


WidgetSettingsInteger::WidgetSettingsInteger(
        QString legend,
        QString registryName,
        qint32 defaultValue,
        QWidget *parent
        ) : QWidget(parent)
{
    legend_ = legend;
    registryName_ = registryName;

    lblLegend = new QLabel(legend_);
    lblLegend->setFixedWidth(100);

    edtWriteValue = new QLineEdit;
    edtWriteValue->setFixedWidth(100);
    edtWriteValue->setValidator(new QIntValidator(0,99999));

    lblReadValue = new QLabel("...");
    lblReadValue->setFixedWidth(100);

    lo = new QHBoxLayout;
    lo->addWidget(lblLegend);
    lo->addWidget(edtWriteValue);
    lo->addWidget(lblReadValue);
    lo->addStretch(1);
    lo->setContentsMargins(0,0,0,0);

    this->setLayout(lo);


    settings = new QSettings();
    value = settings->value(registryName_, defaultValue).toInt();
    edtWriteValue->setText(QString::number(value));
    connect(edtWriteValue,SIGNAL(editingFinished()),SLOT(editingFinished()));





}

void WidgetSettingsInteger::editingFinished()
{
//    qDebug() << "WidgetSettingsInteger::editingFinished";

    value = edtWriteValue->text().toInt();
    settings->setValue(registryName_,value);


}



