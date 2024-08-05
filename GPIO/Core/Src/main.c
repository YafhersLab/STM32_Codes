//INCLUDES
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//DEFINES

/*
 * OFFSET -> (0x40020000 -0x40000000 = 0x20000) + 0x14
 * OFFSET = 0x20014
 * siguiendo la formula...
 * BWOFFSET = OFFSET * 32 + BIT * 4
 * BWOFFSET = (0x20014 * 0x20) + (0x5 *0x4)
 * BWOFFSET = 0x400294
 * BWADDRESS = BWOFFSET + DIRECCION_BID_BAND
 */

#define ODR_PA5 *(volatile uint8_t *)(0x400294 + PERIPH_BB_BASE)
//mas rapido asi
#define BW_OFFSET(BASE_ADDR, BIT)	((BASE_ADDR - PERIPH_BASE)* 0x20 + BIT * 0x4)
#define IDR_PC13 *(volatile uint8_t *)(PERIPH_BB_BASE + BW_OFFSET(GPIOC_BASE + 0x10, 0))

//VARIABLES GLOBALES
uint8_t pc13 = 0;

//PROTOTIPOS DE FUNCIONES
void PLL_Config(void);

int main(void){
	//INICIO
	printf("Hola Mundo desde STM32 Cortex M4\r\n");
	printf("Yafhers Mendoza C.\r\n");
	printf("%s  %s\r\n",__TIME__,__DATE__);

	//PC13 -> User Button, PA5 -> User Led

	//1. Habilite el reloj del GPIO (en las STM32F4 GPIO esta en AHB1)
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

	//2. Configurar el modo del pin
	GPIOA -> MODER &=~ GPIO_MODER_MODE5;			//restablecer el modo del pin PA5
	GPIOA -> MODER |= GPIO_MODER_MODE5_0;			//PA5 salida
	GPIOC -> MODER &=~ GPIO_MODER_MODE13;			//restablecer el modo del pin PC13
	GPIOC -> MODER &=~ GPIO_MODER_MODE13;			//PC13 entrada

	//3. Definir el tipo de salida
	GPIOA -> OTYPER &= (GPIO_OTYPER_OT5);			//PA5 modo push pull
	//no toco nada del PC13 pues no es necesario configurar modo ya que es una entrada

	//4. Configurar la velocidad de conmutacion
	GPIOA -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5;		//PA5 very high speed

	//5. Configurar la resistencia Pull Up / Pull Down
	GPIOA -> PUPDR &=~ GPIO_PUPDR_PUPD5;			//PA5 sin resistencia
	GPIOC -> PUPDR &=~ GPIO_PUPDR_PUPD13;			//PC13 sin resistencia

	/***********************************************************************************************/

	/*	prender y apagar pines con ODR y BSRR

	GPIOA -> ODR |= GPIO_ODR_OD5;					//set PA5
	GPIOA -> ODR &=~ GPIO_ODR_OD5;					//reset PA5

	GPIOA -> BSRR |= GPIO_BSRR_BS5;					//set PA5
	GPIOA -> BSRR |= GPIO_BSRR_BR5;					//reset PA5 */

    /* 	leer el valor de un pulsador y prender un led
	for(;;){
		pc13 = (GPIOC -> IDR >> 13) & 0x1;
		if(pc13 == 1){
			GPIOA -> BSRR |= GPIO_BSRR_BS5;}
		else{
			GPIOA -> BSRR |= GPIO_BSRR_BR5;}
	}*/

	//leer el valor de un pulsador y prender un led pero con bit band
	for(;;){
		pc13 = IDR_PC13;
		if(pc13 == 1){
			//GPIOA -> BSRR |= GPIO_BSRR_BS5;
			ODR_PA5 = 1;
		}
		else{
			//GPIOA -> BSRR |= GPIO_BSRR_BR5;
			ODR_PA5 = 0;
		}
	}

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
