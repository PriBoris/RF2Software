#include "WidgetSettingsFloat.h"


//=================================================================================================
WidgetSettingsFloat::WidgetSettingsFloat(
	QString legend,
	QString registryName,
	float defaultValue,
	QWidget *parent) : QWidget(parent){

    legend_ = legend;
    registryName_ = registryName;

    lblLegend = new QLabel(legend_);
    lblLegend->setFixedWidth(200);
    lblLegend->setFont(QFont("Verdana",10,QFont::Normal,false));

    edtWriteValue = new QLineEdit;
    edtWriteValue->setFixedWidth(200);
    //edtWriteValue->setValidator(new QDoubleValidator(0.0,(double)10000.0,2));
    edtWriteValue->setFont(QFont("Verdana",10,QFont::Normal,false));

    lblReadValue = new QLabel("unknown");
    lblReadValue->setFixedWidth(200);
    lblReadValue->setFont(QFont("Verdana",10,QFont::Normal,false));

    lo = new QHBoxLayout;
    lo->addWidget(lblLegend);
    lo->addWidget(edtWriteValue);
    lo->addWidget(lblReadValue);
    lo->addStretch(1);
    lo->setContentsMargins(0,0,0,0);

    this->setLayout(lo);


    settings = new QSettings();
    value = settings->value(registryName_, defaultValue).toFloat();
    edtWriteValue->setText(QString::number(value));
    connect(edtWriteValue,SIGNAL(editingFinished()),SLOT(editingFinished()));



}
//=================================================================================================
float WidgetSettingsFloat::getWriteValue() {
    return edtWriteValue->text().toFloat();
};
//=================================================================================================
void WidgetSettingsFloat::setReadValue(float newValue) {
    value = newValue;
    lblReadValue->setText(QString::number(value));
};
//=================================================================================================
void WidgetSettingsFloat::clearReadValue() {
    lblReadValue->setText("unknown");
}
//=================================================================================================
void WidgetSettingsFloat::setUnknownReadValue() {
    lblReadValue->setText("unknown");
}
//=================================================================================================
void WidgetSettingsFloat::editingFinished(){

    value = edtWriteValue->text().toFloat();
    settings->setValue(registryName_,value);
}
//=================================================================================================
void WidgetSettingsFloat::setValidator(QValidator *validator){
    edtWriteValue->setValidator(validator);
}
//=================================================================================================
