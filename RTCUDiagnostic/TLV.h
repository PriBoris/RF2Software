#ifndef TLV_H
#define TLV_H

#include <QObject>
#include <QDebug>

class TLV{

protected:

    static quint32 crcTable_[256];
    static bool crcTableCreated;
    static void createCrcTable(void);
    static void crc32(quint32 *pdwCrc32,quint8 bNewByte);



public:



    static const quint8 TAG_ReportCurrentMode = 0x00;
    static const quint8 TAG_ReportPersonalSettings = 0x01;
    static const quint8 TAG_ReportIsokineticExcerciseSettings = 0x02;
    static const quint8 TAG_ReportMachineSettings = 0x03;
    static const quint8 TAG_RfidProximity = 0x04;
    static const quint8 TAG_RtcuDebugMessage = 0x05;
    static const quint8 TAG_ReportGenericExcerciseSettings = 0x06;
    static const quint8 TAG_ReportMachineSettingsExtended = 0x07;
    static const quint8 TAG_ReportServoMode = 0x08;


    static const quint8 TAG_EnableServo = 0x40;
    static const quint8 TAG_LoadPersonalSettings = 0x41;
    static const quint8 TAG_LoadIsokineticExcerciseSettings = 0x42;
    static const quint8 TAG_LoadMachineSettings = 0x43;
    static const quint8 TAG_Parking = 0x44;
    static const quint8 TAG_Personal = 0x45;
    static const quint8 TAG_PersonalExit = 0x46;
    static const quint8 TAG_PersonalButtonPressed = 0x47;
    static const quint8 TAG_PersonalButtonHold = 0x48;
    static const quint8 TAG_PersonalButtonReleased = 0x49;
    static const quint8 TAG_TestConcentric = 0x4A;
    static const quint8 TAG_TestEccentric = 0x4B;
    static const quint8 TAG_ExcerciseIsokinetic = 0x4C;
    static const quint8 TAG_ResetError = 0x4D;
    static const quint8 TAG_Cancel = 0x4E;
    static const quint8 TAG_TestStatic = 0x4F;
    static const quint8 TAG_LoadGenericExcerciseSettings = 0x50;
    static const quint8 TAG_GenericExcerciseIsokinetic = 0x51;
    static const quint8 TAG_LoadMachineSettingsExtended = 0x52;




    static const quint8 SLIP_END = 0xC0;
    static const quint8 SLIP_ESC = 0xDB;
    static const quint8 SLIP_ESC_END = 0xDC;
    static const quint8 SLIP_ESC_ESC = 0xDD;



    static qint32 getInt32(QByteArray &array,int offset);
    static quint32 getUint32(QByteArray &array,int offset);
    static QString getDateTimeStr(QByteArray &array,int offset,bool showDate=true);
    static float getFloat(QByteArray &array,int offset);



};

#endif // TLV_H
