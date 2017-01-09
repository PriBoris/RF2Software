#include "WidgetSettingsInteger.h"

//=================================================================================================
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
    lblLegend->setFixedWidth(200);
    lblLegend->setFont(QFont("Verdana",10,QFont::Normal,false));


    edtWriteValue = new QLineEdit;
    edtWriteValue->setFixedWidth(200);
    //edtWriteValue->setValidator(new QIntValidator(0,99999));
    edtWriteValue->setFont(QFont("Verdana",10,QFont::Normal,false));

    lblReadValue = new QLabel("unknown");
    lblReadValue->setFixedWidth(200);
    lblReadValue->setFont(QFont("Verdana",10,QFont::Normal,false));
    lblReadValue->setStyleSheet("border: 1px solid");    

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
//=================================================================================================
void WidgetSettingsInteger::editingFinished(){

    value = edtWriteValue->text().toInt();
    settings->setValue(registryName_,value);


}
//=================================================================================================
qint32 WidgetSettingsInteger::getWriteValue() {
    return edtWriteValue->text().toInt();
};
//=================================================================================================
void WidgetSettingsInteger::setReadValue(qint32 newValue) {
    value = newValue;
    lblReadValue->setText(QString::number(value));
};
//=================================================================================================
void WidgetSettingsInteger::clearReadValue() {
    lblReadValue->setText("unknown");
}
//=================================================================================================
void WidgetSettingsInteger::setUnknownReadValue() {
    lblReadValue->setText("unknown");
}
//=================================================================================================
void WidgetSettingsInteger::setReadOnly(){
    edtWriteValue->setEnabled(false);
}
//=================================================================================================
void WidgetSettingsInteger::setValidator(QValidator *validator){
    edtWriteValue->setValidator(validator);
}
//=================================================================================================


