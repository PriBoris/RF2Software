
#ifndef MFRC522_H
#define MFRC522_H


#include <stdint.h>



class MFRC522{

private:

	static const uint8_t WRITE_COMMAND = 0;
	static const uint8_t READ_COMMAND = 1;

	class Register{
	public:
		const static uint8_t Command = 0x01;//  starts and stops command execution Table 23 on page 38
		const static uint8_t ComlEn = 0x02;//  enable and disable interrupt request control bits Table 25 on page 38
		const static uint8_t DivlEn = 0x03;//  enable and disable interrupt request control bits Table 27 on page 39
		const static uint8_t ComIrq = 0x04;//  interrupt request bits Table 29 on page 39
		const static uint8_t DivIrq = 0x05;//  interrupt request bits Table 31 on page 40
		const static uint8_t Error = 0x06;//  error bits showing the error status of the last command executed Table 33 on page 41
		const static uint8_t Status1 = 0x07;//  communication status bits Table 35 on page 42
		const static uint8_t Status2 = 0x08;//  receiver and transmitter status bits Table 37 on page 43
		const static uint8_t FIFOData = 0x09;//  input and output of 64 byte FIFO buffer Table 39 on page 44
		const static uint8_t FIFOLevel = 0x0A;//  number of bytes stored in the FIFO buffer Table 41 on page 44
		const static uint8_t WaterLevel = 0x0B;//  level for FIFO underflow and overflow warning Table 43 on page 44
		const static uint8_t Control = 0x0C;//  miscellaneous control registers Table 45 on page 45
		const static uint8_t BitFraming = 0x0D;//  adjustments for bit-oriented frames Table 47 on page 46
		const static uint8_t Coll = 0x0E;//  bit position of the first bit-collision detected on the RF interface Table 49 on page 46
		const static uint8_t Mode = 0x11;//  defines general modes for transmitting and receiving Table 55 on page 48
		const static uint8_t TxMode = 0x12;//  defines transmission data rate and framing Table 57 on page 48
		const static uint8_t RxMode = 0x13;//  defines reception data rate and framing Table 59 on page 49
		const static uint8_t TxControl = 0x14;//  controls the logical behavior of the antenna driver pins TX1 and TX2 Table 61 on page 50
		const static uint8_t TxASK = 0x15;//  controls the setting of the transmission modulation Table 63 on page 51
		const static uint8_t TxSel = 0x16;//  selects the internal sources for the antenna driver Table 65 on page 51
		const static uint8_t RxSel = 0x17;//  selects internal receiver settings Table 67 on page 52
		const static uint8_t RxThreshold = 0x18;//  selects thresholds for the bit decoder Table 69 on page 53
		const static uint8_t Demod = 0x19;//  defines demodulator settings Table 71 on page 53
		const static uint8_t MfTx = 0x1C;//  controls some MIFARE communication transmit parameters Table 77 on page 55
		const static uint8_t MfRx = 0x1D;//  controls some MIFARE communication receive parameters Table 79 on page 55
		const static uint8_t SerialSpeed = 0x1F;//  selects the speed of the serial UART interface Table 83 on page 55
		const static uint8_t CRCResult = 0x21;//  shows the MSB and LSB values of the CRC calculation Table 87 on page 57
		const static uint8_t ModWidth = 0x24;//  controls the ModWidth setting Table 93 on page 58
		const static uint8_t RFCfg = 0x26;//  configures the receiver gain Table 97 on page 59
		const static uint8_t GsN = 0x27;//  selects the conductance of the antenna driver pins TX1 and TX2 for modulation Table 99 on page 59
		const static uint8_t CWGsP = 0x28;//  defines the conductance of the p-driver output during periods of no modulation Table 101 on page 60
		const static uint8_t ModGsP = 0x29;//  defines the conductance of the p-driver output during periods of modulation Table 103 on page 60
		const static uint8_t TMode = 0x2A;//  defines settings for the internal timer Table 105 on page 60
		const static uint8_t TPrescaler = 0x2B;//  Table 107 on page 61
		const static uint8_t TReload = 0x2C;//  defines the 16-bit timer reload value Table 109 on page 62
		const static uint8_t TCounterVal = 0x2E;//  shows the 16-bit timer value Table 113 on page 63
		const static uint8_t TestSel1 = 0x31;//  general test signal configuration Table 119 on page 63
		const static uint8_t TestSel2 = 0x32;//  general test signal configuration and PRBS control Table 121 on page 64
		const static uint8_t TestPinEn = 0x33;//  enables pin output driver on pins D1 to D7 Table 123 on page 64
		const static uint8_t TestPinValue = 0x34;//  defines the values for D1 to D7 when it is used as an I/O bus Table 125 on page 65
		const static uint8_t TestBus = 0x35;//  shows the status of the internal test bus Table 127 on page 65
		const static uint8_t AutoTest = 0x36;//  controls the digital self test Table 129 on page 66
		const static uint8_t Version = 0x37;//  shows the software version Table 131 on page 66
		const static uint8_t AnalogTest = 0x38;//  controls the pins AUX1 and AUX2 Table 133 on page 67
		const static uint8_t TestDAC1 = 0x39;//  defines the test value for TestDAC1 Table 135 on page 68
		const static uint8_t TestDAC2 = 0x3A;//  defines the test value for TestDAC2 Table 137 on page 68
		const static uint8_t TestADC = 0x3B;//  shows the value of ADC I and Q channels Table 139 on page 68 = 0x3C;//  3Fh Reserved reserved for production tests Table 141 to Table 147 on page 69
	};

	class Command{
	public:
		const static uint8_t Idle = 0;// 0000 no action, cancels current command execution
		const static uint8_t Mem = 1;// 0001 stores 25 bytes into the internal buffer
		const static uint8_t GenerateRandomID = 2;//  0010 generates a 10-byte random ID number
		const static uint8_t CalcCRC = 3;// 0011 activates the CRC coprocessor or performs a self test
		const static uint8_t Transmit = 4;// 0100 transmits data from the FIFO buffer
		const static uint8_t NoCmdChange = 7;// 0111 no command change, can be used to modify the CommandReg register bits without affecting the command, for example, the PowerDown bit
		const static uint8_t Receive = 8;// 1000 activates the receiver circuits
		const static uint8_t Transceive = 12;// 1100 transmits data from FIFO buffer to antenna and automatically activates the receiver after transmission
		const static uint8_t MFAuthent = 14;// 1110 performs the MIFARE standard authentication as a reader
		const static uint8_t SoftReset = 15;// 1111 resets the MFRC522		
	};


	static void spiCsAssert();
	static void spiCsDeassert();
	static uint8_t spiRxTxByte(uint8_t tx);
	static uint8_t spiReadRegister(uint8_t reg);
	static void spiWriteRegister(uint8_t reg,uint8_t val);
	
	

public:

	static void start();
	static void spiInterrupt();

};

#endif
