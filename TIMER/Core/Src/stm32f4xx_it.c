#include "stm32f4xx.h"

volatile uint32_t uwTicks = 0;
volatile uint32_t dataBT;
int flagX = 0;

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

void USART6_IRQHandler(void){
	if(USART6 -> SR & USART_SR_RXNE){
		//leo el dato recibido por la interrupcion
		dataBT = USART6 -> DR;
	}
}

void TIM5_IRQHandler(void){
	if(TIM5 -> SR & TIM_SR_UIF){
		TIM5 -> SR &=~ TIM_SR_UIF;	//limpiar bandera
		GPIOA -> ODR ^= GPIO_ODR_OD9;
	}
}

void TIM3_IRQHandler(void){
	static uint32_t ccr3;
	if(TIM3 -> SR & TIM_SR_CC3IF){
		TIM3 -> SR &=~ TIM_SR_CC3IF;
		ccr3 = TIM3 -> CCR3 + 200;
		TIM3 -> CCR3  = ccr3;
	}
}
