#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "stm32f4xx_hal.h"
#include "main.h"

#define ROW1_PIN	GPIO_PIN_8
#define ROW1_PORT	GPIOB
#define ROW2_PIN	GPIO_PIN_10
#define ROW2_PORT	GPIOB
#define ROW3_PIN	GPIO_PIN_4
#define ROW3_PORT	GPIOB
#define ROW4_PIN	GPIO_PIN_5
#define ROW4_PORT	GPIOB
#define COL1_PIN	GPIO_PIN_3
#define COL1_PORT	GPIOB
#define COL2_PIN	GPIO_PIN_10
#define COL2_PORT	GPIOA
#define COL3_PIN	GPIO_PIN_2
#define COL3_PORT	GPIOA
#define COL4_PIN	GPIO_PIN_3
#define COL4_PORT	GPIOA

void Keypad_Init(void);
char Keypad_Read(void);

#endif /* INC_KEYPAD_H_ */
