/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Boton_Interno_Pin GPIO_PIN_13
#define Boton_Interno_GPIO_Port GPIOC
#define USART2_TX_Pin GPIO_PIN_2
#define USART2_TX_GPIO_Port GPIOA
#define USART2_RX_Pin GPIO_PIN_3
#define USART2_RX_GPIO_Port GPIOA
#define Led_Interno_Pin GPIO_PIN_5
#define Led_Interno_GPIO_Port GPIOA
#define BIN1_Second_Pin GPIO_PIN_5
#define BIN1_Second_GPIO_Port GPIOC
#define AIN2_First_Pin GPIO_PIN_2
#define AIN2_First_GPIO_Port GPIOB
#define AIN1_Second_Pin GPIO_PIN_10
#define AIN1_Second_GPIO_Port GPIOB
#define BIN2_Second_Pin GPIO_PIN_12
#define BIN2_Second_GPIO_Port GPIOB
#define BIN2_First_Pin GPIO_PIN_14
#define BIN2_First_GPIO_Port GPIOB
#define BIN1_First_Pin GPIO_PIN_15
#define BIN1_First_GPIO_Port GPIOB
#define PWMB_First_Pin GPIO_PIN_7
#define PWMB_First_GPIO_Port GPIOC
#define PWMA_First_Pin GPIO_PIN_8
#define PWMA_First_GPIO_Port GPIOC
#define AIN1_First_Pin GPIO_PIN_8
#define AIN1_First_GPIO_Port GPIOA
#define AIN2_Second_Pin GPIO_PIN_9
#define AIN2_Second_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define PWMA_Second_Pin GPIO_PIN_8
#define PWMA_Second_GPIO_Port GPIOB
#define PWMB_Second_Pin GPIO_PIN_9
#define PWMB_Second_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
