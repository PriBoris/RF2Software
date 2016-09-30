

#include "rtc.h"

#include <stm32f4xx_conf.h>

#include "business/Odometer.h"

#define RTC_INITIALIZED_CODE	(0xA5A55A78)

uint32_t NV::RealTimeClock::prevTRValue;



void NV::RealTimeClock::init(){


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RCC_LSEConfig(RCC_LSE_Bypass);
//	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){
//	};
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();

	uint32_t temp = RTC_ReadBackupRegister(RTC_BKP_DR0);

	if (temp!=RTC_INITIALIZED_CODE){

		RTC_InitTypeDef   RTC_InitStructure;
		RTC_TimeTypeDef   RTC_TimeStructure;
		RTC_DateTypeDef   RTC_DateStructure;

		RTC_TimeStructure.RTC_Hours   = 0x01;
		RTC_TimeStructure.RTC_Minutes = 0x40;
		RTC_TimeStructure.RTC_Seconds = 0x00;

		RTC_DateStructure.RTC_Month = RTC_Month_February;//1..12
		RTC_DateStructure.RTC_Date = 0x08;  
		RTC_DateStructure.RTC_Year = 0x16; 
		RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Monday; //1..7
		
		RTC_InitStructure.RTC_AsynchPrediv = 127;
		RTC_InitStructure.RTC_SynchPrediv =  255;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
		RTC_Init(&RTC_InitStructure);

		RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);  
		RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure); 
		
		RTC_WriteBackupRegister(RTC_BKP_DR0, RTC_INITIALIZED_CODE);

	} else {
		
	}
	NV::RealTimeClock::prevTRValue = RTC->TR;	
	
}

void NV::RealTimeClock::poll(){
	
	uint32_t TRValue = RTC->TR;
	if (TRValue!=prevTRValue){
		prevTRValue = TRValue;
		
		Odometer::incrementSeconds();
		

		
		
		
	}
	
	
}

void NV::RealTimeClock::getCurrentDateTime(TCurrentDateTime &currentDateTime){


	uint32_t TRValue = RTC->TR;
	uint32_t DRValue = RTC->DR;

	currentDateTime.seconds = bcdByteToDec((TRValue&0x0000007F)>>0);
	currentDateTime.minutes = bcdByteToDec((TRValue&0x00007F00)>>8);
	currentDateTime.hours = bcdByteToDec((TRValue&0x003F0000)>>16);

	currentDateTime.date = bcdByteToDec((DRValue&0x0000003F)>>0);
	currentDateTime.year = bcdByteToDec((DRValue&0x00FF0000)>>16);
	currentDateTime.month = bcdByteToDec((DRValue&0x00001F00)>>8);


}

uint8_t NV::RealTimeClock::bcdByteToDec(uint8_t bcd)
{
	return ((bcd>>4)&0x0F)*10+(bcd&0x0F);
}

