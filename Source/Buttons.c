/*
 * Buttons.c
 *
 * Created: 4/9/2018 3:27:15 PM
 *  Author: Developer
 */ 

#include "Buttons.h"

volatile static bool Buttons_Button1WasPushed = 0;
volatile static bool Buttons_Button2WasPushed = 0;
volatile static bool Buttons_Button3WasPushed = 0;
volatile static bool Buttons_Button4WasPushed = 0;

volatile bool Buttons_SetButtonState = true;

bool bitIsSet (uint8_t pins, uint8_t pin)
{
	return ((pins & (1 << pin)));
}

void Buttons_IO_Init (void)
{
	BUTTONS_DDR_1 &= ~(1 << BUTTONS_DDR_1_BUTTON_1_PIN | 1 << BUTTONS_DDR_1_BUTTON_2_PIN);
	BUTTONS_DDR_2 &= ~(1 << BUTTONS_DDR_2_BUTTON_1_PIN | 1 << BUTTONS_DDR_2_BUTTON_2_PIN);
	BUTTONS_PORT_1 |= 1 << BUTTONS_DDR_1_BUTTON_1_PIN | 1 << BUTTONS_DDR_1_BUTTON_2_PIN;
	BUTTONS_PORT_2 |= 1 << BUTTONS_DDR_2_BUTTON_1_PIN | 1 << BUTTONS_DDR_2_BUTTON_2_PIN;
}

void Buttons_Init (void)
{
	Buttons_IO_Init();
	TCCR0A |= 1 << WGM01;																		//	Set CTC mode for button's state polling and set LCD LED pin
	TCCR0B |= 1 << CS02 | 0 << CS01 | 0 << CS00;												//	Set Timer 0 Prescaler = F_CPU / 256
	TIMSK0 |= 1 << OCIE0A;																		//	Enable Interrupts on compare
	OCR0A = (BUTTONS_DELAY_US / BUTTONS_PRESCALER);												//	Set OCR value()
}


ISR (TIMER0_COMPA_vect)													//	Button's state polling
{
	uint8_t mode = GeekSport_Mode;
	
	if (!(bitIsSet(BUTTONS_PINS_1, BUTTONS_DDR_1_BUTTON_1_PIN)))							//	Mode button
	{
		if (!Buttons_Button1WasPushed)
		{
			Buttons_Button1WasPushed = true;
			
			if ((mode < (GEEKSPORT_LAST_MODE - 1)) && mode != GEEKSPORT_START)	GeekSport_Mode++;
			else if (mode == GEEKSPORT_START);
			else	GeekSport_Mode = GEEKSPORT_WATCH_MODE;
		}
	}
	else	Buttons_Button1WasPushed = false;

//	###########################################################################################################################################

	if (!(bitIsSet(BUTTONS_PINS_1, BUTTONS_DDR_1_BUTTON_2_PIN)))							//	Select button
	{
		if (!Buttons_Button2WasPushed)
		{
			Buttons_Button2WasPushed = true;
			
			if (Buttons_SetButtonState)
			{
				switch (mode)
				{
					case GEEKSPORT_START:	break;
					
					case GEEKSPORT_WATCH_MODE:
					{
						if (DS1307_Mode < DS1307_TIME_SETTING_MODE)	DS1307_Mode++;
						else	DS1307_Mode = DS1307_WATCH_MODE;
						
						break;
					}
					
					case GEEKSPORT_SPEEDOMETER_MODE:
					{
						if (Speedometer_Mode < (SPEEDOMETER_LAST_MODE - 1))	Speedometer_Mode++;
						else Speedometer_Mode = SPEEDOMETER_SPEED_MEASUREMENT_MODE;

						break;
					}
					
					case GEEKSPORT_BLUETOOTH_SCREEN_MODE:
					{
						if (Bluetooth_Mode == BLUETOOTH_VIEW_MESSAGE_MODE)	Bluetooth_Mode++;
						else	Bluetooth_Mode = BLUETOOTH_VIEW_MESSAGE_MODE;
						
						break;
					}
					
					case GEEKSPORT_SETTINGS_MENU:
					{
						if (Settings_Mode == SETTINGS_INFO_MODE)	Settings_Mode = SETTINGS_MODE;
						else	Buttons_SetButtonState = true;
						
						break;
					}
					
					default:	break;
				}
			}
			else	Buttons_SetButtonState = true;
		}
	}
	else	Buttons_Button2WasPushed = false;
	
//	###############################################################################################################################################

	if (!(bitIsSet(BUTTONS_PINS_2, BUTTONS_DDR_2_BUTTON_1_PIN)))							//	Set button
	{
		if (!Buttons_Button3WasPushed)
		{
			Buttons_Button3WasPushed = true;
			
			switch (mode)
			{
				uint8_t subMode;
				case GEEKSPORT_START:
				case GEEKSPORT_WATCH_MODE:
				case GEEKSPORT_SPEEDOMETER_MODE:
				
				case GEEKSPORT_SETTINGS_MENU:
				{
					GeekSport_ChangeSettingDigit();
					
					break;
				}
				
				case GEEKSPORT_BLUETOOTH_SCREEN_MODE:
				{
					subMode = Bluetooth_Mode;
					switch (subMode)
					{
						case BLUETOOTH_VIEW_MESSAGE_MODE:
						{
							if (Bluetooth_State)
							{
								USART_ReceiveString(Bluetooth_ReceiveBuffer);
// 								Bluetooth_ReceiveBuffer[0] = 'G';
// 								Bluetooth_ReceiveBuffer[1] = 'e';
// 								Bluetooth_ReceiveBuffer[2] = 'e';
// 								Bluetooth_ReceiveBuffer[3] = 'K';
// 								Bluetooth_ReceiveBuffer[4] = '\r';
// 								Bluetooth_ReceiveBuffer[5] = '\n';
// 								Bluetooth_ReceiveBuffer[6] = 'S';
							}
							
							break;
						}
						
						case BLUETOOTH_ON_OFF_COMMAND_MODE:
						{
							Bluetooth_State = !Bluetooth_State;
							Buttons_SetPressed = true;
							
							break;
						}
						
						default:	break;
					}
					
					break;
				}
				
// 				{
// 					Buttons_SetButtonState = true;
// 					if
// 					
// 					break;
// 				}
				
				default:	break;
			}
		}
	}
	else	Buttons_Button3WasPushed = false;
	
//	###############################################################################################################################################
	
	if (!(bitIsSet(BUTTONS_PINS_2, BUTTONS_DDR_2_BUTTON_2_PIN)))							//	LED Button
	{
		if (!Buttons_Button4WasPushed)
		{
			Buttons_Button4WasPushed = true;
			
			LCD_NOKIA_LED_PORT ^= 1 << LCD_NOKIA_LED_PIN;																//	Turn on LED
		}
	}
	else	Buttons_Button4WasPushed = false;
}