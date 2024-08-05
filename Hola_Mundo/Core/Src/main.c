#include "main.h"
#include "GPIO.h"
#include "SysConfig.h"
#include "LCD.h"
#include "Timer.h"

LCD_PortTypeDef LCD_Port[] = {
		D4_Port, D5_Port, D6_Port, D7_Port
};

LCD_PinTypeDef LCD_Pin[] = {
		D4_Pin, D5_Pin, D6_Pin, D7_Pin
};

extern TIM_HandleTypeDef Tim3;

int main(){
	HAL_Init();
	SysConfigClock();
	GPIO_Led_Init();
	//LCD_Struct_t LCD = LCD_Create(LCD_Port, LCD_Pin, RS_Port, RS_Pin, E_Port, E_Pin);

	//LCD_XY(&LCD, 0, 0);
	//LCD_Cadena(&LCD, "Hola mundo :D");

	Timer_Init(100);
	HAL_TIM_Base_Start_IT(&Tim3);

	while(1){

	}
	return 0;
}

void Error_Handler(void){
  __disable_irq();
  while (1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim -> Instance == TIM3){
		GPIO_Led_Toggle();
	}
}
