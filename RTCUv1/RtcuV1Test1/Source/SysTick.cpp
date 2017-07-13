

#include "RtcuV1Test1.h"

uint32_t heartbeatCounter = 0;


//==============================================================================================
void SysTickInit(void)
{
	heartbeatCounter = 0;
	

	*(uint32_t*)0xE000E014 = 120000000/1000;//SYST_RVR	- SysTick Reload Value	
	*(uint32_t*)0xE000E010 = 
		(1<<0)|//enable
		(1<<1)|//interrupt
		(1<<2)|//processor clock
		0;//SYST_CSR
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//NVIC_PriorityGroup_2: 2 bits for pre-emption priority 2 bits for subpriority 
	while(heartbeatCounter<1000);
	
}
//==============================================================================================
extern "C" {void SysTick_Handler(void)//1000Hz
{
	heartbeatCounter++;
	
	static uint16_t modeCounter = 0;
	
	if (modeCounter==100)
	{
			modeCounter = 0;
			modeProcessFlag = true;
	}
	modeCounter++;
	
	
	
}}
//==============================================================================================
