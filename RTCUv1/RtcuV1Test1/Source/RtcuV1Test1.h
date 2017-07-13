
#ifndef RTCUV1_H
#define RTCUV1_H

#include "stm32f2xx_conf.h"
#include <string.h>
#include "Servo.h"
#include "Protocol.h"
#include "BlackBox.h"
#include "crc32.h"
#include "Hmi.h"
#include "Nfc.h"
#include "OdometerRTC.h"

#include "mode.h"


extern void SysTickInit(void);
extern void gpioInit(void);

extern bool modeProcessFlag;


//====================<StrainGauge.cpp>==============================================

extern void strainGaugeInit(void);

inline void srainGaugeCsAssert()
{
		GPIOB->BSRRH = (1<<6);
};
inline void srainGaugeCsDeassert()
{
		GPIOB->BSRRL = (1<<6);
};

extern uint8_t strainGaugeSpiPollByte(uint8_t rxByte);

#define STRAIN_GAUGE_BUFFER_LENGTH	(512)
extern int32_t strainGaugeResults[STRAIN_GAUGE_BUFFER_LENGTH];
extern uint32_t strainGaugeResultsPtr;
extern uint32_t strainGaugeResultsCount;
extern bool strainGaugeFault;

extern int32_t getStrainGaugeFilteredResult(void);















#endif

