

#include "stm32f2xx_conf.h"
#include <string.h>
#include "RtcNvmem.h"


#define RTC_SPI_MAX_LENGTH	(256)
#define NVMEM_SPI_MAX_LENGTH	(256)
uint8_t rtcTxData[RTC_SPI_MAX_LENGTH];
uint8_t rtcRxData[RTC_SPI_MAX_LENGTH];
uint8_t nvmemTxData[NVMEM_SPI_MAX_LENGTH];
uint8_t nvmemRxData[NVMEM_SPI_MAX_LENGTH];
uint32_t rtcNvmemRxTxLength;
uint32_t rtcNvmemRxTxCounter;
typedef enum
{
		RTC_OFF_NVMEM_OFF = 0,
		RTC_ON = 1,
		NVMEM_ON = 2
} TRtcNvmemRxTxState;
TRtcNvmemRxTxState rtcNvmemRxTxState;


inline void rtcCsAssert()
{
	GPIOD->BSRRH = (1<<14);
}
inline void rtcCsDeassert()
{
	GPIOD->BSRRL = (1<<14);
}



inline void nvmemCsAssert()
{
	GPIOD->BSRRH = (1<<15);
}

inline void nvmemCsDeassert()
{
	GPIOD->BSRRL = (1<<15);
}







//==============================================================================================
void rtcNvmemInit()
{
		rtcCsDeassert();
		nvmemCsDeassert();
		
		rtcNvmemRxTxState = RTC_OFF_NVMEM_OFF;
	
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


	

		

	
	
}
//==============================================================================================
void rtcStartRxTx()
{
		rtcCsAssert();
		rtcNvmemRxTxState = RTC_ON;
		rtcNvmemRxTxCounter = 0;
		SPI3->DR = rtcTxData[0];
}

void nvmemStartRxTx()
{
		nvmemCsAssert();
		rtcNvmemRxTxState = NVMEM_ON;
		rtcNvmemRxTxCounter = 0;
		SPI3->DR = nvmemTxData[0];
}

bool rtcNvmemIsBusy()
{
	return (rtcNvmemRxTxState!=RTC_OFF_NVMEM_OFF);
	
}
void rtcNvmemPollingWait()
{
		while(rtcNvmemIsBusy());
}


//==============================================================================================
extern "C" {void SPI3_IRQHandler(void){

	if (rtcNvmemRxTxState==RTC_ON)
	{
		rtcRxData[rtcNvmemRxTxCounter++] = SPI3->DR;
		if (rtcNvmemRxTxCounter<rtcNvmemRxTxLength)
		{
				SPI3->DR = rtcTxData[rtcNvmemRxTxCounter];
		}
		else
		{
				rtcNvmemRxTxState = RTC_OFF_NVMEM_OFF;
				rtcCsDeassert();
		}
	}
	else//rtcNvmemRxTxState==NVMEM_ON
	{
		nvmemRxData[rtcNvmemRxTxCounter++] = SPI3->DR;
		if (rtcNvmemRxTxCounter<rtcNvmemRxTxLength)
		{
				SPI3->DR = nvmemTxData[rtcNvmemRxTxCounter];
		}
		else
		{
				rtcNvmemRxTxState = RTC_OFF_NVMEM_OFF;
				nvmemCsDeassert();
		}
	}
}}
//==============================================================================================
uint8_t rtcReadRegisterPolling(uint8_t regAddr)
{
	rtcTxData[0] = DS3234CMD_READ+regAddr; 
	rtcTxData[1] = 0;
	rtcNvmemRxTxLength = 2;
	rtcStartRxTx();
	rtcNvmemPollingWait();
	return rtcRxData[1];
}
//==============================================================================================
void rtcWriteRegisterPolling(uint8_t regAddr,uint8_t regValue)
{
	rtcTxData[0] = DS3234CMD_WRITE+regAddr; 
	rtcTxData[1] = regValue;
	rtcNvmemRxTxLength = 2;
	rtcStartRxTx();
	rtcNvmemPollingWait();
}
//==============================================================================================
void rtcReadRegistersPolling(uint8_t regStartAddr,uint8_t regCount,uint8_t *regData)
{
	rtcTxData[0] = DS3234CMD_READ+regStartAddr; 
	memset(&rtcTxData[1],0,regCount);
	rtcNvmemRxTxLength = 1+regCount;
	rtcStartRxTx();
	rtcNvmemPollingWait();
	memcpy(regData,&rtcRxData[1],regCount);
}
//==============================================================================================
void rtcReadRegistersStart(uint8_t regStartAddr,uint8_t regCount)
{
	rtcTxData[0] = DS3234CMD_READ+regStartAddr; 
	memset(&rtcTxData[1],0,regCount);
	rtcNvmemRxTxLength = 1+regCount;
	rtcStartRxTx();
}
uint8_t rtcReadRegistersResult(uint8_t regIndex)
{
		return rtcRxData[1+regIndex];
}
//==============================================================================================
uint8_t bcdByteToDec(uint8_t bcd)
{
	return ((bcd>>4)&0x0F)*10+(bcd&0x0F);
}
//==============================================================================================
bool rtcCheck()
{
		uint8_t rtcControlStatusRegister = rtcReadRegisterPolling(DS3234REG_ControlStatus);
		//check oscillator stop flag
		if ((rtcControlStatusRegister&(1<<7))!=0)
		{
			return false;
		}	
		else
		{
			return true;
		}
}
//==============================================================================================




