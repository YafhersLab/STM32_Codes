#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "main.h"

typedef enum{
	LOW,
	HIGH
}gpio_state;

void GPIO_Led_Init(void);
void GPIO_Led_Write(gpio_state estado);
void GPIO_Led_Toggle(void);
void GPIO_Button_Init(void);
gpio_state GPIO_Button_Read(void);

#endif
