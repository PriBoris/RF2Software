#include "WidgetGenericMoveSettings.h"

WidgetGenericMoveSettings::WidgetGenericMoveSettings(
	QString legend,
	QString registryName,
	qint32 defaultValue1,
	qint32 defaultValue2,
	QWidget *parent
	) : QWidget(parent){

	legend_ = legend;
	registryName_ = registryName;

	lblLegend = new QLabel(legend_);
	lblLegend->setFixedWidth(100);
	lblLegend->setFont(QFont("Verdana",10,QFont::Normal,false));

	{
		edtWriteValue1 = new QLineEdit;
		edtWriteValue1->setFixedWidth(150);
		edtWriteValue1->setFont(QFont("Verdana",10,QFont::Normal,false));
		edtWriteValue2 = new QLineEdit;
		edtWriteValue2->setFixedWidth(150);
		edtWriteValue2->setFont(QFont("Verdana",10,QFont::Normal,false));
	}

	{
		lblReadValue1 = new QLabel("unknown");
		lblReadValue1->setFixedWidth(150);
		lblReadValue1->setFont(QFont("Verdana",10,QFont::Normal,false));
		lblReadValue1->setStyleSheet("border: 1px solid");

		lblReadValue2 = new QLabel("unknown");
		lblReadValue2->setFixedWidth(150);
		lblReadValue2->setFont(QFont("Verdana",10,QFont::Normal,false));
		lblReadValue2->setStyleSheet("border: 1px solid");
	}


	lo = new QHBoxLayout;
	lo->addWidget(lblLegend);
	lo->addWidget(edtWriteValue1);
	lo->addWidget(lblReadValue1);
	lo->addSpacing(10);
	lo->addWidget(edtWriteValue2);
	lo->addWidget(lblReadValue2);
	lo->addSpacing(10);
	lo->addStretch(1);
	lo->setContentsMargins(0,0,0,0);

	this->setLayout(lo);


	settings = new QSettings();
    value1 = settings->value(registryName_+"DestinationPosition", defaultValue1).toInt();
    value2 = settings->value(registryName_+"Speed", defaultValue2).toInt();
	edtWriteValue1->setText(QString::number(value1));
	edtWriteValue2->setText(QString::number(value2));
	connect(edtWriteValue1,SIGNAL(editingFinished()),SLOT(editingFinished()));
	connect(edtWriteValue2,SIGNAL(editingFinished()),SLOT(editingFinished()));



}
//=================================================================================================
void WidgetGenericMoveSettings::editingFinished(){

	value1 = edtWriteValue1->text().toInt();
	value2 = edtWriteValue2->text().toInt();
	settings->setValue(registryName_+"DestinationPosition",value1);
	settings->setValue(registryName_+"Speed",value2);
}
//=================================================================================================
qint32 WidgetGenericMoveSettings::getDestinationPositionWriteValue(){
	return edtWriteValue1->text().toInt();
}
//=================================================================================================
qint32 WidgetGenericMoveSettings::getSpeedWriteValue(){
	return edtWriteValue1->text().toInt();
}
//=================================================================================================
void WidgetGenericMoveSettings::setReadValue(qint32 newDestinationPositionValue, qint32 newSpeedValue){
	value1 = newDestinationPositionValue;
	value2 = newSpeedValue;
	lblReadValue1->setText(QString::number(value1));   
	lblReadValue2->setText(QString::number(value2));   
}
//=================================================================================================
void WidgetGenericMoveSettings::setUnknownReadValues(){
	lblReadValue1->setText("unknown dest pos");
	lblReadValue2->setText("unknown speed");
}
//=================================================================================================
void WidgetGenericMoveSettings::setValidators(QValidator *validatorDestinationPosition, QValidator *validatorSpeed){
	edtWriteValue1->setValidator(validatorDestinationPosition);	
    edtWriteValue2->setValidator(validatorSpeed);
}
//=================================================================================================


