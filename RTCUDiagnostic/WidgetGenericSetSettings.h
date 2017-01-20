#ifndef WIDGETGENERICSETSETTINGS_H
#define WIDGETGENERICSETSETTINGS_H

#include <QWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>


#include "serialporttransceiver.h"
#include "TLV.h"
#include "tlvwriter.h"
#include "WidgetSettingsInteger.h"
#include "WidgetGenericMoveSettings.h"



class WidgetGenericSetSettings : public QWidget{

	Q_OBJECT

	QLabel *lblRxMessageCounter;

	QLabel *lblPause1Title;
	QLabel *lblPause2Title;
	QLabel *lblStartPositionRelTitle;
	QLabel *lblMoveCountTitle;

	WidgetSettingsInteger *wgtPause1;
	WidgetSettingsInteger *wgtPause2;
	WidgetSettingsInteger *wgtStartPositionRel;
	WidgetSettingsInteger *wgtMoveCount;


	static const int MOVE_COUNT_MAX = 50;
	QVector<WidgetGenericMoveSettings*> wgtsMove;

    QPushButton *btnWriteSettings;





	QVBoxLayout *loMain;

	SerialPortTransceiver *serialPortTransceiver_;
	quint32 rxMessageCounter;

public:
	explicit WidgetGenericSetSettings(
		SerialPortTransceiver *serialPortTransceiver,
		QWidget *parent = 0
		);

signals:

public slots:
	void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);

	void slotMoveCountUpdated();

	void slotWriteSettings();


};

#endif // WIDGETGENERICSETSETTINGS_H
