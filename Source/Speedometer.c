/*
 * Speedometer.c
 *
 * Created: 4/9/2018 2:59:03 PM
 *  Author: Developer
 */ 

#include "Speedometer.h"


// volatile uint16_t Speedometer_WheelLength_mm = 2074;									//	Default value
// volatile bool Speedometer_IsRunning = false;
// volatile uint8_t Speedometer_Mode = SPEEDOMETER_SPEED_MEASUREMENT_MODE;

// volatile /*static*/ uint32_t totalDistance;// = 0;
// volatile /*static*/ uint32_t totalTime;// = 0;
// volatile /*static*/ uint32_t averageVelocity;// = 0;
// volatile /*static*/ uint32_t maxVelocity;// = 0;
// volatile /*static*/ uint32_t currentDistance;// = 0;

//volatile uint32_t Speedometer_Data[5] = {0};


void Speedometer_IO_Init (void)
{
	SPEEDOMETER_HALL_SWITCH_DDR &= ~(1 << SPEEDOMETER_HALL_SWITCH_PIN);
	SPEEDOMETER_HALL_SWITCH_PORT |= 1 << SPEEDOMETER_HALL_SWITCH_PIN;
}

void Speedometer_Init (void)
{
	Speedometer_IO_Init();
	EICRA |= 1 << ISC01 | 0 << ISC00;															//	Set interrupts on failing edge for the reed switch
	EIMSK |= 1 << INT0;																			//	Enable interrupts
}

void Speedometer_Settings (void)
{
	uint16_t tempLength;
	
//	Wheel length setting
	GeekSport_SettingsDigit = 0;
	Buttons_SetButtonState = false;
	while (!Buttons_SetButtonState)	GeekSport_LCD_Nokia_UpdateDigitOnScreen(63, 11);
	
	Buttons_SetButtonState = false;
	tempLength = (GeekSport_SettingsDigit * 1000);
	
	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState)	GeekSport_LCD_Nokia_UpdateDigitOnScreen(68, 11);
	
	Buttons_SetButtonState = false;
	tempLength += (GeekSport_SettingsDigit * 100);
	
	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState)	GeekSport_LCD_Nokia_UpdateDigitOnScreen(73, 11);
	
	Buttons_SetButtonState = false;
	tempLength += (GeekSport_SettingsDigit * 10);
	
	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState) GeekSport_LCD_Nokia_UpdateDigitOnScreen(78, 11);
	
	tempLength += GeekSport_SettingsDigit;
	
	TimeRegPointer = &Speedometer_Buffer;
	TimeRegPointer->Speedometer_WheelLength = tempLength;
}

void Speedometer_LCD_Nokia_Main (void)
{
	switch (Speedometer_Mode)
	{
		case SPEEDOMETER_SPEED_MEASUREMENT_MODE:
		{
			//LCD_Nokia_ClearBuffer();									/////////
			DS1307_ReceiveTimeData();
			
			//	Out the time
			LCD_Nokia_SetPixelPosition(59, 0);
			LCD_Nokia_Send5Integer(TimeRegPointer->hours);
			LCD_Nokia_Put5Char(':');
			LCD_Nokia_Send5Integer(TimeRegPointer->minutes);
			
			//	Out the current speed
			TimeRegPointer = &Speedometer_Buffer;
			uint32_t temp = TimeRegPointer->Speedometer_Speed;
			LCD_Nokia_SetPixelPosition(0, 12);
			if (temp == 0)
			{
				LCD_Nokia_Send12CharString_P((int8_t *) PSTR("00"));
			}
			else if (((temp * 36) / 10000) < 10)
			{
				LCD_Nokia_Put12Char('0');
				LCD_Nokia_Put12Char(((temp * 36) / 10000) + ASCII_NULL);
			}
			else
			{
				LCD_Nokia_Put12Char((((temp * 36) / 10000) / 10) + ASCII_NULL);
				LCD_Nokia_Put12Char((((temp * 36) / 10000) % 10) + ASCII_NULL);
			}
			LCD_Nokia_SetPixelPosition(23, 21);
			LCD_Nokia_Put5Char('.');
			LCD_Nokia_SetPixelPosition(26, 12);
			
			if ((((temp * 36) % 10000) > 100000) || (((temp * 36) % 10000) == 0))	LCD_Nokia_Put12Char(ASCII_NULL);
			else if ((((temp * 36) % 10000) > 10000) && (((temp * 36) % 10000) <= 100000))	LCD_Nokia_Put12Char((((temp * 36) % 10000) / 10000) + ASCII_NULL);
			else if ((((temp * 36) % 10000) > 1000) && (((temp * 36) % 10000) <= 10000))	LCD_Nokia_Put12Char((((temp * 36) % 10000) / 1000) + ASCII_NULL);
			else if ((((temp * 36) % 10000) > 100) && (((temp * 36) % 10000) <= 1000))	LCD_Nokia_Put12Char((((temp * 36) % 10000) / 100) + ASCII_NULL);
			else if ((((temp * 36) % 10000) > 10) && (((temp * 36) % 10000) <= 100))	LCD_Nokia_Put12Char((((temp * 36) % 10000) / 10) + ASCII_NULL);
			
			LCD_Nokia_SetPixelPosition(43, 21);
			LCD_Nokia_Send6CharString_P((int8_t* ) kmh);
			
			//	Out another data
			LCD_Nokia_SetPixelPosition(0 , 33);
			LCD_Nokia_Send6CharString_P((int8_t* ) odoString);
			LCD_Nokia_Send6Integer(TimeRegPointer->totalDistance / 1000);
			LCD_Nokia_Send6CharString_P((int8_t* ) km);
			LCD_Nokia_SetPixelPosition(0, 41);
			LCD_Nokia_Send6CharString_P((int8_t* ) dstString);
			LCD_Nokia_Send6Integer(TimeRegPointer->currentDistance / 1000);
			LCD_Nokia_SetPixelPosition(48, 41);
			LCD_Nokia_Put5Char('.');
			LCD_Nokia_SetPixelPosition(51, 41);
			LCD_Nokia_Send6Integer(TimeRegPointer->currentDistance % 1000);
			LCD_Nokia_Send6CharString_P((int8_t* ) km);
			
			LCD_Nokia_UpdateFullScreen();
			
			break;
		}
		
		case SPEEDOMETER_SCREEN_MODE:
		{
			LCD_Nokia_ClearBuffer();
			
			Buttons_SetButtonState = false;
			while (!Buttons_SetButtonState)
			{
				LCD_Nokia_SetPixelPosition(0,8);
				LCD_Nokia_Send6CharString_P((int8_t *) odoString);
				LCD_Nokia_Send6Integer(TimeRegPointer->totalDistance / 1000);
				LCD_Nokia_Send5CharString_P((int8_t* ) km);
				
				LCD_Nokia_SetPixelPosition(0,16);
				LCD_Nokia_Send6CharString_P((int8_t *) timeString);
				if ((TimeRegPointer->totalTime / 3600) > 9)	LCD_Nokia_Send6Integer(TimeRegPointer->totalTime / 3600);
				else
				{
					LCD_Nokia_Put6Char('0');
					LCD_Nokia_Put6Char((TimeRegPointer->totalTime / 3600) + ASCII_NULL);
				}
				LCD_Nokia_Put6Char(':');
				if (((TimeRegPointer->totalTime % 3600) / 60) > 9)	LCD_Nokia_Send6Integer((TimeRegPointer->totalTime % 3600) / 60);
				else
				{
					LCD_Nokia_Put6Char('0');
					LCD_Nokia_Put6Char(((TimeRegPointer->totalTime % 3600) / 60) + ASCII_NULL);
				}
				LCD_Nokia_Put6Char(':');
				if (((TimeRegPointer->totalTime % 3600) % 60) > 9)	LCD_Nokia_Send6Integer((TimeRegPointer->totalTime % 3600) % 60);
				else
				{
					LCD_Nokia_Put6Char('0');
					LCD_Nokia_Put6Char(((TimeRegPointer->totalTime % 3600) % 60) + ASCII_NULL);
				}
				
				LCD_Nokia_SetPixelPosition(0,24);
				LCD_Nokia_Send6CharString_P((int8_t *) averageVelocityString);
				LCD_Nokia_Send6Integer((TimeRegPointer->averageVelocity * 3600) / 1000);
				LCD_Nokia_Send5CharString_P((int8_t* ) kmh);
				
				LCD_Nokia_SetPixelPosition(0,32);
				LCD_Nokia_Send6CharString_P((int8_t *) maxVelocityString);
				LCD_Nokia_Send6Integer((TimeRegPointer->maxVelocity * 3600) / 1000);
				LCD_Nokia_Send5CharString_P((int8_t* ) kmh);
				
				LCD_Nokia_SetPixelPosition(0,40);
				LCD_Nokia_Send6CharString_P((int8_t* ) dstString);
				LCD_Nokia_Send6Integer(TimeRegPointer->currentDistance / 1000);
				LCD_Nokia_SetPixelPosition(48, 40);
				LCD_Nokia_Put5Char('.');
				LCD_Nokia_SetPixelPosition(51, 40);
				LCD_Nokia_Send6Integer(TimeRegPointer->currentDistance % 1000);
				LCD_Nokia_Send5CharString_P((int8_t* ) km);
				
				LCD_Nokia_UpdateFullScreen();
			}
			
			LCD_Nokia_ClearBuffer();
			break;
		}
		
		case SPEEDOMETER_WHEEL_LENGTH_SETING_MODE:
		{
			LCD_Nokia_ClearBuffer();
			
			LCD_Nokia_SetPixelPosition(3, 11);
			LCD_Nokia_Send5CharString_P((int8_t *) setWheelLength);							//	Wheel length setting string
			
			Speedometer_Settings();
			
			LCD_Nokia_ClearBuffer();
			Speedometer_Mode = SPEEDOMETER_SPEED_MEASUREMENT_MODE;
			break;
		}
		
		default:	break;
	}
}



//	Update time: 1 second;
//	Stability: Normal;
//	Accuracy: Good;
//	Problems: unstability on velocities > 50 km/h;
ISR (INT0_vect)
{
	TimeRegPointer = &Speedometer_Buffer;
	if (!Speedometer_IsRunning)
	{
		Speedometer_IsRunning = true;
		Speedometer_RevolutionCounter = 1;
		Speedometer_TimerCounter = 1;
		
		OCR1A = 0x03D0;																					//	If F_CPU = 1MHz and prescaler = 1024, T = 1 second
		OCR1B = 0x03D0;//?????????????????????
		TCCR1B |= 1 << WGM12 | 1 << CS12 | 0 << CS11 | 1 << CS10;										//	Prescaler = F_CPU / 1024
		TIMSK1 |= 1 << OCIE1A | 1 << OCIE1B;															//	Enable timer interrupts
	}
	else	Speedometer_RevolutionCounter++;
}

ISR (TIMER1_COMPA_vect)
{
	TimeRegPointer = &Speedometer_Buffer;
	if (Speedometer_TimerCounter < SPEEDOMETER_GENERAL_SAMPLE_TIME)
	{
		TimeRegPointer->Speedometer_Speed = (TimeRegPointer->Speedometer_WheelLength * Speedometer_RevolutionCounter) / Speedometer_TimerCounter;
		Speedometer_TimerCounter++;
	}
	else
	{
		TimeRegPointer->Speedometer_Speed = (TimeRegPointer->Speedometer_WheelLength * Speedometer_RevolutionCounter) / Speedometer_TimerCounter;
		if (TimeRegPointer->Speedometer_Speed == 0)
		{
			TCCR1B &= ~(1 << WGM12 | 1 << CS12 | 0 << CS11 | 1 << CS10);
			TIMSK1 &= ~(1 << OCIE1A | OCIE1B);
			Speedometer_IsRunning = false;
			
			return;
		}
		Speedometer_RevolutionCounter = 0;
		Speedometer_TimerCounter = 1;
	}
}

ISR (TIMER1_COMPB_vect)
{
	TimeRegPointer = &Speedometer_Buffer;
	uint32_t currentVelocity = TimeRegPointer->Speedometer_Speed / 1000;
	TimeRegPointer->totalDistance += currentVelocity;
	TimeRegPointer->currentDistance += currentVelocity;
	TimeRegPointer->totalTime++;
	TimeRegPointer->averageVelocity = TimeRegPointer->totalDistance / TimeRegPointer->totalTime;
	if (currentVelocity > TimeRegPointer->maxVelocity)	TimeRegPointer->maxVelocity = currentVelocity;
}