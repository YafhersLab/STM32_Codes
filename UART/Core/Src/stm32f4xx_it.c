#include "stm32f4xx.h"

volatile uint32_t uwTicks = 0;
volatile uint32_t dataBT;

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

void USART2_IRQHandler(void){
	if(USART2 -> SR & USART_SR_RXNE){
		//leo el dato recibido por la interrupcion
		dataBT = USART2 -> DR;
	}
}
