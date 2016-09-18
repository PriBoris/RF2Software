
#include "Indicator.h"

#include "stm32f0xx.h"


uint32_t Indicator::downCounter = 0;


void Indicator::start(){

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	TIM1->PSC = 0;// Prescaler value-1
	TIM1->ARR = 240;
	TIM1->CCR1 = 5;
	TIM1->CCER = 
		(1<<1)+// 0: OC1 active high 
		(1<<0)+// OC1 signal is output
		0;
	TIM1->CCMR1 = 
		(6<<4)+// Output Compare 1 PWM mode 1
		(0<<0)+//00: CC1 channel is configured as output 
		0;
	TIM1->CR1 = 
		//(1<<7)+//1: TIMx_ARR register is buffered
		(0<<5)+//00: Edge-aligned mode
		(0<<4)+//0: Counter used as upcounter 
		(0<<3)+//0: Counter is not stopped at update event
		(1<<0)+//1: Counter enabled
		0;
	TIM1->BDTR = (1<<15);// MOE: Main output enable


	//ADC start


}

void Indicator::tick(){

	if (downCounter>0){
		downCounter--;
		if (downCounter==0){
			TIM1->CCR1 = 5;
		}
	}else{
		
	}
	
}
void Indicator::flash(){
	
	downCounter = 5;
	TIM1->CCR1 = 60;

}




/*

Pulse Width Modulation mode allows you to generate a signal 
with a frequency determined by the value of the TIMx_ARR register 
and a duty cycle determined by the value of the TIMx_CCRx register.

The PWM mode can be selected independently on each channel (one PWM per OCx output)
by writing ‘110’ (PWM mode 1) or ‘111’ (PWM mode 2) in the OCxM bits in the TIMx_CCMRx register. 

You must enable the corresponding preload register by setting the 
OCxPE bit in the TIMx_CCMRx register, and eventually the auto-reload preload register (in 
upcounting or center-aligned modes) by setting the ARPE bit in the TIMx_CR1 register.

As the preload registers are transferred to the shadow registers only when an update event 
occurs, before starting the counter, you have to initialize all the registers by setting the UG 
bit in the TIMx_EGR register.

OCx polarity is software programmable using the CCxP bit in the TIMx_CCER register.
It can be programmed as active high or active low. 

OCx output is enabled by a combination of the CCxE, CCxNE, MOE, OSSI and OSSR bits (TIMx_CCER and TIMx_BDTR registers). 

In PWM mode (1 or 2), TIMx_CNT and TIMx_CCRx are always compared to determine 
whether TIMx_CCRx ≤ TIMx_CNT or TIMx_CNT ≤ TIMx_CCRx (depending on the direction 
of the counter). 

*/
