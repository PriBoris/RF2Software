

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

typedef enum
{
	STUFF_Waiting,
	STUFF_EscByte, 
	STUFF_NewByte
} TStuffState; 

typedef enum
{
	PROTOCOL_TagExpected,//TODO: insert stopper state
	PROTOCOL_IDByte1Expected,
	PROTOCOL_IDByte2Expected,
	PROTOCOL_IDByte3Expected,
	PROTOCOL_IDByte4Expected,
	PROTOCOL_LenByte1Expected,
	PROTOCOL_LenByte2Expected,
	PROTOCOL_ValueByteExpected,
	PROTOCOL_CrcByte1Expected,
	PROTOCOL_CrcByte2Expected,
	PROTOCOL_CrcByte3Expected,
	PROTOCOL_CrcByte4Expected,
	PROTOCOL_Error,
	PROTOCOL_Success,
	
	
} TProtocolState;





class Protocol
{

private:

		uint8_t *txBuffer;
		uint32_t txBufferLen;
		uint32_t txBufferLenMinus1;
		uint32_t txPtrPending;
		uint32_t txPtrTransmitted;

		void pushByte(uint8_t byte,bool pushStuffed,uint32_t *crc);
		void pushWord(uint16_t word,bool pushStuffed,uint32_t *crc);
		void pushDword(uint32_t dword,bool pushStuffed,uint32_t *crc);

		uint8_t *rxBuffer;
		uint32_t rxBufferLen;
		uint32_t rxBufferLenMinus1;
		uint32_t rxPtrReceived;
		uint32_t rxPtrProcessed;

		uint32_t rxValueLen;
	
	
		TStuffState stuffState;
		TProtocolState protocolState;
	
	
		uint32_t txID;
	
		static const uint8_t SLIP_END = 0xC0;
		static const uint8_t SLIP_ESC = 0xDB;
		static const uint8_t SLIP_ESC_END = 0xDC;
		static const uint8_t SLIP_ESC_ESC = 0xDD;	
	
		uint32_t rxByteCounter;
		
	public:
	
		uint8_t *rxValue;

	
		void init(
			uint8_t *txBuffer,
			uint32_t txBufferLen,
			uint8_t *rxBuffer,
			uint32_t rxBufferLen,
			uint8_t *rxValue,
			uint32_t rxValueLen
			);
	
		void sendPacket(uint8_t tag,uint8_t *data,uint32_t dataLen);

		void receiveByte(uint8_t byte);
		
		bool processRx(void);
		void getRxValue(uint8_t *dest);
		uint8_t getRxTag();
		uint32_t getRxID();
		uint16_t getRxDataLen();
		
		bool bytesPending();
		uint8_t popTxByte();
	
		uint8_t rxTag;
		uint32_t rxID;
		uint32_t rxDataLen;
		

		
		static const uint8_t TAG_ReportCurrentMode = 0x00;
		static const uint8_t TAG_ReportPersonalSettings = 0x01;
		static const uint8_t TAG_ReportExcerciseSettings = 0x02;
		static const uint8_t TAG_ReportMachineSettings = 0x03;
		static const uint8_t TAG_RfidProximity = 0x04;
		static const uint8_t TAG_RtcuDebugMessage = 0x05;
		static const uint8_t TAG_ReportGenericExcerciseSettings = 0x06;
		static const uint8_t TAG_ReportExtendedMachineSettings = 0x07;
		

		static const uint8_t TAG_EnableServo = 0x40;
		static const uint8_t TAG_LoadPersonalSettings = 0x41;
		static const uint8_t TAG_LoadExcerciseSettings = 0x42;
		static const uint8_t TAG_LoadMachineSettings = 0x43;
		static const uint8_t TAG_Parking = 0x44;
		static const uint8_t TAG_Personal = 0x45;
		static const uint8_t TAG_PersonalExit = 0x46;
		static const uint8_t TAG_PersonalButtonPressed = 0x47;
		static const uint8_t TAG_PersonalButtonHold = 0x48;
		static const uint8_t TAG_PersonalButtonReleased = 0x49;
		static const uint8_t TAG_TestConcentric = 0x4A;
		static const uint8_t TAG_TestEccentric = 0x4B;
		static const uint8_t TAG_ExcerciseIsokinetic = 0x4C;
		static const uint8_t TAG_ResetError = 0x4D;
		static const uint8_t TAG_Cancel = 0x4E;
		static const uint8_t TAG_TestStatic = 0x4F;
		static const uint8_t TAG_LoadGenericExcerciseSettings = 0x50;
		static const uint8_t TAG_GenericExcerciseIsokinetic = 0x51;
		static const uint8_t TAG_LoadExtendedMachineSettings = 0x52;


		static const uint8_t TAG_CheckRfidProximity = 0x80;



};


#endif
