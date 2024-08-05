#include "stm32f4xx.h"

volatile uint32_t uwTicks = 0;

void SysTick_Handler(void){
	//multiplexado
	uwTicks += 1;
	return ;
}

void EXTI15_10_IRQHandler(void){
	if(EXTI -> PR & EXTI_PR_PR13){
		EXTI -> PR |= EXTI_PR_PR13;			//limpiar el pending byte PR13
		GPIOA -> ODR ^= GPIO_ODR_OD5;		//toggle PA5
	}
	return ;
}
