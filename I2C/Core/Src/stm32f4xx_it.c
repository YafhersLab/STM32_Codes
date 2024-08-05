#include "stm32f4xx.h"

volatile uint32_t uwTicks = 0;
volatile uint32_t dataBT;
volatile uint16_t adcData = 0;
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

float yk, uk, uk_1;

void ADC_IRQHandler(void){
	if(ADC1 -> SR & ADC_SR_OVR){
		ADC1 -> SR &=~ ADC_SR_OVR;													//cuando haya overrun lo borramos
	}
	if(ADC1 -> SR & ADC_SR_EOC){
		adcData = ADC1 -> DR;

		/*
		//filtremos el dato mediante un pasabajo RC
		//y(k) = 0.03046 * (u(k) + u(k-1)) + 0.09341 * y(k-1)
		uk_1 = uk;
		uk = adcData;
		yk = 0.03046 * (uk + uk_1) + 0.9341 * yk;
		printf("Mi valor es: %f y tambien es %f\r\n", yk, uk);
		*/
	}
}
