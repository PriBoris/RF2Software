
#include "OdometerRTC.h"

#include "stm32f2xx_conf.h"

#include <string.h>

//======================================================================================================
bool OdometerRTC::rxTxActive;
uint8_t OdometerRTC::txData[OdometerRTC::RX_TX_DATA_BUF_LEN];
uint8_t OdometerRTC::rxData[OdometerRTC::RX_TX_DATA_BUF_LEN];
uint32_t OdometerRTC::rxTxDataCount;
uint32_t OdometerRTC::transferLength;
TCurrentDateTime OdometerRTC::currentDateTime;
TCurrentDateTime OdometerRTC::previousDateTime;
OdometerRTC::Record OdometerRTC::record0,OdometerRTC::record1;
uint32_t OdometerRTC::checksumTable[256];	
bool OdometerRTC::checksumTableValid;
bool OdometerRTC::actualRecordIsOdd;



//======================================================================================================
void OdometerRTC::init(){

	rxTxActive = false;
	rxTxDataCount = 0;
	transferLength = 0;
	memset(txData,0,sizeof(txData));
	memset(rxData,0,sizeof(rxData));
	
	checksumTableValid = false;
	actualRecordIsOdd = false;

	nvmemCsDeassert();
	rtcCsDeassert();	

	RCC_APB1PeriphClockCmd (RCC_APB1Periph_SPI3, ENABLE); //30MHz
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_I2S_DeInit(SPI3);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;/* data is always latched in on the rising edge of SCLK and always output on the falling edge of SCLK.  */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_16; /* SPI_BaudRatePrescaler_2 ... SPI_BaudRatePrescaler_256 */
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI3, &SPI_InitStructure);
	SPI_SSOutputCmd(SPI3, ENABLE);
	SPI_Cmd(SPI3, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_RXNE, ENABLE);	

	// check if oscillator has stopped before
	uint8_t controlStatusRegister;
	ds3234ReadRegisters(DS3234REG_ControlStatus,1,&controlStatusRegister);
	if ((controlStatusRegister&(1<<7))!=0){
		while(true);
	}

	readCurrentDateTime(false);


	{
		readRecord(false);
		readRecord(true);
		bool record0Valid = checkRecord(false);
		bool record1Valid = checkRecord(true);
		if ((record0Valid==true)&&(record1Valid==true)){

			if (record0.timeSeconds>=record1.timeSeconds){

				actualRecordIsOdd = false;

			} else {

				actualRecordIsOdd = true;

			}

		} else if (record0Valid==true){

			//actualRecordIsOdd = false;
			memcpy(&record1,&record0,sizeof(Record));
			actualRecordIsOdd = true;



		} else if (record1Valid==true){

			//actualRecordIsOdd = true;
			memcpy(&record0,&record1,sizeof(Record));
			actualRecordIsOdd = false;

		} else {

			actualRecordIsOdd = false;

			memset(&record0,0,sizeof(Record));
			saveRecord(actualRecordIsOdd);

			memcpy(&record1,&record0,sizeof(Record));
			actualRecordIsOdd = true;

		}



	}









	
}
//======================================================================================================
void OdometerRTC::rtcCsAssert(){
	GPIOD->BSRRH = (1<<14);
}
void OdometerRTC::rtcCsDeassert(){
	GPIOD->BSRRL = (1<<14);
}
void OdometerRTC::nvmemCsDeassert(){
	GPIOD->BSRRL = (1<<15);
}
//======================================================================================================
extern "C" {void SPI3_IRQHandler(void){
	OdometerRTC::rxTxByteDone();
}}
//======================================================================================================
void OdometerRTC::rxTxByteDone(){

	rxData[rxTxDataCount++] = SPI3->DR;
	if (rxTxDataCount<transferLength)
	{
		SPI3->DR = txData[rxTxDataCount];
	}
	else
	{
		rxTxActive = false;
		rtcCsDeassert();
	}

}
//======================================================================================================
void OdometerRTC::waitWhileBusy(){

	while(rxTxActive==true);

}
//======================================================================================================
void OdometerRTC::ds3234ReadRegisters(uint8_t startAddress,uint8_t registerCount,uint8_t* data){

	txData[0] = DS3234CMD_READ+startAddress; 
	memset(&txData[1],0,registerCount);
	transferLength = 1+registerCount;
	startRxTx();
	waitWhileBusy();
	memcpy(data,&rxData[1],registerCount);

}
//======================================================================================================
void OdometerRTC::ds3234WriteRegisters(uint8_t startAddress,uint8_t registerCount,uint8_t* data){

	txData[0] = DS3234CMD_WRITE+startAddress; 
	memcpy(&txData[1],data,registerCount);
	transferLength = 1+registerCount;
	startRxTx();
	waitWhileBusy();

}
//======================================================================================================
void OdometerRTC::startRxTx(){
	rtcCsAssert();
	rxTxActive = true;
	rxTxDataCount = 0;
	SPI3->DR = txData[0];
}
//======================================================================================================
uint8_t OdometerRTC::bcdByteToDec(uint8_t bcd){

	return ((bcd>>4)&0x0F)*10+(bcd&0x0F);
}
//======================================================================================================
void OdometerRTC::readCurrentDateTime(bool updateOdometerTime){

	uint8_t timeStampRegisters[7];
	ds3234ReadRegisters(DS3234REG_Seconds,7,&timeStampRegisters[0]);
	
	currentDateTime.year = bcdByteToDec(timeStampRegisters[DS3234REG_Year]);
	currentDateTime.month = bcdByteToDec(timeStampRegisters[DS3234REG_MonthCentury]);
	currentDateTime.date = bcdByteToDec(timeStampRegisters[DS3234REG_Date]);
	currentDateTime.hours = bcdByteToDec(timeStampRegisters[DS3234REG_Hours]);
	currentDateTime.minutes = bcdByteToDec(timeStampRegisters[DS3234REG_Minutes]);
	currentDateTime.seconds = bcdByteToDec(timeStampRegisters[DS3234REG_Seconds]);

	if (updateOdometerTime==false){

		memcpy(&previousDateTime,&currentDateTime,sizeof(TCurrentDateTime));

	} else {

		if (memcmp(&previousDateTime,&currentDateTime,sizeof(TCurrentDateTime))!=0){

			if (actualRecordIsOdd==false){

				record0.timeSeconds++;
				saveRecord(false);
				memcpy(&record1,&record0,sizeof(Record));
				actualRecordIsOdd = true;


			} else {

				record1.timeSeconds++;
				saveRecord(true);
				memcpy(&record0,&record1,sizeof(Record));
				actualRecordIsOdd = false;


			}




			memcpy(&previousDateTime,&currentDateTime,sizeof(TCurrentDateTime));
		}




	}



}
//======================================================================================================
void OdometerRTC::readRecord(bool oddRecord){

	uint8_t recordAddress = (oddRecord==false)?RECORD0_ADDRESS:RECORD1_ADDRESS;
	ds3234WriteRegisters(DS3234REG_SramAddress,1,&recordAddress);

	uint8_t *recordData = (oddRecord==false)?((uint8_t*)&record0):((uint8_t*)&record1);
	for(uint32_t i=0;i<sizeof(Record);i++){
		ds3234ReadRegisters(DS3234REG_SramData,1,&recordData[i]);
	}



}
//======================================================================================================
void OdometerRTC::saveRecord(bool oddRecord){

	{
		uint8_t *recordData = (oddRecord==false)?((uint8_t*)&record0):((uint8_t*)&record1);
		uint32_t checksum = CHECKSUM_SEED;
		for(uint32_t i=0;i<(sizeof(Record)-4);i++){
			checksumAppendByte(*recordData++,checksum);
		}
		memcpy(recordData,&checksum,sizeof(checksum));
	}




	{
		uint8_t recordAddress = (oddRecord==false)?RECORD0_ADDRESS:RECORD1_ADDRESS;
		uint8_t *recordData = (oddRecord==false)?((uint8_t*)&record0):((uint8_t*)&record1);
		uint8_t txPair[2];

		for(uint8_t i=0;i<sizeof(Record);i++){
			txPair[0] = recordAddress+i;
			txPair[1] = *recordData++;
			ds3234WriteRegisters(DS3234REG_SramAddress,2,txPair);
		}


	}

}
//======================================================================================================
bool OdometerRTC::checkRecord(bool oddRecord){

	uint8_t *recordData = (oddRecord==false)?((uint8_t*)&record0):((uint8_t*)&record1);
	uint32_t checksum = CHECKSUM_SEED;
	for(uint32_t i=0;i<(sizeof(Record)-4);i++){
		checksumAppendByte(*recordData++,checksum);
	}
	return (memcmp(recordData,&checksum,sizeof(checksum))==0);
}
//======================================================================================================
void OdometerRTC::checksumTableCreate(void){

    uint32_t c;
    int32_t n, k;
    for (n = 0; n < 256; n++)    {
        c = (uint32_t) n;
        for (k = 0; k < 8; k++)        {
            if (c & 1)            {
                c = 0xedb88320L ^ (c >> 1);
            }else{
                c = c >> 1;
            }
        }
        checksumTable[n] = c;
    }
    checksumTableValid = true;
}
//======================================================================================================
void OdometerRTC::checksumAppendByte(uint8_t newByte,uint32_t &checksum){
	
    uint32_t c = checksum ^ 0xffffffffL;
    if (checksumTableValid==false){
        checksumTableCreate();
	}
    c = checksumTable[(c ^ (uint32_t)newByte) & 0xff] ^ (c >> 8);
	checksum = (c ^ 0xffffffffL);
}
//======================================================================================================
uint32_t OdometerRTC::getOdometerTimeMinutes(){

	return (actualRecordIsOdd==false)?(record0.timeSeconds/60):(record1.timeSeconds/60);
}
//======================================================================================================
uint32_t OdometerRTC::getOdometerDistanceMeters(){

	return (actualRecordIsOdd==false)?(record0.distanceMms/1000):(record1.distanceMms/1000);

}
//======================================================================================================
void OdometerRTC::addMovement(int32_t startAbsPosition,int32_t stopAbsPosition){

	const int32_t ratio = 100;//TODO: 100 must be system setting depending on servo type
	int32_t absDistance = (startAbsPosition - stopAbsPosition)/ratio;
	if (absDistance<0){
		absDistance = -absDistance;
	}

	if (actualRecordIsOdd==false){
		record0.distanceMms += (uint32_t)absDistance;
	} else {
		record1.distanceMms += (uint32_t)absDistance;
	}

}
//======================================================================================================



