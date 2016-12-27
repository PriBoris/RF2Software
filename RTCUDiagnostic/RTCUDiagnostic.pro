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
    tlv.cpp \
    tlvwriter.cpp \
    tlvreader.cpp \
    personalbutton.cpp \
    widgetMode.cpp \
    widgetMachineSettings.cpp \
    widgetSettingsInteger.cpp \
    widgetPersonalSettings.cpp \
    widgetExcerciseSettings.cpp \
    widgetConsole.cpp \
    widgetMain.cpp \
    qcustomplot.cpp \
    widgetTestDynamicStart.cpp \
    widgetTestStaticStart.cpp \
    ModeLogger.cpp \
    widgetNFC.cpp

HEADERS  += \
    serialporttransceiver.h \
    tlv.h \
    tlvwriter.h \
    tlvreader.h \
    personalbutton.h \
    widgetMode.h \
    widgetMachineSettings.h \
    widgetSettingsInteger.h \
    widgetPersonalSettings.h \
    widgetExcerciseSettings.h \
    widgetConsole.h \
    widgetMain.h \
    qcustomplot.h \
    widgetTestDynamicStart.h \
    widgetTestStaticStart.h \
    ModeLogger.h \
    widgetNFC.h
