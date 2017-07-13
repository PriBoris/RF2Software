
#ifndef RTC_NVMEM_H
#define RTC_NVMEM_H




#define DS3234REG_Seconds	(0x00)
#define DS3234REG_Minutes	(0x01)
#define DS3234REG_Hours	(0x02)
#define DS3234REG_Day	(0x03)
#define DS3234REG_Date	(0x04)
#define DS3234REG_MonthCentury	(0x05)
#define DS3234REG_Year	(0x06)
#define DS3234REG_Control	(0x0E)
#define DS3234REG_ControlStatus	(0x0F)
#define DS3234REG_CrystalAgingOffset	(0x10)
#define DS3234REG_TempMSB	(0x11)
#define DS3234REG_TempLSB	(0x12)
#define DS3234REG_DisableTempConversions	(0x13)
#define DS3234REG_SramAddress	(0x18) //256 bytes
#define DS3234REG_SramData	(0x19)
#define DS3234CMD_WRITE 	(1<<7)
#define DS3234CMD_READ 	(0<<7)


extern void rtcNvmemInit();


extern uint8_t rtcReadRegisterPolling(uint8_t regAddr);
extern void rtcReadRegistersPolling(uint8_t regStartAddr,uint8_t regCount,uint8_t *regData);
extern void rtcWriteRegisterPolling(uint8_t regAddr,uint8_t regValue);

extern bool rtcNvmemIsBusy();
extern void rtcReadRegistersStart(uint8_t regStartAddr,uint8_t regCount);
extern uint8_t rtcReadRegistersResult(uint8_t regIndex);

extern uint8_t bcdByteToDec(uint8_t bcd);

#endif

