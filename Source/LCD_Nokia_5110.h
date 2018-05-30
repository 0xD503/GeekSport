/*
 * LCD_Nokia_5110.h
 *
 * Created: 3/4/2018 1:33:33 PM
 *  Author: Developer
 */ 


#ifndef LCD_NOKIA_5110_H_
#define LCD_NOKIA_5110_H_

#include "main.h"

#define LCD_NOKIA_DDR	SPI_DDR
#define LCD_NOKIA_PORT	SPI_PORT
#define LCD_NOKIA_PINS	SPI_PINS

#define LCD_NOKIA_LED_DDR DDRD
#define LCD_NOKIA_LED_PORT PORTD
#define LCD_NOKIA_LED_PINS PIND


#define LCD_NOKIA_CHIP_SELECT_PIN SPI_SS_1
#define LCD_NOKIA_RESET_PIN 1
#define LCD_NOKIA_DATA_NOT_COMMAND_PIN 0
#define LCD_NOKIA_SERIAL_DATA_IN_PIN SPI_MOSI
#define LCD_NOKIA_SERIAL_CLOCK_PIN SPI_SCK

#define LCD_NOKIA_LED_PIN 6

#define LCD_NOKIA_RESET LCD_NOKIA_PORT &= ~(1 << LCD_NOKIA_RESET_PIN)
#define LCD_NOKIA_UNRESET LCD_NOKIA_PORT |= 1 << LCD_NOKIA_RESET_PIN
#define LCD_NOKIA_SELECT SPI_SELECT_SLAVE_1
#define LCD_NOKIA_UNSELECT SPI_UNSELECT_SLAVE_1

#define LCD_NOKIA_SEND_COMMAND LCD_NOKIA_PORT &= ~(1 << LCD_NOKIA_DATA_NOT_COMMAND_PIN)
#define LCD_NOKIA_SEND_DATA LCD_NOKIA_PORT |= 1 << LCD_NOKIA_DATA_NOT_COMMAND_PIN


#define LCD_NOKIA_POWER_ON 0x20
#define LCD_NOKIA_POWER_DOWN 0x24
#define LCD_NOKIA_HORIZONTAL_ENTRY 0x20
#define LCD_NOKIA_VERTICAL_ENTRY 0x22

#define LCD_NOKIA_BASIC_INSTRUCTION_SET 0x20
#define LCD_NOKIA_DISPLAY_CONTROL 0x08
#define LCD_NOKIA_SET_X_ADDRESS 0x80
#define LCD_NOKIA_SET_Y_ADDRESS 0x40


#define LCD_NOKIA_EXTENDED_INSTRUCTION_SET 0x21
#define LCD_NOKIA_SET_OPERATION_VOLTAGE 0x80
#define LCD_NOKIA_SET_BIAS 0x10
#define LCD_NOKIA_SET_TEMPERATURE_COEFFICIENT 0x04

#define LCD_NOKIA_X_RESOLUTION 84
#define LCD_NOKIA_Y_RESOLUTION 48
#define LCD_NOKIA_BUFFER_SIZE ((LCD_NOKIA_X_RESOLUTION * LCD_NOKIA_Y_RESOLUTION) / 8)

#define LCD_NOKIA_LED_DELAY_SECONDS 5
#define LCD_NOKIA_LED_TIMER_PRESCALER 256

#define ASCII_NULL 0x30

struct LCD_Nokia_Display
{
	uint8_t x, y;
	uint8_t screen[];
};

struct LCD_Nokia_Point
{
	uint8_t x, y;
};

struct LCD_Nokia_Line
{
	struct LCD_Nokia_Point point1, point2;
};

struct LCD_Nokia_Display nokiaDisplayPixels;

void LCD_Nokia_PutCommandByte(int8_t byte);
void LCD_Nokia_PutDataByte(int8_t byte);
void LCD_Nokia_SendDataString (int8_t* str);
void LCD_Nokia_SendCommandString (int8_t* str);

void LCD_Nokia_Init (void);

void LCD_Nokia_SetPosition (uint8_t x, uint8_t y);
void LCD_Nokia_SetPixelPosition (uint8_t x, uint8_t y);

void LCD_Nokia_PutPixel (uint8_t x, uint8_t y, uint8_t black);

//void LCD_Nokia_UpdateScreen (uint8_t xLeft, uint8_t yHigh, uint8_t xRight, uint8_t yLow);
void LCD_Nokia_UpdateFullScreen (void);

void LCD_Nokia_SendImage (uint8_t* image);

void LCD_Nokia_Put6Symbol (uint8_t symbol);
void LCD_Nokia_Put6Symbol_P (uint8_t symbol);

void LCD_Nokia_Put5Char (int8_t chr);
void LCD_Nokia_Send5CharString (int8_t* str);
void LCD_Nokia_Send5CharString_P (int8_t* str_InProgramMemory);
void LCD_Nokia_Send5Integer (int32_t integer);

void LCD_Nokia_Put6Char(int8_t chr);
void LCD_Nokia_Send6CharString (int8_t* str);
void LCD_Nokia_Send6CharString_P (int8_t* str_InProgramMemory);
void LCD_Nokia_Send6Integer(int32_t integer);

void LCD_Nokia_Put12Char (int8_t chr);
void LCD_Nokia_Send12CharString (int8_t* str);
void LCD_Nokia_Send12CharString_P (int8_t* str_InProgramMemory);
void LCD_Nokia_Send12Integer(int32_t integer);


void LCD_Nokia_DrawUnfilledRectangle (uint8_t xLeft, uint8_t yHigh, uint8_t xRight, uint8_t yLow, uint8_t width);
void LCD_Nokia_DrawFilledRectangle (uint8_t xLeft, uint8_t yHigh, uint8_t xRight, uint8_t yLow);

void LCD_Nokia_ClearDisplay (void);
void LCD_Nokia_ClearBuffer (void);

void LCD_Nokia_SetPower (uint8_t on);
void LCD_Nokia_SetUpContrast (uint8_t contrast);
void LCD_Nokia_TurnOnLED (void);


#endif /* LCD_NOKIA_5110_H_ */