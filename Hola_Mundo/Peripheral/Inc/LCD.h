#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "main.h"

#define LCD_PinTypeDef uint16_t
#define LCD_PortTypeDef GPIO_TypeDef*

typedef struct{
	LCD_PortTypeDef *dataport;
	LCD_PinTypeDef *datapin;
	LCD_PortTypeDef RS_port;
	LCD_PinTypeDef RS_pin;
	LCD_PortTypeDef E_port;
	LCD_PinTypeDef E_pin;
}LCD_Struct_t;

LCD_Struct_t LCD_Create(
		LCD_PortTypeDef port[], LCD_PinTypeDef pin[],
		LCD_PortTypeDef rs_port, LCD_PinTypeDef rs_pin,
		LCD_PortTypeDef e_port, LCD_PinTypeDef e_pin
);

void LCD_Init(LCD_Struct_t *LCD);
void LCD_Comando(LCD_Struct_t *LCD, char cmd);
void LCD_Caracter(LCD_Struct_t *LCD, char data);
void LCD_Cadena(LCD_Struct_t *LCD, char *cadena);
void LCD_XY(LCD_Struct_t *LCD, char x, char y);

#endif
