#include "LCD.h"
#include "main.h"
#include "stm32f4xx_hal.h"

void LCD_Command(uint8_t x){
	E_OFF;
	E_ON;

	//0001
	if(x & 0x01){D4_ON;} else{D4_OFF;}
	//0010
	if(x & 0x02){D5_ON;} else{D5_OFF;}
	//0100
	if(x & 0x04){D6_ON;} else{D6_OFF;}
	//1000
	if(x & 0x08){D7_ON;} else{D7_OFF;}

	HAL_Delay(2);
	E_OFF;
}

void LCD_Init(void){
	HAL_Delay(2);
	LCD_Command(0x03);
	HAL_Delay(4);
	LCD_Command(0x03);
	HAL_Delay(1);
	LCD_Command(0x03);
	HAL_Delay(1);
	LCD_Command(0x02);
	LCD_Command(0x02);
	LCD_Command(0x08); //16x2
	LCD_Command(0x00);
	LCD_Command(0x08);
	LCD_Command(0x00);
	LCD_Command(0x01);
	LCD_Command(0x00);
	LCD_Command(0x0D); //No Cursor: 0XC
}

void LCD_XY(uint8_t x, location y){
	RS_OFF;
	switch(y){
		case f1:
			LCD_Command(0x80 + x);
			LCD_Command((0x80 + x) >> 4);
			break;
		case f2:
			LCD_Command(0xC0 + x);
			LCD_Command((0xC0 + x) >> 4);
			break;
		case f3:
			LCD_Command(0x80 + 20 + x);
			LCD_Command((0x80 + 20 + x) >> 4);
			break;
		case f4:
			LCD_Command(0xC0 + 20 + x);
			LCD_Command((0xC0 + 20 + x) >> 4);
			break;
		default:
			break;
	}
}

void LCD_Print_Chart(char c){
	RS_ON;
	LCD_Command(c >> 4);
	LCD_Command(c);
}

void LCD_Print_String(char *s){
	while(*s){
		LCD_Print_Chart(*s++);
	}
}

void LCD_NewChar(uint8_t a,uint8_t b,uint8_t c,uint8_t d ,uint8_t e ,uint8_t f,uint8_t g,uint8_t h,uint8_t i){

  switch(a){
  case 1:
	  LCD_Command(0x04);
	  LCD_Command(0x00);
	  RS_ON;
	  LCD_Command(b>>4);
	  LCD_Command(b);
	  LCD_Command(c>>4);
	  LCD_Command(c);
	  LCD_Command(d>>4);
	  LCD_Command(d);
	  LCD_Command(e>>4);
	  LCD_Command(e);
	  LCD_Command(f>>4);
	  LCD_Command(f);
	  LCD_Command(g>>4);
	  LCD_Command(g);
	  LCD_Command(h>>4);
	  LCD_Command(h);
	  LCD_Command(i>>4);
	  LCD_Command(i);
	  RS_OFF;
	  LCD_Command(0x00);
	  LCD_Command(0x01);
	  HAL_Delay(100);
	  break;
  case 2:
	  LCD_Command(0x04);
	  LCD_Command(0x08);
	  RS_ON;
	  LCD_Command(b>>4);
	  LCD_Command(b);
	  LCD_Command(c>>4);
	  LCD_Command(c);
	  LCD_Command(c>>4);
	  LCD_Command(c);
	  LCD_Command(d>>4);
	  LCD_Command(d);
	  LCD_Command(e>>4);
	  LCD_Command(e);
	  LCD_Command(f>>4);
	  LCD_Command(f);
	  LCD_Command(g>>4);
	  LCD_Command(g);
	  LCD_Command(h>>4);
	  LCD_Command(h);
	  LCD_Command(i>>4);
	  LCD_Command(i);
	  RS_OFF;
	  LCD_Command(0x00);
	  LCD_Command(0x01);
	  HAL_Delay(100);
	  break;
  case 3:
	  LCD_Command(0x04);
	  LCD_Command(0xFF);
	  RS_ON;
	  LCD_Command(b>>4);
	  LCD_Command(b);
	  LCD_Command(c>>4);
	  LCD_Command(c);
	  LCD_Command(d>>4);
	  LCD_Command(d);
	  LCD_Command(e>>4);
	  LCD_Command(e);
	  LCD_Command(f>>4);
	  LCD_Command(f);
	  LCD_Command(g>>4);
	  LCD_Command(g);
	  LCD_Command(h>>4);
	  LCD_Command(h);
	  LCD_Command(i>>4);
	  LCD_Command(i);
	  RS_OFF;
	  LCD_Command(0x00);
	  LCD_Command(0x01);
	  HAL_Delay(100);
	  break;
  }
}

void LCD_Print_NewChar(uint8_t a){
	RS_ON;
	LCD_Command(0x00);
	LCD_Command(a-1);
	RS_OFF;
}

void LCD_Clear(void){
	RS_OFF;
	LCD_Command(0x00);
	LCD_Command(0x01);
}
