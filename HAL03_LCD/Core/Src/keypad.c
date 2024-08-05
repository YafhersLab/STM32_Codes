#include "keypad.h"
#include "stm32f4xx_hal.h"

void Keypad_Init(void){
	//SALIDAS
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = ROW1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ROW1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ROW2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ROW2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ROW3_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ROW3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ROW4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ROW4_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(ROW1_PORT, ROW1_PIN, 0);
	HAL_GPIO_WritePin(ROW2_PORT, ROW1_PIN, 0);
	HAL_GPIO_WritePin(ROW3_PORT, ROW1_PIN, 0);
	HAL_GPIO_WritePin(ROW4_PORT, ROW1_PIN, 0);

	//ENTRADAS
	GPIO_InitStruct.Pin = COL1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(COL1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = COL2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(COL2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = COL3_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(COL3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = COL4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(COL4_PORT, &GPIO_InitStruct);
}

char Keypad_Read(void){
	char keys[4][4] = {
			{'1', '2', '3', 'A'},
			{'4', '5', '6', 'B'},
			{'7', '8', '9', 'C'},
			{'*', '0', '#', 'D'}};

	for(int i = 0; i < 4; i++){
		switch(i){
		  case 0:
			HAL_GPIO_WritePin(ROW1_PORT, ROW1_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(ROW2_PORT, ROW2_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(ROW3_PORT, ROW3_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(ROW4_PORT, ROW4_PIN, GPIO_PIN_SET);
			break;

		  case 1:
			HAL_GPIO_WritePin(ROW1_PORT, ROW1_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(ROW2_PORT, ROW2_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(ROW3_PORT, ROW3_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(ROW4_PORT, ROW4_PIN, GPIO_PIN_SET);
			break;

		  case 2:
			HAL_GPIO_WritePin(ROW1_PORT, ROW1_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(ROW2_PORT, ROW2_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(ROW3_PORT, ROW3_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(ROW4_PORT, ROW4_PIN, GPIO_PIN_SET);
			break;

		  case 3:
			HAL_GPIO_WritePin(ROW1_PORT, ROW1_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(ROW2_PORT, ROW2_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(ROW3_PORT, ROW3_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(ROW4_PORT, ROW4_PIN, GPIO_PIN_RESET);
			break;
	}

	if(HAL_GPIO_ReadPin(COL1_PORT, COL1_PIN) == GPIO_PIN_RESET)
	  return keys[i][0];
	if(HAL_GPIO_ReadPin(COL2_PORT, COL2_PIN) == GPIO_PIN_RESET)
	  return keys[i][1];
	if(HAL_GPIO_ReadPin(COL3_PORT, COL3_PIN) == GPIO_PIN_RESET)
	  return keys[i][2];
	if(HAL_GPIO_ReadPin(COL4_PORT, COL4_PIN) == GPIO_PIN_RESET)
	  return keys[i][3];
	}

	return 0;
}
