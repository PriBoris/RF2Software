
#ifndef ODOMETER_H
#define ODOMETER_H

#include <stdint.h>

typedef struct 
{
	uint8_t year;//00-99
	uint8_t month;//01-12
	uint8_t date;//01-31
	uint8_t hours;//00-23
	uint8_t minutes;//00–59
	uint8_t seconds;//00–59
} TCurrentDateTime;//TODO: packed



class OdometerRTC{
private:



	static void rtcCsAssert();
	static void rtcCsDeassert();
	static void nvmemCsDeassert();

	static bool rxTxActive;

	static const uint32_t RX_TX_DATA_BUF_LEN = 1024;
	static uint32_t rxTxDataCount;
	static uint32_t transferLength;
	static uint8_t txData[RX_TX_DATA_BUF_LEN];
	static uint8_t rxData[RX_TX_DATA_BUF_LEN];



	static void waitWhileBusy();
	static void startRxTx();

	static const uint8_t DS3234REG_Seconds = (0x00);
	static const uint8_t DS3234REG_Minutes = (0x01);
	static const uint8_t DS3234REG_Hours = (0x02);
	static const uint8_t DS3234REG_Day = (0x03);
	static const uint8_t DS3234REG_Date = (0x04);
	static const uint8_t DS3234REG_MonthCentury = (0x05);
	static const uint8_t DS3234REG_Year = (0x06);
	static const uint8_t DS3234REG_Control = (0x0E);
	static const uint8_t DS3234REG_ControlStatus = (0x0F);
	static const uint8_t DS3234REG_CrystalAgingOffset = (0x10);
	static const uint8_t DS3234REG_TempMSB = (0x11);
	static const uint8_t DS3234REG_TempLSB = (0x12);
	static const uint8_t DS3234REG_DisableTempConversions = (0x13);
	static const uint8_t DS3234REG_SramAddress = (0x18); //256 bytes;
	static const uint8_t DS3234REG_SramData = (0x19);

	static const uint8_t DS3234CMD_WRITE =  (1<<7);
	static const uint8_t DS3234CMD_READ =  (0<<7);


	static void ds3234ReadRegisters(uint8_t startAddress,uint8_t registerCount,uint8_t* data);
	static void ds3234WriteRegisters(uint8_t startAddress,uint8_t registerCount,uint8_t* data);
	static uint8_t bcdByteToDec(uint8_t bcd);



	typedef struct  { 
		uint32_t timeSeconds;
		uint32_t distanceMms;
		uint32_t checksum;
	} Record;
	static Record record0,record1;
	static const uint8_t RECORD0_ADDRESS = 0;
	static const uint8_t RECORD1_ADDRESS = 16;

	static void readRecord(bool oddRecord);
	static bool checkRecord(bool oddRecord);
	static void saveRecord(bool oddRecord);
	static bool actualRecordIsOdd;


	static const uint32_t CHECKSUM_SEED = 0x00000000;
	static uint32_t checksumTable[256];	
	static bool checksumTableValid;
	static void checksumTableCreate(void);
	static void checksumAppendByte(uint8_t newByte,uint32_t &checksum);

	static TCurrentDateTime previousDateTime;



public:
	static void init();

	static void rxTxByteDone();


	static void readCurrentDateTime(bool updateOdometerTime);
	static TCurrentDateTime currentDateTime;

	static uint32_t getOdometerTimeMinutes();
	static uint32_t getOdometerDistanceMeters();

	static void addMovement(int32_t startAbsPosition,int32_t stopAbsPosition);



};







#endif
