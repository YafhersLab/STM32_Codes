//INCLUDES
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//DEFINES

//VARIABLES GLOBALES

//PROTOTIPOS DE FUNCIONES
void PLL_Config(void);

int main(void){
	//INICIO
	printf("Hola Mundo desde STM32 Cortex M4\r\n");
	printf("Yafhers Mendoza C.\r\n");
	printf("%s  %s\r\n",__TIME__,__DATE__);

	PLL_Config();

    //LOOP INFINITO
	for(;;);
}

//DECLARACIÓN DE FUNCIONES
int __io_putchar(int ch){
	ITM_SendChar(ch);
	return ch;
}

void PLL_Config(void){
	//1. Apagar el PLL y esperar que la bandera se limpie
	RCC -> CR &=~ RCC_CR_PLLON;	 					//RCC -> CR &=~ (1U<24); apagar PLL
	while(RCC -> CR & RCC_CR_PLLRDY);				//esperar que la bandera se limpie del PLL
	//2. Configurar los parametros del PLL
	RCC -> CR |= RCC_CR_HSION;						//habilitar HSI
	while(!(RCC -> CR & RCC_CR_HSIRDY));				//esperar que la bandera se limpie del HSI
	RCC -> PLLCFGR &=~ RCC_PLLCFGR_PLLSRC;			//selecciono HSI como fuente de PLL
	/* HSI -> 16MHz, hallamos M, N y P del PLL
	 * 16Mhz -> / M <1-2MHz> -> * N <100-432MHz>
	 * SYSCLK = ((16MHz / 8) * 168) / 2 = 168MHz
	 * M = 8, N = 168, P = 2 */
	RCC -> PLLCFGR &=~ RCC_PLLCFGR_PLLM;			//limpio M
	RCC -> PLLCFGR |= 8U<<RCC_PLLCFGR_PLLM_Pos;		//M = 8
	RCC -> PLLCFGR &=~ RCC_PLLCFGR_PLLN;			//limpio N
	RCC -> PLLCFGR |= 168U<<RCC_PLLCFGR_PLLN_Pos;	//N = 168
	RCC -> PLLCFGR &=~ RCC_PLLCFGR_PLLP;			//limpio P y P = 8
	//3. Configurar latencia de la flash
	FLASH -> ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;
	FLASH -> ACR |= FLASH_ACR_LATENCY_5WS;
	//4. Encender el PLL
	RCC -> CR |= RCC_CR_PLLON;						//habilitar PLL
	while(!(RCC -> CR & RCC_CR_PLLRDY));			//esperar que se active PLL
	//5. Configurar los prescaler de los buses APB1 y APB2
	RCC -> CFGR &=~ RCC_CFGR_PPRE1;					//limpiamos
	RCC -> CFGR |= 0X5U<<RCC_CFGR_PPRE1_Pos;		//prescaler APB1 / 4
	RCC -> CFGR &=~ RCC_CFGR_PPRE2;					//limpiamos
	RCC -> CFGR |= RCC_CFGR_PPRE2_2;				//prescaler APB2 / 2
	//6. Seleccionar PLL como fuente de reloj
	RCC -> CFGR |= RCC_CFGR_SW_PLL;					//activo PLL
	while(!(RCC -> CFGR & RCC_CFGR_SWS_PLL));		//espero que se active PLL
	SystemCoreClockUpdate();						//actualizar el reloj
	return;
}
