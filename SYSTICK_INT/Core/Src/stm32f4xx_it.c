#include "stm32f4xx.h"

volatile uint32_t uwTicks = 0;

void SysTick_Handler(void){
	//multiplexado
	uwTicks += 1;
	return ;
}
