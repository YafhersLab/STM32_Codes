#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stdint.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#define E_ON	HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, SET);
#define E_OFF	HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, RESET);
#define RS_ON	HAL_GPIO_WritePin(E_GPIO_Port, RS_Pin, SET);
#define RS_OFF	HAL_GPIO_WritePin(E_GPIO_Port, RS_Pin, RESET);
#define D4_ON	HAL_GPIO_WritePin(E_GPIO_Port, D4_Pin, SET);
#define D4_OFF	HAL_GPIO_WritePin(E_GPIO_Port, D4_Pin, RESET);
#define D5_ON	HAL_GPIO_WritePin(E_GPIO_Port, D5_Pin, SET);
#define D5_OFF	HAL_GPIO_WritePin(E_GPIO_Port, D5_Pin, RESET);
#define D6_ON	HAL_GPIO_WritePin(E_GPIO_Port, D6_Pin, SET);
#define D6_OFF	HAL_GPIO_WritePin(E_GPIO_Port, D6_Pin, RESET);
#define D7_ON	HAL_GPIO_WritePin(E_GPIO_Port, D7_Pin, SET);
#define D7_OFF	HAL_GPIO_WritePin(E_GPIO_Port, D7_Pin, RESET);

typedef enum{
	f1 = 0,
	f2 = 1,
	f3 = 2,
	f4 = 3
}location;

void LCD_Command(uint8_t x);
void LCD_Init(void);
void LCD_XY(uint8_t x, location y);
void LCD_Print_Chart(char c);
void LCD_Print_String(char *s);
void LCD_NewChar(uint8_t a,uint8_t b,uint8_t c,uint8_t d ,uint8_t e ,uint8_t f,uint8_t g,uint8_t h,uint8_t i);
void LCD_Print_NewChar(uint8_t a);
void LCD_Clear(void);

#endif
