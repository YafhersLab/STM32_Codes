/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t LM35;
uint8_t dataTX[32];
int count = 0;
int temperature;
int centenas, decenas, unidades;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void cero(void);
static void uno(void);
static void dos(void);
static void tres(void);
static void cuatro(void);
static void cinco(void);
static void seis(void);
static void siete(void);
static void ocho(void);
static void nueve(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADC_Start_DMA(&hadc1, &LM35, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	  //temperature = LM35 * 0.08058;
	  centenas = LM35 / 100;
	  decenas = (LM35 / 10) % 10;
	  unidades = LM35 % 10;

	  HAL_GPIO_WritePin(DIG_1_GPIO_Port, DIG_1_Pin, RESET);
	  HAL_GPIO_WritePin(DIG_2_GPIO_Port, DIG_1_Pin, SET);
	  HAL_GPIO_WritePin(DIG_3_GPIO_Port, DIG_1_Pin, SET);
	  switch(unidades){
	  case 0:
		  cero();
		  break;
	  case 1:
		  uno();
		  break;
	  case 2:
		  dos();
		  break;
	  case 3:
		  tres();
		  break;
	  case 4:
		  cuatro();
		  break;
	  case 5:
	  	  cinco();
	  	break;
	  case 6:
	  	  seis();
	  	break;
	  case 7:
		  siete();
		  break;
	  case 8:
		  ocho();
		  break;
	  case 9:
		  nueve();
		  break;
	  default:
		  cero();
	  }
	  HAL_Delay(10);

	  HAL_GPIO_WritePin(DIG_1_GPIO_Port, DIG_1_Pin, SET);
	  HAL_GPIO_WritePin(DIG_2_GPIO_Port, DIG_1_Pin, RESET);
	  HAL_GPIO_WritePin(DIG_3_GPIO_Port, DIG_1_Pin, SET);
	  switch(decenas){
	  case 0:
		  cero();
		  break;
	  case 1:
		  uno();
		  break;
	  case 2:
		  dos();
		  break;
	  case 3:
		  tres();
		  break;
	  case 4:
		  cuatro();
		  break;
	  case 5:
		  cinco();
		break;
	  case 6:
		  seis();
		break;
	  case 7:
		  siete();
		  break;
	  case 8:
		  ocho();
		  break;
	  case 9:
		  nueve();
		  break;
	  default:
		  cero();
	  }
	  HAL_Delay(10);

	  HAL_GPIO_WritePin(DIG_1_GPIO_Port, DIG_1_Pin, SET);
	  HAL_GPIO_WritePin(DIG_2_GPIO_Port, DIG_1_Pin, SET);
	  HAL_GPIO_WritePin(DIG_3_GPIO_Port, DIG_1_Pin, RESET);
	  switch(centenas){
	  case 0:
		  cero();
		  break;
	  case 1:
		  uno();
		  break;
	  case 2:
		  dos();
		  break;
	  case 3:
		  tres();
		  break;
	  case 4:
		  cuatro();
		  break;
	  case 5:
		  cinco();
		break;
	  case 6:
		  seis();
		break;
	  case 7:
		  siete();
		  break;
	  case 8:
		  ocho();
		  break;
	  case 9:
		  nueve();
		  break;
	  default:
		  cero();
	  }
	  HAL_Delay(10);
    /* USER CODE BEGIN 3 */


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Led_Integrado_Pin|DIG_1_Pin|DIG_2_Pin|DIG_3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SEG_A_Pin|SEG_B_Pin|SEG_C_Pin|SEG_D_Pin
                          |SEG_E_Pin|SEG_F_Pin|SEG_G_Pin|SEG_DP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Boton_Integrado_Pin */
  GPIO_InitStruct.Pin = Boton_Integrado_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Boton_Integrado_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Led_Integrado_Pin DIG_1_Pin DIG_2_Pin DIG_3_Pin */
  GPIO_InitStruct.Pin = Led_Integrado_Pin|DIG_1_Pin|DIG_2_Pin|DIG_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SEG_A_Pin SEG_B_Pin SEG_C_Pin SEG_D_Pin
                           SEG_E_Pin SEG_F_Pin SEG_G_Pin SEG_DP_Pin */
  GPIO_InitStruct.Pin = SEG_A_Pin|SEG_B_Pin|SEG_C_Pin|SEG_D_Pin
                          |SEG_E_Pin|SEG_F_Pin|SEG_G_Pin|SEG_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static void uno(void){
	HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, RESET); //a
	HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, SET);   //b
	HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, SET);   //c
	HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, RESET); //d
	HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, RESET); //e
	HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, RESET); //f
	HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, RESET); //g
}

static void dos(void){
	HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, SET); //a
	HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, SET);   //b
	HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, RESET);   //c
	HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, SET); //d
	HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, SET); //e
	HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, RESET); //f
	HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, SET); //g
}

static void tres(void){
	HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, SET); //a
	HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, SET);   //b
	HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, SET);   //c
	HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, SET); //d
	HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, RESET); //e
	HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, RESET); //f
	HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, SET); //g
}

static void cuatro(void){
	HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, RESET); //a
	HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, RESET);   //b
	HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, SET);   //c
	HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, SET); //d
	HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, RESET); //e
	HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, SET); //f
	HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, SET); //g
}

static void cinco(void){
	HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, SET); //a
	HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, RESET);   //b
	HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, SET);   //c
	HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, SET); //d
	HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, RESET); //e
	HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, SET); //f
	HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, SET); //g
}

static void seis(void){
	HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, SET); //a
	HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, RESET);   //b
	HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, SET);   //c
	HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, SET); //d
	HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, SET); //e
	HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, SET); //f
	HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, SET); //g
}

static void siete(void){
	HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, SET); //a
	HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, SET);   //b
	HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, SET);   //c
	HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, RESET); //d
	HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, RESET); //e
	HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, RESET); //f
	HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, RESET); //g
}

static void ocho(void){
	HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, SET); //a
	HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, SET);   //b
	HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, SET);   //c
	HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, SET); //d
	HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, SET); //e
	HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, SET); //f
	HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, SET); //g
}

static void nueve(void){
	HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, SET); //a
	HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, SET);   //b
	HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, SET);   //c
	HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, RESET); //d
	HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, RESET); //e
	HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, SET); //f
	HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, SET); //g
}

static void cero(void){
	HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, SET); //a
	HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, SET);   //b
	HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, SET);   //c
	HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, SET); //d
	HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, SET); //e
	HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, SET); //f
	HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, RESET); //g
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
