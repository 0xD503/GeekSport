/*
 * LCD_Nokia_5110.c
 *
 * Created: 3/4/2018 1:33:14 PM
 *  Author: Developer
 */ 

#include "LCD_Nokia_5110.h"

struct LCD_Nokia_Display nokiaDisplayPixels = {0};

void LCD_Nokia_Reset (void)
{
	LCD_NOKIA_RESET;
	_delay_ms(10);		// ?????????????????????????? <<<<<<<<<<<<<<<<<<<<<<<<<
	LCD_NOKIA_UNRESET;
}

void LCD_Nokia_PutByte (int8_t byte, uint8_t mode)
{
	if (mode)	LCD_NOKIA_SEND_DATA;
	else	LCD_NOKIA_SEND_COMMAND;
	
	LCD_NOKIA_SELECT;
	SPI_PutByte(byte);
	LCD_NOKIA_UNSELECT;
}

void LCD_Nokia_PutCommandByte(int8_t byte)
{
	LCD_Nokia_PutByte(byte, 0);
}

void LCD_Nokia_PutDataByte(int8_t byte)
{
	LCD_Nokia_PutByte(byte, 1);
}

void LCD_Nokia_SendDataString (int8_t* str)
{
	while(*str)	LCD_Nokia_PutDataByte(*(str++));
}

void LCD_Nokia_SendCommandString (int8_t* str)
{
	while(*str)	LCD_Nokia_PutCommandByte(*(str++));
}

void LCD_Nokia_ClearDisplay (void)
{
	for (uint16_t i = 0; i < LCD_NOKIA_BUFFER_SIZE; i++)	LCD_Nokia_PutDataByte(0x00);
}

void LCD_Nokia_ClearBuffer (void)
{
	//memset(nokiaDisplayPixels.screen, 0, LCD_NOKIA_BUFFER_SIZE);
	for (uint16_t i = 0; i < LCD_NOKIA_BUFFER_SIZE; i++)	nokiaDisplayPixels.screen[i] = 0;
}

void LCD_Nokia_Init (void)
{
	LCD_NOKIA_DDR |= 1 << LCD_NOKIA_RESET_PIN | 1 << LCD_NOKIA_DATA_NOT_COMMAND_PIN;
	LCD_NOKIA_LED_DDR |= 1 << LCD_NOKIA_LED_PIN;
	LCD_NOKIA_UNRESET;
	LCD_NOKIA_UNSELECT;
	LCD_Nokia_Reset();
	LCD_Nokia_PutCommandByte(LCD_NOKIA_EXTENDED_INSTRUCTION_SET);
	LCD_Nokia_PutCommandByte(LCD_NOKIA_SET_OPERATION_VOLTAGE | 0x10);
	LCD_Nokia_PutCommandByte(LCD_NOKIA_SET_TEMPERATURE_COEFFICIENT | 0x03);
	LCD_Nokia_PutCommandByte(LCD_NOKIA_SET_BIAS | 0x11);
	LCD_Nokia_PutCommandByte(LCD_NOKIA_BASIC_INSTRUCTION_SET);
	LCD_Nokia_PutCommandByte(LCD_NOKIA_DISPLAY_CONTROL | 0x04);
	LCD_Nokia_ClearDisplay();
}

void LCD_Nokia_SetPosition (uint8_t x, uint8_t y)
{
	LCD_NOKIA_SELECT;
	LCD_Nokia_PutCommandByte(LCD_NOKIA_SET_X_ADDRESS | (x));
	LCD_Nokia_PutCommandByte(LCD_NOKIA_SET_Y_ADDRESS | (y));
	LCD_NOKIA_UNSELECT;
}

void LCD_Nokia_SetPixelPosition (uint8_t x, uint8_t y)
{
	nokiaDisplayPixels.x = x;
	nokiaDisplayPixels.y = y;
}

void LCD_Nokia_PutPixel (uint8_t x, uint8_t y, uint8_t black)
{
	if (black)	nokiaDisplayPixels.screen[x + ((y / 8) * 84)] |= 1 << (y % 8);
	else	nokiaDisplayPixels.screen[x + ((y / 8) * 84)] &= ~(1 << (y % 8));
}

void LCD_Nokia_SetPixel (uint8_t x, uint8_t y)
{
	LCD_Nokia_PutPixel(x, y, 1);
}

void LCD_Nokia_ClearPixel (uint8_t x, uint8_t y)
{
	LCD_Nokia_PutPixel(x, y, 0);
}

void LCD_Nokia_PutSmallChar (int8_t chr, uint8_t width)
{
	static uint8_t byte;
	switch (width)
	{
		case 5:
		{
			for (uint8_t i = 0; i < 5; i++)
			{
				byte = pgm_read_byte(&(LCD_Nokia_ASCII_Charset5[chr - 0x20][i]));
				for (uint8_t j = 0; j < 8; j++)
				{
					if (byte & (1 << j))	LCD_Nokia_PutPixel(nokiaDisplayPixels.x, nokiaDisplayPixels.y++, 1);
					else	LCD_Nokia_PutPixel(nokiaDisplayPixels.x, nokiaDisplayPixels.y++, 0);
				}
				nokiaDisplayPixels.x++;
				nokiaDisplayPixels.y -= 8;
			}
			
			break;
		}
		
		case 6:
		{
			for (uint8_t i = 0; i < 6; i++)
			{
				byte = pgm_read_byte(&(LCD_Nokia_SmallFont6x8[chr - 0x20][i]));
				for (uint8_t j = 0; j < 8; j++)
				{
					if (byte & (1 << j))	LCD_Nokia_PutPixel(nokiaDisplayPixels.x, nokiaDisplayPixels.y++, 1);
					else	LCD_Nokia_PutPixel(nokiaDisplayPixels.x, nokiaDisplayPixels.y++, 0);
				}
				nokiaDisplayPixels.x++;
				nokiaDisplayPixels.y -= 8;
			}
			
			break;
		}
	}
	
	if (nokiaDisplayPixels.x >= 80)
	{
		nokiaDisplayPixels.x = 0;
		nokiaDisplayPixels.y += 8;
		if (nokiaDisplayPixels.y >= 48)	nokiaDisplayPixels.y = 0;
	}
}

void LCD_Nokia_PutSymbol (uint8_t symbol, uint8_t width)
{
	static uint8_t byte;
	switch (width)
	{
		case 5:
		{
			for (uint8_t i = 0; i < 5; i++)
			{
				byte = pgm_read_byte(&(LCD_Nokia_ASCII_Charset5[symbol][i]));
				for (uint8_t j = 0; j < 8; j++)
				{
					if (byte & (1 << j))	LCD_Nokia_PutPixel(nokiaDisplayPixels.x, nokiaDisplayPixels.y++, 1);
					else	LCD_Nokia_PutPixel(nokiaDisplayPixels.x, nokiaDisplayPixels.y++, 0);
				}
				nokiaDisplayPixels.x++;
				nokiaDisplayPixels.y -= 8;
			}
			
			break;
		}
		
		case 6:
		{
			for (uint8_t i = 0; i < 6; i++)
			{
				byte = pgm_read_byte(&(LCD_Nokia_SmallFont6x8[symbol][i]));
				for (uint8_t j = 0; j < 8; j++)
				{
					if (byte & (1 << j))	LCD_Nokia_PutPixel(nokiaDisplayPixels.x, nokiaDisplayPixels.y++, 1);
					else	LCD_Nokia_PutPixel(nokiaDisplayPixels.x, nokiaDisplayPixels.y++, 0);
				}
				nokiaDisplayPixels.x++;
				nokiaDisplayPixels.y -= 8;
			}
			
			break;
		}
	}
	
	if (nokiaDisplayPixels.x >= 80)
	{
		nokiaDisplayPixels.x = 0;
		nokiaDisplayPixels.y += 8;
		if (nokiaDisplayPixels.y >= 48)	nokiaDisplayPixels.y = 0;
	}
}

void LCD_Nokia_Put5Char(int8_t chr)
{
	LCD_Nokia_PutSmallChar(chr, 5);
}

void LCD_Nokia_Put6Char(int8_t chr)
{
	LCD_Nokia_PutSmallChar(chr, 6);
}

void LCD_Nokia_Put6Symbol (uint8_t symbol)
{
	LCD_Nokia_PutSymbol(symbol, 6);
}

void LCD_Nokia_Put6Symbol_P (uint8_t symbol)
{
	LCD_Nokia_Put6Symbol(pgm_read_byte(symbol));
}

void LCD_Nokia_Send5CharString (int8_t* str)
{
	while (*str)	LCD_Nokia_Put5Char(*(str++));
}

void LCD_Nokia_Send5CharString_P (int8_t* str_InProgramMemory)
{
	while (pgm_read_byte(str_InProgramMemory))	LCD_Nokia_Put5Char(pgm_read_byte(str_InProgramMemory++));
}

void LCD_Nokia_Send6CharString (int8_t* str)
{
	while (*str)	LCD_Nokia_Put6Char(*(str++));
}

void LCD_Nokia_Send6CharString_P (int8_t* str_InProgramMemory)
{
	while (pgm_read_byte(str_InProgramMemory))	LCD_Nokia_Put6Char(pgm_read_byte(str_InProgramMemory++));
}


void LCD_Nokia_Put12Char (int8_t chr)
{
	static uint8_t byte;
	for (uint8_t k = 0; k < 2; k++)
	{
		for (uint8_t i = 0; i < 12; i++)
		{
			byte = pgm_read_byte(&(LCD_Nokia_MediumFont12x16[chr - 0x2D][k][i]));
			for (uint8_t j = 0; j < 8; j++)
			{
				if (byte & (1 << j))	LCD_Nokia_PutPixel(nokiaDisplayPixels.x, nokiaDisplayPixels.y++, 1);
				else	LCD_Nokia_PutPixel(nokiaDisplayPixels.x, nokiaDisplayPixels.y++, 0);
			}
			nokiaDisplayPixels.x++;
			nokiaDisplayPixels.y -= 8;
		}
		nokiaDisplayPixels.x -= 12;
		nokiaDisplayPixels.y += 8;
	}
	nokiaDisplayPixels.x += 12;
	nokiaDisplayPixels.y -= 16;
	
	if (nokiaDisplayPixels.x >= 83)
	{
		nokiaDisplayPixels.x = 0;
		nokiaDisplayPixels.y += 16;
		if (nokiaDisplayPixels.y >= 47)	nokiaDisplayPixels.y = 0;
	}
}

void LCD_Nokia_Send12CharString (int8_t* str)
{
	while (*str)	LCD_Nokia_Put12Char(*(str++));
}

void LCD_Nokia_Send12CharString_P (int8_t* str_InProgramMemory)
{
	while (pgm_read_byte(str_InProgramMemory))	LCD_Nokia_Put12Char(pgm_read_byte(str_InProgramMemory++));
}

void LCD_Nokia_SendInteger (int32_t integer, uint8_t fontWidth)
{
	uint8_t numberOfSymbols = 0;
	
	switch (fontWidth)
	{
		case 5:
		{
			if (integer < 0)
			{
				LCD_Nokia_Put5Char('-');
				integer = abs(integer);
			}
			
			if (integer < 10)	numberOfSymbols = 1;
			if (((integer >= 10) & (integer < 100)))	numberOfSymbols = 2;
			if (((integer >= 100) & (integer < 1000)))	numberOfSymbols = 3;
			if (((integer >= 1000) & (integer < 10000)))	numberOfSymbols = 4;
			if (((integer >= 10000) & (integer < 100000)))	numberOfSymbols = 5;
			//	if (((integer >= 100000) & (integer < 1000000)))	numberOfSymbols = 6;
			
			switch (numberOfSymbols)
			{
				case  1:
				{
					LCD_Nokia_Put5Char(integer + ASCII_NULL);
					
					break;
				}
				
				case  2:
				{
					LCD_Nokia_Put5Char((integer / 10) + ASCII_NULL);
					LCD_Nokia_Put5Char((integer % 10) + ASCII_NULL);
					
					break;
				}
				
				case  3:
				{
					LCD_Nokia_Put5Char((integer / 100) + ASCII_NULL);
					LCD_Nokia_Put5Char(((integer % 100) / 10) + ASCII_NULL);
					LCD_Nokia_Put5Char(((integer % 100) % 10) + ASCII_NULL);
					
					break;
				}

				case  4:
				{
					LCD_Nokia_Put5Char((integer / 1000) + ASCII_NULL);
					LCD_Nokia_Put5Char(((integer % 1000) / 100) + ASCII_NULL);
					LCD_Nokia_Put5Char((((integer % 1000) % 100) / 10) + ASCII_NULL);
					LCD_Nokia_Put5Char((((integer % 1000) % 100) % 10) + ASCII_NULL);
					
					break;
				}

				case  5:
				{
					LCD_Nokia_Put5Char((integer / 10000) + ASCII_NULL);
					LCD_Nokia_Put5Char(((integer % 10000) / 1000) + ASCII_NULL);
					LCD_Nokia_Put5Char((((integer % 10000) % 1000) / 100) + ASCII_NULL);
					LCD_Nokia_Put5Char(((((integer % 10000) % 1000) % 100) / 10) + ASCII_NULL);
					LCD_Nokia_Put5Char(((((integer % 10000) % 1000) % 100) % 10) + ASCII_NULL);
					
					break;
				}
				
				default:
				{
					LCD_Nokia_Send5CharString((int8_t* ) "###");
					
					break;
				}
			}
			
			break;
		}
		
		case 6:
		{
			if (integer < 0)
			{
				LCD_Nokia_Put6Char('-');
				integer = abs(integer);
			}
			
			if (integer < 10)	numberOfSymbols = 1;
			if (((integer >= 10) & (integer < 100)))	numberOfSymbols = 2;
			if (((integer >= 100) & (integer < 1000)))	numberOfSymbols = 3;
			if (((integer >= 1000) & (integer < 10000)))	numberOfSymbols = 4;
			if (((integer >= 10000) & (integer < 100000)))	numberOfSymbols = 5;
			//	if (((integer >= 100000) & (integer < 1000000)))	numberOfSymbols = 6;
			
			switch (numberOfSymbols)
			{
				case  1:
				{
					LCD_Nokia_Put6Char(integer + ASCII_NULL);
					
					break;
				}
				
				case  2:
				{
					LCD_Nokia_Put6Char((integer / 10) + ASCII_NULL);
					LCD_Nokia_Put6Char((integer % 10) + ASCII_NULL);
					
					break;
				}
				
				case  3:
				{
					LCD_Nokia_Put6Char((integer / 100) + ASCII_NULL);
					LCD_Nokia_Put6Char(((integer % 100) / 10) + ASCII_NULL);
					LCD_Nokia_Put6Char(((integer % 100) % 10) + ASCII_NULL);
					
					break;
				}

				case  4:
				{
					LCD_Nokia_Put6Char((integer / 1000) + ASCII_NULL);
					LCD_Nokia_Put6Char(((integer % 1000) / 100) + ASCII_NULL);
					LCD_Nokia_Put6Char((((integer % 1000) % 100) / 10) + ASCII_NULL);
					LCD_Nokia_Put6Char((((integer % 1000) % 100) % 10) + ASCII_NULL);
					
					break;
				}

				case  5:
				{
					LCD_Nokia_Put6Char((integer / 10000) + ASCII_NULL);
					LCD_Nokia_Put6Char(((integer % 10000) / 1000) + ASCII_NULL);
					LCD_Nokia_Put6Char((((integer % 10000) % 1000) / 100) + ASCII_NULL);
					LCD_Nokia_Put6Char(((((integer % 10000) % 1000) % 100) / 10) + ASCII_NULL);
					LCD_Nokia_Put6Char(((((integer % 10000) % 1000) % 100) % 10) + ASCII_NULL);
					
					break;
				}
				
				default:
				{
					LCD_Nokia_Send6CharString((int8_t* ) "###");
					
					break;
				}
			}
			
			break;
		}
		
		case 12:
		{
			if (integer < 0)
			{
				LCD_Nokia_Put12Char('-');
				integer = abs(integer);
			}
			
			if (integer < 10)	numberOfSymbols = 1;
			if (((integer >= 10) & (integer < 100)))	numberOfSymbols = 2;
			if (((integer >= 100) & (integer < 1000)))	numberOfSymbols = 3;
			if (((integer >= 1000) & (integer < 10000)))	numberOfSymbols = 4;
			if (((integer >= 10000) & (integer < 100000)))	numberOfSymbols = 5;
			if (((integer >= 100000) & (integer < 1000000)))	numberOfSymbols = 6;
			
			switch (numberOfSymbols)
			{
				case  1:
				{
					LCD_Nokia_Put12Char(integer + ASCII_NULL);
					
					break;
				}
				
				case  2:
				{
					LCD_Nokia_Put12Char((integer / 10) + ASCII_NULL);
					LCD_Nokia_Put12Char((integer % 10) + ASCII_NULL);
					
					break;
				}
				
				case  3:
				{
					LCD_Nokia_Put12Char((integer / 100) + ASCII_NULL);
					LCD_Nokia_Put12Char(((integer % 100) / 10) + ASCII_NULL);
					LCD_Nokia_Put12Char(((integer % 100) % 10) + ASCII_NULL);
					
					break;
				}

				case  4:
				{
					LCD_Nokia_Put12Char((integer / 1000) + ASCII_NULL);
					LCD_Nokia_Put12Char(((integer % 1000) / 100) + ASCII_NULL);
					LCD_Nokia_Put12Char((((integer % 1000) % 100) / 10) + ASCII_NULL);
					LCD_Nokia_Put12Char((((integer % 1000) % 100) % 10) + ASCII_NULL);
					
					break;
				}

				case  5:
				{
					LCD_Nokia_Put12Char((integer / 10000) + ASCII_NULL);
					LCD_Nokia_Put12Char(((integer % 10000) / 1000) + ASCII_NULL);
					LCD_Nokia_Put12Char((((integer % 10000) % 1000) / 100) + ASCII_NULL);
					LCD_Nokia_Put12Char(((((integer % 10000) % 1000) % 100) / 10) + ASCII_NULL);
					LCD_Nokia_Put12Char(((((integer % 10000) % 1000) % 100) % 10) + ASCII_NULL);
					
					break;
				}

				case  6:
				{
					LCD_Nokia_Put12Char((integer / 100000) + ASCII_NULL);
					LCD_Nokia_Put12Char(((integer % 100000) / 10000) + ASCII_NULL);
					LCD_Nokia_Put12Char((((integer % 100000) % 10000) / 1000) + ASCII_NULL);
					LCD_Nokia_Put12Char(((((integer % 100000) % 10000) % 1000) / 100) + ASCII_NULL);
					LCD_Nokia_Put12Char((((((integer % 100000) % 10000) % 1000) % 100) / 10) + ASCII_NULL);
					LCD_Nokia_Put12Char((((((integer % 100000) % 10000) % 1000) % 100) % 10) + ASCII_NULL);
					
					break;
				}
				
				default:
				{
					LCD_Nokia_Send12CharString((int8_t* ) "###");
					
					break;
				}
			}
			
			break;
		}
	}
	
}

void LCD_Nokia_Send5Integer(int32_t integer)
{
	LCD_Nokia_SendInteger(integer, 5);
}

void LCD_Nokia_Send12Integer(int32_t integer)
{
	LCD_Nokia_SendInteger(integer, 12);
}

void LCD_Nokia_Send6Integer(int32_t integer)
{
	LCD_Nokia_SendInteger(integer, 6);
}

void LCD_Nokia_DrawLine (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)	//struct LCD_Nokia_Line line)
{
// 	int8_t stepX, stepY;
// 	int8_t dx = x2 - x1;
// 	int8_t dy = y2 - y1;
// 	stepX = ((dx < 0) ? -1 : 1);
// 	stepY = ((dy < 0) ? -1 : 1);
}

void LCD_Nokia_DrawRectangle (uint8_t xLeft, uint8_t yHigh, uint8_t xRight, uint8_t yLow, uint8_t filling)
{
	uint8_t temp, i;
	if (xLeft <= xRight);
	else
	{
		temp = xRight;
		xRight = xLeft;
		xLeft = temp;
	}
	if (yHigh <= yLow);
	else
	{
		temp = yLow;
		yLow = yHigh;
		yHigh = temp;
	}
	
	if (filling)
	{
		for (i = yHigh; i <= yLow; i++)
		{
			for (uint8_t j = xLeft; j <= xRight; j++)	LCD_Nokia_SetPixel(j, i);
		}
	}
	else
	{
		for (i = xLeft; i <= xRight; i++)	LCD_Nokia_SetPixel(i, yHigh);
		for (i = xLeft; i <= xRight; i++)	LCD_Nokia_SetPixel(i, yLow);
		for (i = yHigh; i <= yLow; i++)	LCD_Nokia_SetPixel(xLeft, i);
		for (i = yHigh; i <= yLow; i++)	LCD_Nokia_SetPixel(xRight, i);
	}
}


void LCD_Nokia_DrawUnfilledRectangle (uint8_t xLeft, uint8_t yHigh, uint8_t xRight, uint8_t yLow, uint8_t width)
{
	for (uint8_t i = 0; i < width; i++)	LCD_Nokia_DrawRectangle(xLeft++, yHigh++, xRight--, yLow--, 0);
}

void LCD_Nokia_DrawFilledRectangle (uint8_t xLeft, uint8_t yHigh, uint8_t xRight, uint8_t yLow)
{
	LCD_Nokia_DrawRectangle(xLeft, yHigh, xRight, yLow, 1);
}

void LCD_Nokia_DrawUnfilledCircle (uint8_t xO, uint8_t yO, uint8_t radius)
{
	
}

// void LCD_Nokia_UpdateScreen (uint8_t xLeft, uint8_t yHigh, uint8_t xRight, uint8_t yLow)
// {
// 	for (uint8_t i = yHigh; i <= yLow; i++)
// 	{
// 		for (uint8_t j = xLeft; j <= xRight; j++)
// 		{
// 			LCD_Nokia_SetPosition(j, i);
// 			LCD_Nokia_PutDataByte(nokiaDisplayPixels.screen[j + (i * 84)]);
// 		}
// 	}
// }

void LCD_Nokia_UpdateFullScreen (void)
{
	LCD_Nokia_SetPosition(0, 0);
	for (uint16_t i = 0; i < LCD_NOKIA_BUFFER_SIZE; i++)	LCD_Nokia_PutDataByte(nokiaDisplayPixels.screen[i]);
}


void LCD_Nokia_SendImage (uint8_t* image)
{
	static uint8_t byte;
	LCD_Nokia_SetPixelPosition(0, 0);
	for (uint16_t i = 0; i < 504; i++)
	{
		byte = pgm_read_byte(&(image[i]));
		for (uint8_t j = 0; j < 8; j++)
		{
			if (byte & (1 << j))	LCD_Nokia_PutPixel(nokiaDisplayPixels.x, nokiaDisplayPixels.y++, 1);
			else	LCD_Nokia_PutPixel(nokiaDisplayPixels.x, nokiaDisplayPixels.y++, 0);
		}
		nokiaDisplayPixels.x++;
		nokiaDisplayPixels.y -= 8;
		if (nokiaDisplayPixels.x >= 84)
		{
			nokiaDisplayPixels.x = 0;
			nokiaDisplayPixels.y += 8;
			if (nokiaDisplayPixels.y >= 48)	nokiaDisplayPixels.y = 0;	//	????????
		}
	}
}


void LCD_Nokia_SetPower (uint8_t on)
{
	LCD_Nokia_PutCommandByte(on ?	LCD_NOKIA_POWER_ON : LCD_NOKIA_POWER_DOWN);
}

void LCD_Nokia_SetEntryMode (uint8_t horizontal)
{
	LCD_Nokia_PutCommandByte(horizontal ?	LCD_NOKIA_HORIZONTAL_ENTRY : LCD_NOKIA_VERTICAL_ENTRY);
}

void LCD_Nokia_SetDiplayMode (uint8_t mode)
{
	LCD_Nokia_PutCommandByte(LCD_NOKIA_BASIC_INSTRUCTION_SET);
	LCD_Nokia_PutCommandByte(LCD_NOKIA_DISPLAY_CONTROL | mode);
}

void LCD_Nokia_SetUpContrast (uint8_t contrast)
{
	LCD_Nokia_PutCommandByte(LCD_NOKIA_EXTENDED_INSTRUCTION_SET);
	LCD_Nokia_PutCommandByte(LCD_NOKIA_SET_OPERATION_VOLTAGE | contrast);
}

void LCD_Nokia_SetUpTemperature (uint8_t temperature)
{
	LCD_Nokia_PutCommandByte(LCD_NOKIA_EXTENDED_INSTRUCTION_SET);
	LCD_Nokia_PutCommandByte(LCD_NOKIA_SET_TEMPERATURE_COEFFICIENT | temperature);
}

void LCD_Nokia_TurnOnLED (void)
{
	LCD_NOKIA_PORT |= 1 << LCD_NOKIA_LED_PIN;
	//	timer CTC init
}