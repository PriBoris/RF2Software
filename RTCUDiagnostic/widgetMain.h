#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QDebug>
#include <QBoxLayout>
#include <QVector>
#include <QGroupBox>
#include <QPushButton>
#include <QSettings>

#include "widget.h"
#include "tlvreader.h"
#include "serialporttransceiver.h"
#include "widgetMode.h"
#include "widgetPersonalSettings.h"
#include "widgetMachineSettings.h"
#include "widgetExcerciseSettings.h"
#include "widgetnfc.h"
#include "widgetConsole.h"



class WidgetMain : public QWidget
{
    Q_OBJECT


    WidgetMode *widgetHmiMode;
    WidgetPersonalSettings *widgetSettingsPosition;
    WidgetMachineSettings *widgetMachineSettings;
    WidgetExcerciseSettings *widgetExcerciseSettings;
    WidgetNFC *widgetNFC;
    WidgetConsole *widgetConsole;

    QLabel *lblPortStatus;
    QPushButton *btnTabMode;
    QPushButton *btnTabPersonalSettings;
    QPushButton *btnTabMachineSettings;
    QPushButton *btnTabExcerciseSettings;
    QPushButton *btnTabNFC;
    QPushButton *btnTabConsole;
    QHBoxLayout *loTabs;

    QFont *fontRegular;
    QFont *fontSelect;

    QVector<Widget*> widgetArray;
    QVector<QGroupBox*> widgetGroupBoxArray;
    QVector<QVBoxLayout*> widgetGroupBoxLayoutArray;

    QSerialPort *serialPort;
    SerialPortTransceiver *serialPortTransceiver;
    TLVReader *tlvReader;

    QVBoxLayout *lo;

    static const int TAB_Mode=0;
    static const int TAB_MachineSettings=1;
    static const int TAB_PersonalSettings=2;
    static const int TAB_ExcerciseSettings=3;
    static const int TAB_NFC=4;
    static const int TAB_CONSOLE=5;


public:
    explicit WidgetMain(QWidget *parent = 0);
    //~WidgetMain();

signals:

public slots:

    void slotTabClicked(int tabIndex);



};

#endif // MAINWINDOW_H
