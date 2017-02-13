#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QDebug>
#include <QBoxLayout>
#include <QGridLayout>
#include <QVector>
#include <QGroupBox>
#include <QPushButton>
#include <QSettings>

#include "tlvreader.h"
#include "serialporttransceiver.h"
#include "WidgetMode.h"
#include "WidgetServoMode.h"
#include "WidgetPersonalSettings.h"
#include "WidgetMachineSettings.h"
#include "WidgetMachineSettingsExtended.h"
#include "WidgetExcerciseSettings.h"
#include "WidgetGenericSetSettings.h"
#include "WidgetNFC.h"
#include "WidgetConsole.h"



class WidgetMain : public QWidget{

	Q_OBJECT


	WidgetMode *widgetMode;
	WidgetServoMode *widgetServoMode;
	WidgetPersonalSettings *widgetSettingsPosition;
	WidgetMachineSettings *widgetMachineSettings;
	WidgetMachineSettingsExtended *widgetMachineSettingsExtended;
	WidgetExcerciseSettings *widgetExcerciseSettings;
	WidgetGenericSetSettings *widgetGenericSetSettings;
	WidgetNFC *widgetNFC;
	WidgetConsole *widgetConsole;

	QPushButton *btnTabMode;
	QPushButton *btnTabServoMode;
	QPushButton *btnTabPersonalSettings;
	QPushButton *btnTabMachineSettings;
	QPushButton *btnTabExcerciseSettings;
	QPushButton *btnTabGenericSetSettings;
	QPushButton *btnTabNFC;
	QPushButton *btnTabConsole;
	QGridLayout *loTabs;


	QLabel *lblPortStatus;
	QLabel *lblPortRxStats;
	QLabel *lblPortRxErrors;
	QLabel *lblMode;
	QVBoxLayout *loPort;
	quint32 rxMessageCounter;
	quint32 rxMessageMaxLength;
	quint32 rxErrorCounter;


	QHBoxLayout *loHeader;




	QFont *fontRegular;
	QFont *fontSelect;

	QVector<QWidget*> widgetArray;

	QSerialPort *serialPort;
	SerialPortTransceiver *serialPortTransceiver;
	TLVReader *tlvReader;

	QVBoxLayout *lo;


	static const int TAB_Mode=0;
	static const int TAB_ServoMode=1;
	static const int TAB_MachineSettings=2;
	static const int TAB_PersonalSettings=3;
	static const int TAB_ExcerciseSettings=4;
	static const int TAB_GenericSetSettings=5;
	static const int TAB_NFC=6;
	static const int TAB_CONSOLE=7;

	void saveGeometry();
	void restoreGeometry();

	QWidget *container;
	QScrollArea *scrollArea;
	QVBoxLayout *loContainer;

public:
	explicit WidgetMain(QWidget *parent = 0);
	~WidgetMain();

signals:

public slots:

	void slotTabClicked(int tabIndex);
	void newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value);
	void errorDetected(void);


};

#endif // MAINWINDOW_H
