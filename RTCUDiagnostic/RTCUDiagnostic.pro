#-------------------------------------------------
#
# Project created by QtCreator 2015-08-02T23:50:43
#
#-------------------------------------------------

QT       += core gui
QT += serialport
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RTCUDiagnostic
TEMPLATE = app


SOURCES += main.cpp\
    serialporttransceiver.cpp \
    tlvwriter.cpp \
    tlvreader.cpp \
    personalbutton.cpp \
    qcustomplot.cpp \
    WidgetMachineSettingsExtended.cpp \
    WidgetMachineSettings.cpp \
    WidgetSettingsInteger.cpp \
    WidgetSettingsFloat.cpp \
    WidgetMode.cpp \
    WidgetTestDynamicStart.cpp \
    WidgetTestStaticStart.cpp \
    WidgetNFC.cpp \
    WidgetConsole.cpp \
    WidgetExcerciseSettings.cpp \
    WidgetMain.cpp \
    WidgetPersonalSettings.cpp \
    WidgetServoMode.cpp \
    RTCU.cpp \
    TLV.cpp \
    ReportLogger.cpp \
    WidgetGenericSetSettings.cpp \
    WidgetGenericMoveSettings.cpp \
    Utils.cpp

HEADERS  += \
    serialporttransceiver.h \
    tlvwriter.h \
    tlvreader.h \
    personalbutton.h \
    qcustomplot.h \
    WidgetMachineSettingsExtended.h \
    WidgetMachineSettings.h \
    WidgetSettingsInteger.h \
    WidgetSettingsFloat.h \
    WidgetMode.h \
    WidgetTestDynamicStart.h \
    WidgetTestStaticStart.h \
    WidgetNFC.h \
    WidgetExcerciseSettings.h \
    WidgetConsole.h \
    WidgetMain.h \
    WidgetPersonalSettings.h \
    WidgetServoMode.h \
    RTCU.h \
    TLV.h \
    ReportLogger.h \
    WidgetGenericSetSettings.h \
    WidgetGenericMoveSettings.h \
    Utils.h
