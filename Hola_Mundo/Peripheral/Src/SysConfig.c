#include "SysConfig.h"

void SysConfigClock(void){
	RCC_OscInitTypeDef oscInit = {0};
	RCC_ClkInitTypeDef clkInit = {0};

	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	oscInit.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	oscInit.HSIState = RCC_HSI_ON;
	oscInit.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	oscInit.PLL.PLLState = RCC_PLL_NONE;

#if 0
	oscInit.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	oscInit.PLL.PLLState = RCC_PLL_ON;
	oscInit.PLL.PLLM = 8;
	oscInit.PLL.PLLN = 188;
	oscInit.PLL.PLLP = RCC_PLLP_DIV2;
	oscInit.PLL.PLLQ = 7;
#endif

	if (HAL_RCC_OscConfig(&oscInit) != HAL_OK){
	    Error_Handler();
	  }

	clkInit.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_SYSCLK;
	clkInit.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;	//cambiar para PLL
	clkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clkInit.APB1CLKDivider = RCC_HCLK_DIV1;
	clkInit.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&clkInit, FLASH_ACR_LATENCY_0WS) != HAL_OK){
		Error_Handler();
	}
}
