
#ifndef SERVO_SERVO_H
#define SERVO_SERVO_H

#include <stdint.h>


class Servo{
private:

	static const uint32_t SPI_MAX_LENGTH = (256);
	static uint8_t spi2TxData[SPI_MAX_LENGTH];
	static uint8_t spi2RxData[SPI_MAX_LENGTH];
	static uint32_t spi2RxTxLength;
	static uint32_t spi2RxTxCounter;
	static bool spi2RxTxActive;
	static void spi2StartRxTx();

	static const uint32_t RX_BUFFER_LENGTH = (2048);
	static const uint32_t TX_BUFFER_LENGTH = (2048);
	static uint8_t txBuffer[TX_BUFFER_LENGTH];
	static uint8_t rxBuffer[RX_BUFFER_LENGTH];
	static uint32_t txPointerPending;
	static uint32_t txPointerTransmitted;

	static void pushByte(uint8_t byte);
	static void pushWordBigEndian(uint16_t word);


	static void wizCsAssert();
	static void wizCsDeassert();
	static void wizResetAssert();
	static void wizResetDeassert();

	enum TState{
		
		SRV_Idle=0,
		SRV_BootWait,
		SRV_SetUpAddresses,
		SRV_SetUpSockets1to7Memory,
		SRV_SetUpSocket0Memory,
		SRV_SetUpRetryParameters,
		SRV_SetTcpMode,
		SRV_SetClientPort,
		SRV_SetServerAddressPort,
		SRV_SocketInitCommand,
		SRV_SocketInitWait,
		SRV_SocketConnectCommand,
		SRV_SocketConnectWait,
		SRV_SocketEstablished,
		SRV_SocketDisconnectCommand,
		SRV_SocketDisconnectWait,
		
	};

	static TState state;

	const static uint16_t portClient = 501;
	const static uint16_t portServer = 502;

	static uint8_t ipAddressClient[4];
	static uint8_t ipAddressServer[4];
	static uint8_t ipAddressSubnet[4];
	static uint8_t ipAddressGateway[4];
	static uint8_t macAddressClient[6];
	static uint8_t macAddressServer[6];


	static uint32_t connectEstablishedCounter;

	static uint32_t txHeartbeat;
	static uint32_t rxHeartbeat;
	

public:

	static void init();

	static void process();

	static void spiInterruptHandler();

	static void enable();
	static void disable();

	struct FhppInputData{
		uint8_t scon;
		uint8_t spos;
		uint8_t sdir;
		uint8_t actualValue1;
		int32_t actualValue2;
	};
	struct FhppOutputData{
		uint8_t ccon;
		uint8_t cpos;
		uint8_t cdir;
		uint8_t setpointValue1;
		uint32_t setpointValue2;
	};

	static void readHoldingRegisters();
	static void presetMultipleRegisters(FhppOutputData &data);
	static void getFhppInputData(FhppInputData *data,uint8_t *rxMessage);
	static bool checkFhppInputData(FhppInputData *data);

	static uint16_t getRxLenght();
	static void flushRx();

	static uint32_t rxPointerReceived;
	static uint32_t rxPointerProcessed;

	static void popBytes(uint8_t *dst,uint16_t byteCount);


};













//=====================================================================================
/*
W5500 has  one Common Register Block,  eight Socket Register Blocks, and TX/RX Buffer Blocks allocated to each Socket.
*/

// Common Register Block
#define COMREG_MR		0x0000
#define COMREG_GAR0		0x001
#define COMREG_GAR1		0x002
#define COMREG_GAR2		0x003
#define COMREG_GAR3		0x004
#define COMREG_SUBR0		0x005
#define COMREG_SUBR1		0x006
#define COMREG_SUBR2		0x007
#define COMREG_SUBR3		0x008
#define COMREG_SHAR0		0x009
#define COMREG_SHAR1		0x00A
#define COMREG_SHAR2		0x00B
#define COMREG_SHAR3		0x00C
#define COMREG_SHAR4		0x00D
#define COMREG_SHAR5		0x00E
#define COMREG_SIPR0		0x00F
#define COMREG_SIPR1		0x0010
#define COMREG_SIPR2		0x0011
#define COMREG_SIPR3		0x0012
#define COMREG_INTLEVEL0		0x0013
#define COMREG_INTLEVEL1		0x0014
#define COMREG_IR		0x0015
#define COMREG_IMR		0x0016
#define COMREG_SIM		0x0017
#define COMREG_SIMR		0x0018
#define COMREG_RTR0		0x0019
#define COMREG_RTR1		0x001A
#define COMREG_RCR		0x001B
#define COMREG_PTIMER		0x001C
#define COMREG_PMAGIC		0x001D
#define COMREG_PHAR0		0x001E
#define COMREG_PHAR1		0x001F
#define COMREG_PHAR2		0x0020
#define COMREG_PHAR3		0x0021
#define COMREG_PHAR4		0x0022
#define COMREG_PHAR5		0x0023
#define COMREG_PSID0		0x0024
#define COMREG_PSID1		0x0025
#define COMREG_PMRU0		0x0026
#define COMREG_PMRU1		0x0027
#define COMREG_UIPR0		0x0028
#define COMREG_UIPR1		0x0029
#define COMREG_UIPR2		0x002A
#define COMREG_UIPR3		0x002B
#define COMREG_UPORTR0		0x002C
#define COMREG_UPORTR1		0x002D
#define COMREG_PHYCFGR		0x002E
#define COMREG_VERSIONR		0x0039


// Socket Register Block
#define Sn_MR	0x0000			
#define Sn_CR	0x001			
#define Sn_IR	0x002			
#define Sn_SR	0x003			
#define Sn_PORT0	0x004			
#define Sn_PORT1	0x005			
#define Sn_DHAR0	0x006			
#define Sn_DHAR1	0x007			
#define Sn_DHAR2	0x008			
#define Sn_DHAR3	0x009			
#define Sn_DHAR4	0x00A			
#define Sn_DHAR5	0x00B			
#define Sn_DIPR0	0x00C			
#define Sn_DIPR1	0x00D			
#define Sn_DIPR2	0x00E			
#define Sn_DIPR3	0x00F			
#define Sn_DPORT0	0x0010			
#define Sn_DPORT1	0x0011			
#define Sn_MSSR0	0x0012			
#define Sn_MSSR1	0x0013			
#define Sn_TOS	0x0015			
#define Sn_TTL	0x0016			
#define Sn_RXBUF_SIZE	0x001E			
#define Sn_TXBUF_SIZE	0x001F			
#define Sn_TX_FSR0	0x0020			
#define Sn_TX_FSR1	0x0021			
#define Sn_TX_RD0	0x0022			
#define Sn_TX_RD1	0x0023			
#define Sn_TX_WR0	0x0024			
#define Sn_TX_WR1	0x0025			
#define Sn_RX_RSR0	0x0026			
#define Sn_RX_RSR1	0x0027			
#define Sn_RX_RD0	0x0028			
#define Sn_RX_RD1	0x0029			
#define Sn_RX_WR0	0x002A			
#define Sn_RX_WR1	0x002B			
#define Sn_IMR	0x002C			
#define Sn_FRAG0	0x002D			
#define Sn_FRAG1	0x002E			
#define Sn_KPALVTR	0x002F


#define COMMAND_OPEN		(0x01)
#define COMMAND_LISTEN 		(0x02)
#define COMMAND_CONNECT 	(0x04)
#define COMMAND_DISCON 		(0x08)
#define COMMAND_CLOSE 		(0x10)
#define COMMAND_SEND 		(0x20) 
#define COMMAND_SEND_KEEP 	(0x22)
#define COMMAND_RECV 		(0x40)

#define STATUS_SOCK_CLOSED	(0x00)
#define STATUS_SOCK_INIT 	(0x13)
#define STATUS_SOCK_LISTEN	(0x14) 
#define STATUS_SOCK_ESTABLISHED	(0x17)
#define STATUS_SOCK_CLOSE_WAIT 	(0x1C)
#define STATUS_SOCK_UDP	(0x22)
#define STATUS_SOCK_MACRAW	(0x42)
#define STATUS_SOCK_SYNSENT	(0x15)
#define STATUS_SOCK_SYNRECV	(0x16)
#define STATUS_SOCK_FIN_WAIT	(0x18)
#define STATUS_SOCK_CLOSING	(0x1A)
#define STATUS_SOCK_TIME_WAIT	(0X1B)
#define STATUS_SOCK_LAST_ACK	(0X1D)

#define ACCESS_READ	(0<<2)
#define ACCESS_WRITE	(1<<2)

#define OM_VDM	(0)


#define BSB_CommonRegister	(0<<3)
#define BSB_Socket0Register	(1<<3)
#define BSB_Socket0TxBuffer	(2<<3)	
#define BSB_Socket0RxBuffer	(3<<3)	
#define BSB_Socket1Register	(5<<3)
#define BSB_Socket1TxBuffer	(6<<3)	
#define BSB_Socket1RxBuffer	(7<<3)	
#define BSB_Socket2Register	(9<<3)
#define BSB_Socket2TxBuffer	(10<<3)	
#define BSB_Socket2RxBuffer	(11<<3)	
#define BSB_Socket3Register	(13<<3)
#define BSB_Socket3TxBuffer	(14<<3)	
#define BSB_Socket3RxBuffer	(15<<3)	
#define BSB_Socket4Register	(17<<3)
#define BSB_Socket4TxBuffer	(18<<3)	
#define BSB_Socket4RxBuffer	(19<<3)	
#define BSB_Socket5Register	(21<<3)
#define BSB_Socket5TxBuffer	(22<<3)	
#define BSB_Socket5RxBuffer	(23<<3)	
#define BSB_Socket6Register	(25<<3)
#define BSB_Socket6TxBuffer	(26<<3)	
#define BSB_Socket6RxBuffer	(27<<3)	
#define BSB_Socket7Register	(29<<3)
#define BSB_Socket7TxBuffer	(30<<3)	
#define BSB_Socket7RxBuffer	(31<<3)


#endif //SERVO_SERVO_H
