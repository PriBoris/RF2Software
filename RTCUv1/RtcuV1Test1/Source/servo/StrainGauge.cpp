
#include "RtcuV1Test1.h"



uint32_t strainGaugeCounterExti = 0;
uint8_t strainGaugeByteCounter;
uint8_t strainGaugeAdcData[5];

int32_t strainGaugeResults[STRAIN_GAUGE_BUFFER_LENGTH];
uint32_t strainGaugeResultsPtr;
uint32_t strainGaugeResultsCount;
bool strainGaugeFault;





//==============================================================================================
void strainGaugeInit(void)
{
	strainGaugeResultsCount = 0;
	strainGaugeResultsPtr = 0;
	memset(&strainGaugeResults[0],0,sizeof(strainGaugeResults));
	strainGaugeFault = false;
	
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_SPI1, ENABLE); //60MHz
	
	
	srainGaugeCsDeassert();

	SPI_InitTypeDef  SPI_InitStructure;
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_SSOutputCmd(SPI1, ENABLE);
	SPI_Cmd(SPI1, ENABLE);
	

	
	while(1)
	{
		//read ID register
		srainGaugeCsAssert();
		
		strainGaugeSpiPollByte(0xFF);		
		strainGaugeSpiPollByte(0xFF);		
		strainGaugeSpiPollByte(0xFF);		
		strainGaugeSpiPollByte(0xFF);		
		strainGaugeSpiPollByte(0xFF);		
		
		uint8_t idRegisterComm = 
			(0<<7)+ //~WEN
			(1<<6)+ //read
			(4<<3)+ //id register
			(0<<2)+ //single read
			0;
		strainGaugeSpiPollByte(idRegisterComm);		
		uint8_t idRegister = strainGaugeSpiPollByte(0xFF);
		srainGaugeCsDeassert();
		
		if (idRegister==0xA0)
		{
			break;
		}
	}


		{
		// write CONFIGURATION register
			srainGaugeCsAssert();
	
			uint8_t confRegisterComm = 
				(0<<7)+ //~WEN
				(0<<6)+ //write
				(2<<3)+ //conf register
				(0<<2)+ //single write
				0;
			strainGaugeSpiPollByte(confRegisterComm);		
		
			uint32_t confRegister = 
				(0<<23)+ //chop disabled
				(0<<20)+ //External reference applied between REFIN1(+) and REFIN1(-). 
				(1<<6)+ // reference detect enabled
				(0<<8)+ //Channel 0
				(1<<4)+ //Enable buffer
				(0<<3)+ //bipolar operation
				(7<<0)+ // Gain=128
				0;
			strainGaugeSpiPollByte((confRegister>>16)&0xFF);		
			strainGaugeSpiPollByte((confRegister>>8)&0xFF);		
			strainGaugeSpiPollByte((confRegister>>0)&0xFF);		
			srainGaugeCsDeassert();
		}
		
		{
		// write MODE register
			srainGaugeCsAssert();
	
			uint8_t modeRegisterComm = 
				(0<<7)+ //~WEN
				(0<<6)+ //write
				(1<<3)+ //conf register
				(0<<2)+ //single write
				0;
			strainGaugeSpiPollByte(modeRegisterComm);		
	
			uint32_t modeRegister = 
				(0<<21)+ //Continuous conversion mode
				(1<<20)+// enables the transmission of status register contents after each data register read
				(2<<18)+//Internal 4.92 MHz clock. Pin MCLK2 is tristated. 
				(0<<15)+ // sinc3 ???
				(1<<13)+//Enable parity bit
				(8<<0)+//rate select [Output Data Rate = (4.92MHz/1024)/FS ]
				0;
			strainGaugeSpiPollByte((modeRegister>>16)&0xFF);		
			strainGaugeSpiPollByte((modeRegister>>8)&0xFF);		
			strainGaugeSpiPollByte((modeRegister>>0)&0xFF);		

			srainGaugeCsDeassert();

		}

			{
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
			}
				
				
		{
		// set MISO/PA6 as GPIO
    GPIOA->MODER &= (uint32_t)~((uint32_t)(3<<(2*6)));
		

		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);
		
		EXTI_InitTypeDef EXTI_InitStructure;
		EXTI_InitStructure.EXTI_Line = EXTI_Line6;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		}
	
		srainGaugeCsAssert();

	
}
//==============================================================================================
uint8_t strainGaugeSpiPollByte(uint8_t txByte)
{
		SPI1->DR = txByte;
		while(1)
		{
			if (SPI1->SR & SPI_I2S_FLAG_RXNE)
			{
				
				break;
			}
		}
		return (SPI1->DR);
}
//==============================================================================================
extern "C" {void EXTI9_5_IRQHandler(void){
	
		//disable gpio interrupt
		EXTI_ClearITPendingBit(EXTI_Line6);
		strainGaugeCounterExti++;
		EXTI->IMR = 0;
	
		// set MISO/PA6 as MISO
    GPIOA->MODER |= (uint32_t)(2<<(2*6));
	

//		SPI1->DR = 0xFF;
		SPI1->DR = 0x58;
		strainGaugeByteCounter = 0;
		

	
}}
//==============================================================================================
extern "C" {void SPI1_IRQHandler(void){

	strainGaugeAdcData[strainGaugeByteCounter++] = SPI1->DR;
	if (strainGaugeByteCounter!=5)
	{
		SPI1->DR = 0xFF;
	}
	else
	{
		// set MISO/PA6 as GPIO
    GPIOA->MODER &= (uint32_t)~((uint32_t)(3<<(2*6)));
		//reenable gpio interrupt
		EXTI_ClearITPendingBit(EXTI_Line6);
		EXTI->IMR = (1<<6);
		
		// TODO: check parity somehow
		
		
		int32_t dataValue;
		uint8_t *pb = (uint8_t*)&(dataValue);
		*pb++ = 0;
		*pb++ = strainGaugeAdcData[3];
		*pb++ = strainGaugeAdcData[2];
		*pb++ = strainGaugeAdcData[1];
		*(uint32_t*)&(dataValue) -= 0x80000000; // offset
		
		strainGaugeResults[strainGaugeResultsPtr] = dataValue;
		if (strainGaugeAdcData[4]&(1<<5))
		{
			strainGaugeFault = true;
		}
		/*
		if (strainGaugeAdcData[4]&(1<<6))
		{
				blackBoxStrainGauge.overflow=1;
		}
		else
		{
				blackBoxStrainGauge.overflow=0;
		}
		*/
		
		strainGaugeResultsCount++;
		strainGaugeResultsPtr = (strainGaugeResultsPtr+1)&(STRAIN_GAUGE_BUFFER_LENGTH-1);
		
	}
	
	//TODO: add 5 extra dummy reads for reliability
	
	
	

}}
//==============================================================================================
int32_t getStrainGaugeFilteredResult(void)
{
	uint32_t ptr = strainGaugeResultsPtr;
	int32_t accu = 0;
	
	for(int i=0;i<16;i++)
	{
		ptr = (ptr-1)&(STRAIN_GAUGE_BUFFER_LENGTH-1);
		accu += strainGaugeResults[ptr]/16;
	}
	
//0,001149804
	
	double accuGramms = (double)accu*0.000287451;
	return (int32_t)accuGramms;
//	return accu;
}
//==============================================================================================





