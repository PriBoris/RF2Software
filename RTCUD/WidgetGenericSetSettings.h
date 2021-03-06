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

#include "ReportLogger.h"

#include "qcustomplot.h"



class WidgetGenericSetSettings : public QWidget{

	Q_OBJECT

	QLabel *lblRxMessageCounter;
	QLabel *lblTxMessageCounter;

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

    QCustomPlot* plotPositionVsTime;




	QVBoxLayout *loMain;

	SerialPortTransceiver *serialPortTransceiver_;
	quint32 rxMessageCounter;
	quint32 txMessageCounter;

	bool checkMessageLength(int messageLength);
	qint32 getMoveCountFromLength(int messageLength);

	struct Move{
		qint32 destinationPositionRel;
		qint32 duration;
	};

	struct Set{
		qint32 pause1;
		qint32 pause2;
		qint32 startPositionRel;
		qint32 moveCount;
		Move moves[MOVE_COUNT_MAX];
	};

    ReportLogger *reportLogger;


public:
	explicit WidgetGenericSetSettings(
		SerialPortTransceiver *serialPortTransceiver,
		QWidget *parent = 0
		);
    ~WidgetGenericSetSettings();

signals:

public slots:
	void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);

	void slotMoveCountUpdated();

	void slotWriteSettings();

	void slotEditingFinished();

};

#endif // WIDGETGENERICSETSETTINGS_H
