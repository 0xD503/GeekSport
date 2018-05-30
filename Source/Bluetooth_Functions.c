/*
 * Bluetooth_Functions.c
 *
 * Created: 5/8/2018 9:23:59 AM
 *  Author: Developer
 */ 

#include "Bluetooth_Functions.h"

volatile bool Bluetooth_State = false;
volatile uint8_t Bluetooth_Mode = BLUETOOTH_VIEW_MESSAGE_MODE;

int8_t Bluetooth_TransmitBuffer[BLUETOOTH_BUFFER_SIZE] = {0};
int8_t Bluetooth_ReceiveBuffer[BLUETOOTH_BUFFER_SIZE] = {0};


void Bluetooth_On (void)
{
	USART_Init();
}

void Bluetooth_Off (void)
{
	UCSR0B &= ~(1 << RXCIE0 | 1 << RXEN0 | 1 << TXEN0);									//	Turn off transceiver, receiver; disable Rx Complete Interrupt (when there are unread data in the receive buffer detected)
}

void Bluetooth_SendCommand (int8_t command)
{
	USART_PutByte(command);
}

void Bluetooth_GetMessage (void)
{
	USART_ReceiveString(Bluetooth_ReceiveBuffer);
}

void Bluetooth_ReceiveCommand (void)
{
	int8_t command = USART_GetByte();
	switch (command)
	{
		case BLUETOOTH_CHECK_CONNECTION:
		{
			
			
			break;
		}
		
		default:	break;
	}
}

void BLuetooth_LCD_Nokia_Main (void)
{
	/*static */bool Bluetooth_LastState = false;
	
	switch(Bluetooth_Mode)
	{
		case BLUETOOTH_VIEW_MESSAGE_MODE:
		{
			//LCD_Nokia_ClearBuffer();
			LCD_Nokia_SetPixelPosition(0, 0);
			LCD_Nokia_Send6CharString(Bluetooth_ReceiveBuffer);
			LCD_Nokia_UpdateFullScreen();
			
			break;
		}
		
		case BLUETOOTH_ON_OFF_COMMAND_MODE:
		{
			LCD_Nokia_ClearBuffer();
			
			Buttons_SetPressed = false;
			while (!Buttons_SetPressed)
			{
				LCD_Nokia_SetPixelPosition(0, 4);
				LCD_Nokia_Send6CharString_P((int8_t *) bluetoothString);
				
				if (Bluetooth_State)
				{
					if (!Bluetooth_LastState)
					{
						Bluetooth_On();
						Bluetooth_LastState = true;
					}
					LCD_Nokia_Send6CharString_P((int8_t *) bluetoothOnString);
// 					LCD_Nokia_SetPixelPosition(45, 0);
// 					LCD_Nokia_Put6Symbol_P(GeekSportSymbols[1]);
				}
				else
				{
					if (Bluetooth_LastState)
					{
						Bluetooth_Off();
						Bluetooth_LastState = false;
					}
					LCD_Nokia_Send6CharString_P((int8_t *) bluetoothOffString);
				}
				LCD_Nokia_UpdateFullScreen();
			}
			
			Bluetooth_Mode = BLUETOOTH_VIEW_MESSAGE_MODE;
			LCD_Nokia_ClearBuffer();
			break;
		}
		
		default:	break;
	}
}