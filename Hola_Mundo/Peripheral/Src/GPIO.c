#include "GPIO.h"

void GPIO_Led_Init(void){
	//PA5 -> LED
	GPIO_InitTypeDef gpioInit = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpioInit.Pin = GPIO_PIN_5;
	gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOA, &gpioInit);
}

void GPIO_Led_Write(gpio_state estado){
	if(estado == HIGH){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	}
}

void GPIO_Led_Toggle(void){
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

void GPIO_Button_Init(void){
	//PC13 -> Button
	GPIO_InitTypeDef gpioInit = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();

	gpioInit.Pin = GPIO_PIN_13;
	gpioInit.Mode = GPIO_MODE_INPUT;
	gpioInit.Pull = GPIO_PULLUP;
	gpioInit.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOC, &gpioInit);
}

gpio_state GPIO_Button_Read(void){
	return (bool) HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
}
