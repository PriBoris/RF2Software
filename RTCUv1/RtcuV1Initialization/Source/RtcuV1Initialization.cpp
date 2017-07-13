
#include "RtcuV1Initialization.h"



int main()
{
	
	
	gpioInit();
	rtcNvmemInit();
	
	
	// new date time
	uint8_t newDateYear = 0x15;
	uint8_t newDateMonth = 0x08;//1..12
	uint8_t newDateDate = 0x13;//1..31
	uint8_t newTimeHours = 0x00;
	uint8_t newTimeMinutes = 0x55;
	uint8_t newTimeSeconds = 0x12;
	
	rtcWriteRegisterPolling(DS3234REG_Seconds,newTimeSeconds);
	rtcWriteRegisterPolling(DS3234REG_Minutes,newTimeMinutes);
	rtcWriteRegisterPolling(DS3234REG_Hours,newTimeHours);
	rtcWriteRegisterPolling(DS3234REG_Date,newDateDate);
	rtcWriteRegisterPolling(DS3234REG_MonthCentury,newDateMonth);
	rtcWriteRegisterPolling(DS3234REG_Year,newDateYear);

	// control register
	uint8_t newControlReg = 
		(0<<7)+
		(0<<6)+
		(0<<5)+
		(0<<3)+
		(0<<2)+
		(0<<1)+
		(0<<0)+
		0;
	rtcWriteRegisterPolling(DS3234REG_Control,newControlReg);

	// control/status register
	uint8_t newControlStatusReg = 
		(0<<7)+//reset oscillator stop flag
		(0<<6)+//disable  Battery-Backed 32kHz Output 
		(0<<4)+// Conversion  Rate = 1/64Hz
		(0<<3)+//disable 32kHz Output 
		0;
	rtcWriteRegisterPolling(DS3234REG_ControlStatus,newControlStatusReg);



	// initializa SRAM
	// ....


	
	uint8_t currentDateTime[7];
	while(1)
	{
		rtcReadRegistersPolling(DS3234REG_Seconds,7,currentDateTime);
		
		
	}
	
	
	
}

