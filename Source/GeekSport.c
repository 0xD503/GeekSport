/*
 * GeekSport.c
 *
 * Created: 4/13/2018 7:34:46 PM
 *  Author: Developer
 */ 

#include "GeekSport.h"

volatile uint8_t GeekSport_Mode = GEEKSPORT_START;
uint8_t GeekSport_LastMode = GEEKSPORT_START;
// 
volatile bool sleepModeOn = false;
volatile bool goToSleepMode = false;
volatile uint16_t timeToSleep = 0;

volatile uint8_t Settings_Mode = SETTINGS_MODE;

void GeekSport_LCD_Nokia_UpdateDigitOnScreen (uint8_t xPos, uint8_t yPos)
{
	LCD_Nokia_SetPixelPosition(xPos, yPos);
	LCD_Nokia_Put5Char((int8_t) (GeekSport_SettingsDigit + ASCII_NULL));
	LCD_Nokia_UpdateFullScreen();
}

void GeekSport_ChangeSettingDigit (void)
{
	uint8_t tempDigit = GeekSport_SettingsDigit;
	if (tempDigit < 9)	tempDigit++;
	else	tempDigit = 0;
	GeekSport_SettingsDigit = tempDigit;
}

void GeekSport_TimerToSleep_Init (void)
{
	OCR3A = 0x3D0;//E4E1;												//	0x03D0 * 60 seconds
	TCCR3B |= 1 << WGM32 | 1 << CS32 | 0 << CS31 | 1 << CS30;			//	CTC mode, prescaler = 1024
	TIMSK3 |= 1 << OCIE3A;												//	Enable Interrupt
}

void Geeksport_WatchDogInit (void)
{
	WDTCSR |= 1 << WDCE | 1 << WDE;
	WDTCSR |= 1 << WDIE | 0 << WDP3 | 1 << WDP2 | 1 << WDP1 | 0 << WDP0;	//	1 s
}

void GeekSport_SleepModePreparations (void)
{
	ACSR |= 1 << ACD;													//	Disable an Analog Comparator
	ADCSRA = 0x00;														//	Don't know :D
	PRR0 |= 1 << PRTIM2 | 1 << PRUSART1 | 1 << PRADC;					//	Disable Timer 2, USART 1, ADC
	PRR1 |= 1 << PRTWI1 | 1 << PRPTC | 1 << PRTIM4 | 1 << PRSPI1;		//	Disable TWI 1, PTC, Timer 4, SPI 1
}

void Geeksport_GoToSleep (void)
{
	I2C_TurnOff();
	SPI_Master_Off();
	LCD_NOKIA_LED_PORT &= ~(1 << LCD_NOKIA_LED_PIN);
//	turn off LCD
	LCD_Nokia_SetPower(0);
//	ds1307, Bluetooth, etc..
	DS1307_POWER_PORT &= ~(1 << DS1307_POWER_PIN);
	SMCR &= ~(1 << SM2 | 1 << SM1 | 1 << SM0);							//	Set IDLE mode
	PRR0 |= 1 << PRTWI0 | 1 << PRTIM1 | 1 << PRSPI0 | 1 << PRUSART0;	//	Disable DS1307 communication, Speedometer Timers, SPI for a LCD, Bluetooth module communication
	SMCR |= 1 << SE;													//	Set SLEEP Enable bit
//	Before go to the IDLE mode, disable BOD by software
//	Writing to the BODS bit is controlled by a timed sequence and an enable bit.
	MCUCR |= 1 << BODS | 1 << BODSE;			//	Timed sequence
	MCUCR |= 1 << BODS;							//	Timed sequence
	MCUCR &= ~(1 << BODSE);						//	Timed sequence
// And now a sleep instruction must be executed within 3 cycles, while BODS = 1;
	sei();
	asm ("SLEEP");
}

void GeekSport_Init (void)
{
	//GeekSport_TimerToSleep_Init();
	GeekSport_SleepModePreparations();
	Geeksport_WatchDogInit();
}

void GeekSport_SettingsMenu (void)
{
	uint8_t mode = Settings_Mode;
	
	switch (mode)
	{
		case SETTINGS_INFO_MODE:
		{
			LCD_Nokia_SetPixelPosition(20, 2);
			LCD_Nokia_Send6CharString_P((int8_t *) GeekSportString);
			LCD_Nokia_SetPixelPosition(2, 12);
			LCD_Nokia_Send6CharString_P((int8_t *) developerInfo0);
			LCD_Nokia_SetPixelPosition(0, 21);
			LCD_Nokia_Send6CharString_P((int8_t *) developerInfo1);
			LCD_Nokia_SetPixelPosition(54, 40);
			LCD_Nokia_Send6CharString_P((int8_t *) developerInfo2);
			
			LCD_Nokia_UpdateFullScreen();
			
			break;
		}
		
		case SETTINGS_MODE:
		{
			LCD_Nokia_ClearBuffer();
			LCD_Nokia_SetPixelPosition(19, 0);
			LCD_Nokia_Send6CharString_P((int8_t *) settings);
			
			LCD_Nokia_SetPixelPosition(3, 11);
			LCD_Nokia_Send5CharString_P((int8_t *) setWheelLength);							//	Wheel length setting string
			
			LCD_Nokia_SetPixelPosition(0, 21);
			LCD_Nokia_Send6CharString_P((int8_t* ) timeSettingString);
			
			LCD_Nokia_SetPixelPosition(0, 30);
			LCD_Nokia_Send6CharString_P((int8_t* ) dateSettingString);
			
			LCD_Nokia_SetPixelPosition(0, 39);
			LCD_Nokia_Send6CharString_P((int8_t* ) weekdaySettingString);
			
			Speedometer_Settings();
//			DS1307_Settings();
			
			LCD_Nokia_ClearBuffer();
			LCD_Nokia_UpdateFullScreen();
			Settings_Mode = SETTINGS_INFO_MODE;
			GeekSport_Mode = GEEKSPORT_WATCH_MODE;
			
			break;
		}
	}
	
}

void GeekSport_SetMode (uint8_t mode)
{
	switch (mode)
	{
		case GEEKSPORT_START:
		{
			if (GeekSport_LastMode != GeekSport_Mode)
			{
 				GeekSport_LastMode = GeekSport_Mode;
				LCD_Nokia_ClearBuffer();
 			}
			GeekSport_SettingsMenu();
		}
		
		case GEEKSPORT_WATCH_MODE:
		{
			if (GeekSport_LastMode != GeekSport_Mode)
			{
				GeekSport_LastMode = GeekSport_Mode;
				LCD_Nokia_ClearBuffer();
 			}
			DS1307_LCD_Nokia_Main();
			
			break;
		}
		
		case GEEKSPORT_SPEEDOMETER_MODE:
		{
			if (GeekSport_LastMode != GeekSport_Mode)
			{
 				GeekSport_LastMode = GeekSport_Mode;
				LCD_Nokia_ClearBuffer();
			}
			Speedometer_LCD_Nokia_Main();
			
			break;
		}
		
		case GEEKSPORT_BLUETOOTH_SCREEN_MODE:
		{
			if (GeekSport_LastMode != GeekSport_Mode)
			{
				GeekSport_LastMode = GeekSport_Mode;
				LCD_Nokia_ClearBuffer();
			}
			BLuetooth_LCD_Nokia_Main();
			
			break;
		}
		
		case GEEKSPORT_SETTINGS_MENU:
		{
			if (GeekSport_LastMode != GeekSport_Mode)
			{
				GeekSport_LastMode = GeekSport_Mode;
				LCD_Nokia_ClearBuffer();
			}
			GeekSport_SettingsMenu();
			
			break;
		}
		
		case GEEKSPORT_SLEEP_MODE:
		{
			if (sleepModeOn)
			{
				USART_Init();
				USART_SendString((int8_t *) "sleep");
				Bluetooth_Off();
				cli();
				LCD_Nokia_ClearBuffer();
				LCD_Nokia_UpdateFullScreen();
				Geeksport_GoToSleep();
			}
			
			break;
		}
		
		default:
		{
			//	Error code
			
			break;
		}
	}
}

/*
ISR (TIMER3_COMPA_vect)
{
	if (sleepModeOn);
	else if (goToSleepMode && (!sleepModeOn))
	{
		timeToSleep++;
		if (timeToSleep >= 5)
		{
			sleepModeOn = true;
			GeekSport_Mode = GEEKSPORT_SLEEP_MODE;
		}
	}
	else
	{
		goToSleepMode = true;
		timeToSleep = 0;
	}
}
*/

ISR (WDT_vect)
{
	if (sleepModeOn)
	{
		if (!(bitIsSet(BUTTONS_PINS_2, BUTTONS_DDR_2_BUTTON_2_PIN)))							//	LED Button
		{
			if (!Buttons_Button4WasPushed)
			{
				Buttons_Button4WasPushed = true;
				
				SMCR &= ~(1 << SE);
				cli();
				PRR0 &= ~(1 << PRTWI0 | 1 << PRTIM1 | 1 << PRSPI0 | 1 << PRUSART0);
				DS1307_POWER_PORT |= 1 << DS1307_POWER_PIN;
				//LCD_Nokia_SetPower(1);
				
				SPI_Master_Init();
				I2C_Init(I2C_SCL_FREQUENCY, I2C_PRESCALER_BITS);
				//USART_Init();
				
				sleepModeOn = false;
				goToSleepMode = false;
				GeekSport_Mode = GEEKSPORT_WATCH_MODE;
				//sei();
			}
		}
		else	Buttons_Button4WasPushed = false;
	}
	else if (goToSleepMode && (!sleepModeOn))
	{
		timeToSleep++;
		if (timeToSleep >= 5)
		{
			sleepModeOn = true;
			GeekSport_Mode = GEEKSPORT_SLEEP_MODE;
		}
	}
	else
	{
		goToSleepMode = true;
		timeToSleep = 0;
	}
}