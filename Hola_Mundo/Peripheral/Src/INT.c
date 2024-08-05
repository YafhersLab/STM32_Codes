#include "INT.h"

void INT_Init(void){
	GPIO_InitTypeDef gpioInit = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();

	gpioInit.Pin = GPIO_PIN_13;
	gpioInit.Mode = GPIO_MODE_IT_FALLING;
	gpioInit.Pull = GPIO_PULLUP;
	gpioInit.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOC, &gpioInit);

	//activar interrupciones
	NVIC_SetPriority(EXTI15_10_IRQn, 5);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}
