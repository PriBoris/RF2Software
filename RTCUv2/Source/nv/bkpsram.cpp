
#include "bkpsram.h"

#include <stm32f4xx_conf.h>



void NV::BkpSram::init(){
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);//BKPSRAM_BASE = ((uint32_t)0x40024000)
	PWR_BackupRegulatorCmd(ENABLE);
	
	
}


uint32_t NV::BkpSram::odometerBlock0Addr(){
	return (BKPSRAM_BASE+0);
}

uint32_t NV::BkpSram::odometerBlock1Addr(){
	return (BKPSRAM_BASE+64);
}

uint32_t NV::BkpSram::settingsBlock0Addr(){
	return (BKPSRAM_BASE+128);
}

uint32_t NV::BkpSram::settingsBlock1Addr(){
	return (BKPSRAM_BASE+128+256);
}



