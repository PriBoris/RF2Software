#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QDebug>
#include <QBoxLayout>
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

    QVector<QWidget*> widgetArray;

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



};

#endif // MAINWINDOW_H
