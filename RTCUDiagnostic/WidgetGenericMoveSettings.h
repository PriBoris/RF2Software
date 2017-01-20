#ifndef WIDGETSETTINGSINTEGERPAIR_H
#define WIDGETSETTINGSINTEGERPAIR_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QBoxLayout>
#include <QValidator>
#include <QSettings>



class WidgetGenericMoveSettings : public QWidget{

	Q_OBJECT

	QLabel *lblLegend;
	QLineEdit *edtWriteValue1;
	QLineEdit *edtWriteValue2;
	QLabel *lblReadValue1;
	QLabel *lblReadValue2;
	QHBoxLayout *lo;

	QString legend_;
	QString registryName_;

	QSettings *settings;
	qint32 value1;
	qint32 value2;
	
public:

    explicit WidgetGenericMoveSettings(
			QString legend,
			QString registryName,
			qint32 defaultValue1,
			qint32 defaultValue2,
			QWidget *parent = 0
			);	

    qint32 getDestinationPositionWriteValue();
    qint32 getSpeedWriteValue();

    void setReadValue(qint32 newDestinationPositionValue, qint32 newSpeedValue);
    void setUnknownReadValues();
    void setValidators(QValidator *validatorDestinationPosition, QValidator *validatorSpeed);    


signals:

public slots:

    void editingFinished();

};

#endif // WIDGETSETTINGSINTEGERPAIR_H
