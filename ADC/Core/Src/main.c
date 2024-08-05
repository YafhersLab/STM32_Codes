//INCLUDES
#include "stm32f4xx.h"
#include "nanomodbus.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//DEFINES

//VARIABLES GLOBALES
extern volatile uint32_t uwTicks;
extern volatile uint8_t dataBT;
extern volatile uint16_t adcData;
uint8_t pushDown = 1;
uint8_t pushUp = 1;
uint32_t ticks;
uint8_t flag = 0;
uint8_t valor;

//PROTOTIPOS DE FUNCIONES
void saludar(void);
void GPIO_Config(void);
void PLL_Config(void);
void EXTI_Config(void);
void USART2_Config(void);
void USART6_Config(void);
void USART2_putc(char ch);
void Timer5_Config(void);
void Timer3_ConfigPWM(void);
void ADC_Config(void);
char USART2_getc(void);
void delay_ms(uint32_t d);
void moverServo(int val);
uint8_t UART_Transmit(USART_TypeDef *USARTx, uint8_t *pData, uint16_t size, uint32_t timeout);
uint8_t UART_Receive(USART_TypeDef *USARTx, uint8_t *pData, uint16_t size, uint32_t timeout);
uint8_t button_read(uint8_t current);
uint16_t ADC_getData(uint8_t ch);
uint32_t getTicks();
void USART2_IRQHandler(void);

int main(void){
	//Setup
	//PLL_Config(); //48MHz
	GPIO_Config();
	SysTick_Config(16000);
	USART2_Config();
	Timer5_Config();
	ADC_Config();

	//Loop
	for(;;){

	}
}

void saludar(void){
	printf("Hola Mundo desde STM32 Cortex M4\r\n");
	printf("Yafhers Mendoza C.\r\n");
	printf("%s  %s\r\n",__TIME__,__DATE__);
}

void GPIO_Config(void){
	//Outputs: PA9 (Verde), PA8 (Azul), PB6 (Rojo), PB5 (Naranja), PB4 (Buzzer), PA5 (Led Interno)
	//Inputs: PC3 (Down), PC2 (Up), PC13 (Boton Interno)

	//1. Habilitamos el reloj del GPIOA, GPIOB, GPIOC (en las STM32F4 GPIO esta en AHB1)
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

	//2. Configuramos el modo del pin
	//Outputs
	GPIOA -> MODER &=~ (GPIO_MODER_MODE8 | GPIO_MODER_MODE9 | GPIO_MODER_MODE5);						//restablecer el modo del pin PA8, PA9, PA5
	GPIOB -> MODER &=~ (GPIO_MODER_MODE6 | GPIO_MODER_MODE4 | GPIO_MODER_MODE5);						//restablecer el modo del pin PB6, PB4 y PB5
	GPIOA -> MODER |= (GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0 | GPIO_MODER_MODE5_0);					//PA8, PA9 y PA5 salida
	GPIOB -> MODER |= (GPIO_MODER_MODE6_0 |  GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0);					//PB6, PB4 y PB5 salida

	//Inputs
	GPIOC -> MODER &=~ (GPIO_MODER_MODE2 | GPIO_MODER_MODE3 | GPIO_MODER_MODE13);						//restablecer el modo del pin PC2, PC3 y PC13 y ponerlo como entrada

	//3. Definimos el tipo de salida
	GPIOA -> OTYPER &=~ (GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9  |GPIO_OTYPER_OT5);							//PA2, PA3, PA5 modo push pull
	GPIOB -> OTYPER &=~ (GPIO_OTYPER_OT6 | GPIO_OTYPER_OT5 | GPIO_OTYPER_OT4);							//PB3, PB4, PB5 modo push pull
	//no toco nada del PC13, PC3 y PC2 pues no es necesario configurar modo ya que son entradas

	//4. Configurar la velocidad de conmutacion
	GPIOA -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR5;		//PA2, PA3 y PA5 very high speed
	GPIOB -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR4;		//PB3, PB4 y PB5 very high speed
	//no toco nada del PC13, PC3 y PC2 pues no es necesario configurar modo ya que son entradas

	//5. Configurar la resistencia Pull Up / Pull Down
	GPIOA -> PUPDR &=~ (GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9 | GPIO_PUPDR_PUPD5);						//PA5, PA8 y PA9 sin resistencia
	GPIOB -> PUPDR &=~ (GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD4 | GPIO_PUPDR_PUPD5);						//PB4, PB5 y PB6 sin resistencia
	GPIOC -> PUPDR &=~ GPIO_PUPDR_PUPD13;																//PC13 sin resistencia
	GPIOC -> PUPDR |= GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3;												//PC2 y PC3 pull up
}

void EXTI_Config(void){
	//Interrupcion Externa: PC13

	//1. Habilitamos el reloj del GPIO y SYSCFG
	RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN;																//habilitamos el reloj del SYSCFG
	//RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;															//habilitamos el reloj del GPIO
	//GPIOC -> MODER &=~ GPIO_MODER_MODE13;																//PC13 como entrada

	//2. Configuramos el disparador de borde
	EXTI -> FTSR |= EXTI_FTSR_TR13;																		//falling, flanco ascendente

	//3. Configuramos la entrega de interrupcion del EXTIx al procesador
	EXTI -> IMR |= EXTI_IMR_IM13;																		//configuramos bit de enmascaramiento sin mascara

	//4. Asignamos el pin de interrupcion a la linea EXTIx
	SYSCFG -> EXTICR[3] &=~ SYSCFG_EXTICR4_EXTI13;														//limpiamos
	SYSCFG -> EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;													//asignamos el PC13

	//5. Configuramos la prioridad de la interrupcion
	EXTI -> PR |= EXTI_PR_PR13;																			//interrupcion 13 como pendiente
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);																//limpiar bandera de interrupcion
	NVIC_SetPriority(EXTI15_10_IRQn, 3);																//asignamos la prioridad

	//6. Habilitar la interrupcion de la EXTIx
	NVIC_EnableIRQ(EXTI15_10_IRQn);																		//habilitamos la interrupcion en el procesador
}

void PLL_Config(void){
	//1. Apagar el PLL y esperar que la bandera se limpie
	RCC -> CR &=~ RCC_CR_PLLON;	 																		//RCC -> CR &=~ (1U<24); apagar PLL
	while(RCC -> CR & RCC_CR_PLLRDY);																	//esperar que la bandera se limpie del PLL

	//2. Configurar los parametros del PLL
	RCC -> CR |= RCC_CR_HSION;																			//habilitar HSI
	while(!(RCC -> CR & RCC_CR_HSIRDY));																//esperar que la bandera se limpie del HSI
	RCC -> PLLCFGR &=~ RCC_PLLCFGR_PLLSRC;																//selecciono HSI como fuente de PLL
	/* HSI -> 16MHz, hallamos M, N y P del PLL
	 * 16Mhz -> / M <1-2MHz> -> * N <100-432MHz> / P <0-100MHz>
	 * SYSCLK = ((16MHz / 8) * 168) / 8 = 42MHz
	 * M = 8, N = 168, P = 8 */
	RCC -> PLLCFGR &=~ RCC_PLLCFGR_PLLM;																//limpio M
	RCC -> PLLCFGR |= 8U<<RCC_PLLCFGR_PLLM_Pos;															//M = 8
	RCC -> PLLCFGR &=~ RCC_PLLCFGR_PLLN;																//limpio N
	RCC -> PLLCFGR |= 168U<<RCC_PLLCFGR_PLLN_Pos;														//N =168
	RCC -> PLLCFGR &=~ RCC_PLLCFGR_PLLP;																//limpio P
	RCC -> PLLCFGR |= RCC_PLLCFGR_PLLP_0 | RCC_PLLCFGR_PLLP_1;											//P = 8

	//3. Configurar latencia de la flash
	FLASH -> ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;
	FLASH -> ACR |= FLASH_ACR_LATENCY_1WS;

	//4. Encender el PLL
	RCC -> CR |= RCC_CR_PLLON;																			//habilitar PLL
	while(!(RCC -> CR & RCC_CR_PLLRDY));																//esperar que se active PLL

	//5. Configurar los prescaler de los buses APB1 y APB2
	RCC -> CFGR &=~ RCC_CFGR_PPRE1;																		//limpiamos
	RCC -> CFGR |= 0X5U<<RCC_CFGR_PPRE1_Pos;															//prescaler APB1 / 4 (<50MHz)
	RCC -> CFGR &=~ RCC_CFGR_PPRE2;																		//limpiamos
	RCC -> CFGR |= RCC_CFGR_PPRE2_2;																	//prescaler APB2 / 2

	//6. Seleccionar PLL como fuente de reloj
	RCC -> CFGR |= RCC_CFGR_SW_PLL;																		//activo PLL como fuente de reloj
	while(!(RCC -> CFGR & RCC_CFGR_SWS_PLL));															//espero que se active PLL
	SystemCoreClockUpdate();																			//actualizar el reloj
	return;
}

void USART2_Config(void){
	//1. Configuramos los pines RX y TX 	PA2 -> TX,	PA3 -> RX
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;																//habilito el reloj del GPIOA
	GPIOA -> MODER &=~ (GPIO_MODER_MODE2 | GPIO_MODER_MODE3);											//lipiamos el registro MODER
	GPIOA -> MODER |= GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1;											//establezco el modo alternativo del PA2 y PA3
	GPIOA -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3;								//establezco PA2 y PA3 a la maxima velocidad
	GPIOA -> AFR[0] &=~ (GPIO_AFRL_AFRL2 | GPIO_AFRL_AFRL3);											//limpiamos el AFRL del PA2 y PA3, usamos el 0 ya que el UART2 esta del 0 al 7
	GPIOA -> AFR[0] |= 0x7 << GPIO_AFRL_AFSEL2_Pos | 0x7 << GPIO_AFRL_AFSEL3_Pos;						//asigno la funcion alternativa #7 (USART2) para el PA2 y PA3

	//2. Configuramos el USART2 (depende del dispositivo a trabajar)
	RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;																//habilito el reloj del USART2
	USART2 -> CR1 = 0;																					//limpio registro CR1
	USART2 -> CR1 |= USART_CR1_TE | USART_CR1_RE;														//habilito transmisión y recepción

	//3. Calculamos el Baud Rate (9600), OVER8 = 0
	/*
	 * USART_DIV = 16000000 / (16*9600)
	 * USART_DIV = 104.166666
	 * parte entera: d104 = x68
	 * parte decimal: 0.166666 * 16 = d2.66665
	 * parte decimal (entero mas cercano) : d3 = x3
	 * USART_BRR = 0x683
	 */
	USART2 -> BRR = 0X683;																				//asigno el BRR previamente calculado
	USART2 -> CR1 |= USART_CR1_UE;																		//habilitar UART
	USART2 -> CR1 |= USART_CR1_RXNEIE;																	//habilito la interrupcion por recepcion
	NVIC_ClearPendingIRQ(USART2_IRQn);																	//limpiar bandera de interrupcion
	NVIC_SetPriority(USART2_IRQn, 2);																	//asignamos la prioridad
	NVIC_EnableIRQ(USART2_IRQn);																		//habilito la IRQ de USART2 en la NVIC
}

void USART6_Config(void){
	//1. Configuramos los pines RX y TX 	PA11 -> TX,	PA12 -> RX
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;																//habilito el reloj del GPIOA
	GPIOA -> MODER &=~ (GPIO_MODER_MODE11 | GPIO_MODER_MODE12);											//lipiamos el registro MODER
	GPIOA -> MODER |= GPIO_MODER_MODE11_1 | GPIO_MODER_MODE12_1;										//establezco el modo alternativo del PA2 y PA3
	GPIOA -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11_0 | GPIO_OSPEEDER_OSPEEDR12_0;							//establezco PA2 y PA3 a la maxima velocidad
	GPIOA -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11_1 | GPIO_OSPEEDER_OSPEEDR12_1;
	GPIOA -> AFR[1] &=~	(GPIO_AFRH_AFRH3 | GPIO_AFRH_AFRH2);											//limpiamos el AFRL del PA2 y PA3, usamos el 0 ya que el UART2 esta del 0 al 7
	GPIOA -> AFR[1] |= 0x8 << GPIO_AFRH_AFSEL11_Pos | 0x8 << GPIO_AFRH_AFSEL12_Pos;						//asigno la funcion alternativa #7 (USART2) para el PA2 y PA3

	//2. Configuramos el USART6 (depende del dispositivo a trabajar)
	RCC -> APB2ENR |= RCC_APB2ENR_USART6EN;																//habilito el reloj del USART2
	USART6 -> CR1 = 0;																					//limpio registro CR1
	USART6 -> CR1 |= USART_CR1_TE | USART_CR1_RE;														//habilito transmisión y recepción

	//3. Calculamos el Baud Rate (9600), OVER8 = 0
	USART6 -> BRR = 0X683;																				//asigno el BRR previamente calculado
	USART6 -> CR1 |= USART_CR1_UE;																		//habilitar UART
	USART6 -> CR1 |= USART_CR1_RXNEIE;																	//habilito la interrupcion por recepcion
	NVIC_ClearPendingIRQ(USART6_IRQn);																	//limpiar bandera de interrupcion
	NVIC_SetPriority(USART6_IRQn, 2);																	//asignamos la prioridad
	NVIC_EnableIRQ(USART6_IRQn);																		//habilito la IRQ de USART2 en la NVIC
}

int __io_putchar(int ch){
	//Serial Wire Viewer ITM
	ITM_SendChar(ch);

	//UART
	while(!(USART2 -> SR & USART_SR_TXE));																//verifica que no hayas datos en el flag de transmision
	USART2 -> DR = (uint8_t)(ch & 0xFF);																//escribo en el data register
	return ch;																							//SR -> Status Register, DR -> Data Register
}

void delay_ms(uint32_t d){
	uint32_t ticksStart = getTicks();
	while(getTicks() - ticksStart <= d);
}

uint8_t UART_Transmit(USART_TypeDef *USARTx, uint8_t *pData, uint16_t size, uint32_t timeout){
	//esto solo sirve para los 8 bits, no sirve si hay paridad
	uint32_t tickStart = getTicks();
	while((getTicks() - tickStart) <= timeout){
		if(USARTx -> SR & USART_SR_TXE){
			USARTx -> DR = *pData;
			pData += 1;
			size -= 1;
			if(size == 0){
				return 0;																				//datos enviados correctamente
			}
		}
	}
	return 1;																							//datos no enviados correctamente
}

uint8_t UART_Receive(USART_TypeDef *USARTx, uint8_t *pData, uint16_t size, uint32_t timeout){
	//esto solo sirve para los 8 bits, no sirve si hay paridad
	uint32_t tickStart = getTicks();
	while((getTicks() - tickStart) <= timeout){
		if(USARTx -> SR & USART_SR_RXNE){
			*pData = USARTx -> DR;
			pData += 1;
			size -= 1;
			if(size == 0){
				return 0;																				//datos enviados correctamente
			}
		}
	}
	return 1;
}

void Timer5_Config(void){
	//Configuraremos el Timer 5
	RCC -> APB1ENR |= RCC_APB1ENR_TIM5EN;																//habilito el reloj para el TIMER5
	//Update Event = 1s -> 1Hz	/	PCLK = 16MHz
	//ARR = (PCLK / (FREQ) * (PSC +1)) - 1
	//ARR = (16MHz / (1) * (1600)) - 1 = 9999
	TIM5 -> CR1 = 0;																					//limpio el registro CR1
	TIM5 -> PSC = 1600 -1;																				//establezco el prescaler
	TIM5 -> ARR = 10000 - 1;																			//estalezco el ARR para una frecuencia de 1Hz
	TIM5 -> DIER |= TIM_DIER_UIE;																		//activo la interrupcion del update interrupt
	NVIC_ClearPendingIRQ(TIM5_IRQn);																	//limpiar bandera de interrupcion
	NVIC_SetPriority(TIM5_IRQn, 2);																		//configurar prioridad
	NVIC_EnableIRQ(TIM5_IRQn);																			//activar IRQ en la NVIC
	TIM5 -> CR1 |= TIM_CR1_CEN;																			//activo el contador
}

void Timer3_ConfigPWM(void){
	RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN;																//habilito el reloj para el TIMER3
	//Update Event = 20ms -> 50Hz	/	PCLK = 16MHz
	//ARR = (PCLK / (FREQ) * (PSC +1)) - 1
	//ARR = (16MHz / (50) * (1600)) - 1 = 199
	TIM3 -> CR1 = 0;																					//limpio el registro CR1
	TIM3 -> PSC = 1600 - 1;																				//establezco el prescaler
	TIM3 -> ARR = 200 - 1;																				//estalezco el ARR para una frecuencia de 50Hz

	//Configuracion del pin PB0 TIM3 CH3 para usar el PWM
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;																//activar el reloj de GPIOB
	GPIOB -> MODER &=~ GPIO_MODER_MODE0;																//limpio el modo del PB0
	GPIOB -> MODER |= GPIO_MODER_MODE0_1;																//establezco el modo de funcion alternativa
	GPIOB -> OSPEEDR |= GPIO_OSPEEDR_OSPEED0;															//maxima velocidad en PB0
	GPIOB -> AFR[0] &=~ GPIO_AFRL_AFSEL0;																//limpio el registro de funcion alternativa
	GPIOB -> AFR[0] |= 0x2 << GPIO_AFRL_AFSEL0_Pos;														//selecciono la funcion alternativa del TIM3

	//Configuracion del canal
	TIM3 -> CCMR2 &=~ TIM_CCMR2_CC3S;																	//configuro el canal como salida
	TIM3 -> CCMR2 &=~ TIM_CCMR2_OC3M;																	//limpio el canal del modo de comparacion
	TIM3 -> CCMR2 |= 0x6 << TIM_CCMR2_OC3M_Pos;															//comparacion mode PWM1, TIMx_CNT < TIMx_CNRR
	TIM3 -> CCER &=~ TIM_CCER_CC3P;																		//ACTIVO A ALTO polaridad
	TIM3 -> CCER |= TIM_CCER_CC3E;																		//habilitar el canal de comparacion
	TIM3 -> CCR3 = 200;																					//tiempo del duty cicle
	TIM3 -> DIER |= TIM_DIER_CC3IE;																		//habilito interrupcion de comparacion
	NVIC_ClearPendingIRQ(TIM3_IRQn);																	//limpiar bandera de interrupcion
	NVIC_SetPriority(TIM3_IRQn, 3);																		//configurar prioridad
	NVIC_EnableIRQ(TIM3_IRQn);																			//activar IRQ en la NVIC
	TIM3 -> CR1 |= TIM_CR1_CEN;																			//iniciar el conteo
}

void ADC_Config(void){
	//1. Configuracion del pin PC4 -> ADC1_14
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;																//activo el reloj del GPIOC
	GPIOC -> MODER |= GPIO_MODER_MODE4;																	//selecciono el modo analogica del PC4
	GPIOC -> PUPDR |= GPIO_PUPDR_PUPD4;																	//desactivo las resistencias pull up / down

	//2. Configuracion del ADC (primero a los ADC comunes)
	RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN;																//habilito el reloj del ADC1
	ADC -> CCR &=~ ADC_CCR_ADCPRE;																		//prescaler desactivado
	ADC -> CCR |= ADC_CCR_TSVREFE;																		//habilito el sensor de temperatura interno

	//3. Configuracion del ADC1 (ahora a mi ADC1 en especifico)
	ADC1 -> CR1 = 0;																					//limpio el registro CR1 de mi ADC1
	ADC1 -> CR1 |= ADC_CR1_OVRIE;																		//habilito la IT para el overrun (sobreescritura de dato)
	ADC1 -> CR1 &=~ ADC_CR1_RES;																		//resolucion de 12 bits
	ADC1 -> CR1 |= ADC_CR1_EOCIE;																		//habilito la IT para el fin de conversion
	ADC1 -> CR2 = 0;																					//limpio el registro CR2 de mi ADC1

#if 1
	//4. Habilitar el disparo externo (DESACTIVAR PARA POLLING)
	ADC1 -> CR2 |= ADC_CR2_EXTEN_0;																		//activo el External Trigger por Rising
	ADC1 -> CR2 |= ADC_CR2_EXTSEL_3;																	//fuente de disparo: TIM3 TRGO (Update Event), TIM_CH es para un pin fisico
	ADC1 -> CR2 &=~ ADC_CR2_ALIGN;																		//alineacion por la derecha
	ADC1 -> CR2 &=~ ADC_CR2_EOCS;																		//no voy a trabajar con DMA
#endif

	//5. Configurar el tiempo de muestreo y la secuencia de conversion
	ADC1 -> SMPR1 |= ADC_SMPR1_SMP14_1 |  ADC_SMPR1_SMP14_0;											//tiempo de muestreo de 56 ciclos de reloj
	ADC1 -> SQR1 = 0;																					//no hay secuencia ya que solo haremos una conversion (aqui va el numero de canales)
	ADC1 -> SQR3 = 14;																					//indica el canal que se va a convertir

	//6. Habilitamos el ADC
	ADC1 -> CR2 |= ADC_CR2_ADON;																		//habilitamos el ADC
	ADC1 -> CR2 |= ADC_CR2_SWSTART;																		//no hace conversión ya que usaremos external trigger

#if 1
	//7. Configuramos la interrupcion del ADC
	NVIC_EnableIRQ(ADC_IRQn);

	//8. Configuramos el External Trigger TIM3 (DESACTIVAR PARA POLLING)
	RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN;																//habilitamos el reloj del timer
	TIM3 -> CR1 = 0;																					//limpiamos el registro CR1 del TIM3
	/*
	 * Calculamos ARR para una frecuencia de 50Hz
	 * ARR = 16E+6 / ((80) * (50)) - 1
	 * ARR = 399
	 * PSC = 79
	 */
	TIM3 -> ARR = 400 - 1;																				//establezco mi ARR para 50 Hz
	TIM3 -> PSC = 80 - 1;																				//establezco mi PSC para 50 Hz
	TIM3 -> CR2 = TIM_CR2_MMS_1;																		//usamos el Update Event
	TIM3 -> CR1 |= TIM_CR1_CEN;																			//iniciar el conteo del tim
#endif
}

//conversion por polling (desventaja: genera un bloqueo)
uint16_t ADC_getData(uint8_t ch){
	ADC1 -> SQR1 &=~ ADC_SQR1_L;																		//indicar que haremos una unica conversion
	ADC1 -> SQR3 = ch;																					//indica el canal que se va a convertir
	ADC1 -> CR2 |= ADC_CR2_SWSTART;																		//hago un disparo por software (polling)
	while(!(ADC1 -> SR & ADC_SR_EOC));																	//espero a que se termine la conversion
	return ADC1 -> DR;																					//retorno el valor convertido
}

uint32_t getTicks(){
	return uwTicks;
}

void USART2_putc(char ch){
	while(!(USART2 -> SR & USART_SR_TXE));
	USART2 -> DR = ch;
}

char USART2_getc(void){
	while(!(USART2 -> SR & USART_SR_RXNE));
	return USART2 -> DR;
}

uint8_t button_read(uint8_t current){
	static uint32_t ticks = 0;
	static uint8_t lastState = 1;	//1 pull up, 0 pull down
	//filtrado
	if((getTicks() - ticks) > 20){		//el 20 es opcional
		ticks = getTicks();
		if(lastState != current){
			lastState = current;
			if(current == 0){			//flanco ascendente
				printf("3\n");
				return 0;
			}
		}
	}
	return 1;
}

void moverServo(int val){

}
