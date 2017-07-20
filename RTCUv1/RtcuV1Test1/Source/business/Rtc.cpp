

#include "RtcuV1Test1.h"







//==============================================================================================

//==============================================================================================
void rtcProcess()
{
	static uint32_t currentDateTimeCounter = 0;
	if (rtcNvmemIsBusy()==false)
	{
		if (currentDateTimeCounter!=0)
		{
				currentDateTime.year = bcdByteToDec(rtcReadRegistersResult(DS3234REG_Year));
				currentDateTime.month = bcdByteToDec(rtcReadRegistersResult(DS3234REG_MonthCentury));
				currentDateTime.date = bcdByteToDec(rtcReadRegistersResult(DS3234REG_Date));
				currentDateTime.hours = bcdByteToDec(rtcReadRegistersResult(DS3234REG_Hours));
				currentDateTime.minutes = bcdByteToDec(rtcReadRegistersResult(DS3234REG_Minutes));
				currentDateTime.seconds = bcdByteToDec(rtcReadRegistersResult(DS3234REG_Seconds));
		}
		rtcReadRegistersStart(DS3234REG_Seconds,7);
		currentDateTimeCounter++;
	
		
	}
	
	
}
//==============================================================================================





