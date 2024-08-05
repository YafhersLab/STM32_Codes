#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

#define D4_Pin GPIO_PIN_4
#define D4_Port GPIOB
#define D5_Pin GPIO_PIN_5
#define D5_Port GPIOB
#define D6_Pin GPIO_PIN_6
#define D6_Port GPIOB
#define D7_Pin GPIO_PIN_7
#define D7_Port GPIOB
#define D8_Pin GPIO_PIN_8
#define D8_Port GPIOB
#define RS_Pin GPIO_PIN_0
#define RS_Port GPIOC
#define E_Pin GPIO_PIN_1
#define E_Port GPIOC

void Error_Handler(void);

#endif
