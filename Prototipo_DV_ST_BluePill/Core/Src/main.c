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

TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint32_t adcValues[9] = {0};
int adcDMASize = sizeof(adcValues) / sizeof(adcValues[0]);
char dataTX[50] = {0};
char dataRX[50] = {0};
int cont = 0;

//Variables del QTR8A para el control
uint32_t lineas[8];
uint32_t fondos[8];
uint32_t umbral[8];
uint32_t digitales[8];

//Variables para el PID
uint32_t pos;
uint32_t ultima_posicion;
uint16_t error = 0;
uint16_t lastError = 0;
uint16_t error1 = 0;
uint16_t error2 = 0;
uint16_t error3 = 0;
uint16_t error4 = 0;
uint16_t error5 = 0;
uint16_t error6 = 0;
uint16_t setpoint = 350;
uint16_t pid_p, pid_d, pid_i;
uint16_t PID = 0;
uint16_t kp = 0;
uint16_t kd = 0;
uint16_t ki = 0;
uint16_t vel = 20;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */
void motores(int velFirst, int velSecond);
void leer_lineas();
void leer_fondos();
void calcular_umbral();
void posicion();
void imprimirDigitales();
void imprimirAnalogicos();
void seguidorPID();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void motores(int velFirst, int velSecond){

	if(velFirst >= 0){
		TIM4 -> CCR2 = velFirst;
		TIM4 -> CCR2 = velFirst;
		HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, SET);
		HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, RESET);
	}
	else if(velFirst < 0){
		velFirst *= -1;
		TIM4 -> CCR2 = velFirst;
		TIM4 -> CCR2 = velFirst;
		HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, RESET);
		HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, SET);
	}

	if(velSecond >= 0){
		TIM4 -> CCR4 = velSecond;
		TIM4 -> CCR4 = velSecond;
		HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, SET);
		HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, RESET);
	}
	if(velSecond < 0){
		velSecond *= -1;
		TIM4 -> CCR4 = velSecond;
		TIM4 -> CCR4 = velSecond;
		HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, RESET);
		HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, SET);
	}

}

void leer_lineas(){

	memset(dataTX, 0 , strlen(dataTX));
	sprintf(dataTX, "Leyendo la linea...\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)dataTX, strlen(dataTX), HAL_MAX_DELAY);

	for(uint8_t i = 0; i < 8; i ++){
		lineas[i] = adcValues[i];
	}

	memset(dataTX, 0,  strlen(dataTX));
	sprintf(dataTX, "%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld\r\n",lineas[0], lineas[1], lineas[2], lineas[3], lineas[4], lineas[5], lineas[6], lineas[7]);
	HAL_UART_Transmit(&huart2, (uint8_t *)dataTX, strlen(dataTX), HAL_MAX_DELAY);

}

void leer_fondos(){

	memset(dataTX, 0 , strlen(dataTX));
	sprintf(dataTX, "\nLeyendo el fondo...\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)dataTX, strlen(dataTX), HAL_MAX_DELAY);

	for(uint8_t i = 0; i < 8; i ++){
		fondos[i] = adcValues[i];
	}

	memset(dataTX, 0,  strlen(dataTX));
	sprintf(dataTX, "%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld\r\n",fondos[0], fondos[1], fondos[2], fondos[3], fondos[4], fondos[5], fondos[6], fondos[7]);
	HAL_UART_Transmit(&huart2, (uint8_t *)dataTX, strlen(dataTX), HAL_MAX_DELAY);

}

void calcular_umbral(){

	memset(dataTX, 0 ,  strlen(dataTX));
	sprintf(dataTX, "\nCalculando el umbral...\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)dataTX, strlen(dataTX), HAL_MAX_DELAY);

	for(uint8_t i = 0; i < 8; i ++){
		umbral[i] = (lineas[i] + fondos[i]) / 2;
	}

	memset(dataTX, 0,  strlen(dataTX));
	sprintf(dataTX, "%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld\r\n\nGo! :)\n",umbral[0], umbral[1], umbral[2], umbral[3], umbral[4], umbral[5], umbral[6], umbral[7]);
	HAL_UART_Transmit(&huart2, (uint8_t *)dataTX, strlen(dataTX), HAL_MAX_DELAY);

}

void posicion() {

  for(uint8_t i = 0; i < 8; i++){

    if(adcValues[i] >= umbral[i]){
      digitales[i] = 1;
    }
    else{
      digitales[i] = 0;
    }
  }

  uint32_t sumaPonderada = digitales[0] * 0 + digitales[1] * 100 + digitales[2] * 200 + digitales[3] * 300 + digitales[4] * 400 + digitales[5] * 500 + digitales[6] * 600 + digitales[7] * 700;
  uint32_t sumaTotal = digitales[0] + digitales[1] + digitales[2] + digitales[3] + digitales[4] + digitales[5] + digitales[6] + digitales[7];
  pos = sumaPonderada / sumaTotal;

  if (ultima_posicion <= 50 && pos == -1) pos = 0;
  if (ultima_posicion >= 650 && pos == -1) pos = 700;
  ultima_posicion = pos;

  imprimirAnalogicos();
  //imprimirDigitales();

}

void imprimirDigitales(){

	memset(dataTX, 0,  strlen(dataTX));
	sprintf(dataTX, "%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld\r\n",digitales[0], digitales[1], digitales[2], digitales[3], digitales[4], digitales[5], digitales[6], digitales[7], pos);
	HAL_UART_Transmit(&huart2, (uint8_t *)dataTX, strlen(dataTX), HAL_MAX_DELAY);

}

void imprimirAnalogicos(){

	memset(dataTX, 0,  strlen(dataTX));
	sprintf(dataTX, "%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld\r\n",adcValues[0], adcValues[1], adcValues[2], adcValues[3], adcValues[4], adcValues[5], adcValues[6], adcValues[7], pos);
	HAL_UART_Transmit(&huart2, (uint8_t *)dataTX, strlen(dataTX), HAL_MAX_DELAY);

}

void seguidorPID(){

	error = pos - setpoint;

	pid_p = error * kp;
	pid_d = (error - lastError) * kd;
	pid_i = (error1 + error2 + error3 + error4 + error5 + error6) * ki;

  	error6 = error5;
  	error5 = error4;
  	error4 = error3;
  	error3 = error2;
  	error2 = error1;
  	error1 = error;
  	lastError = error;

  	PID = pid_p + pid_d + pid_i;
  	motores(vel + PID, vel - PID);

}

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
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
  HAL_ADC_Start_DMA(&hadc1, adcValues, adcDMASize);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while(HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin) == GPIO_PIN_RESET);
  leer_lineas();
  HAL_Delay(1000);
  while(HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin) == GPIO_PIN_RESET);
  leer_fondos();
  HAL_Delay(1000);
  while(HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin) == GPIO_PIN_RESET);
  calcular_umbral();
  HAL_Delay(1000);
  memset(dataRX, 0, 8);
  while(cont == 0)
   {
  	 HAL_UART_Receive_IT(&huart3, (uint8_t*)dataRX, 8);
   }

  while (1)
  {
	  HAL_GPIO_TogglePin(Led_Interno_GPIO_Port, Led_Interno_Pin);
	  //HAL_UART_Receive_IT(&huart3, (uint8_t*)dataRX, 8);
	  HAL_Delay(500);
    /* USER CODE END WHILE */

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 9;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_REGULAR_RANK_9;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 72;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 100 - 1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

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
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Led_Interno_GPIO_Port, Led_Interno_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, AIN2_Pin|AIN1_Pin|BIN1_Pin|BIN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Led_Interno_Pin */
  GPIO_InitStruct.Pin = Led_Interno_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Led_Interno_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : AIN2_Pin AIN1_Pin BIN1_Pin BIN2_Pin */
  GPIO_InitStruct.Pin = AIN2_Pin|AIN1_Pin|BIN1_Pin|BIN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Button_Pin */
  GPIO_InitStruct.Pin = Button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Button_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  cont++;
  memset(dataTX, 0 , strlen(dataTX));
  sprintf(dataTX, "\nData recibida: %s\r", dataRX);
  HAL_UART_Transmit(&huart2, (uint8_t*) dataTX, sizeof(dataTX), HAL_MAX_DELAY);
  memset(dataRX, 0 , 8);
  HAL_UART_Receive_IT(&huart3, (uint8_t*)dataRX, 8);
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
