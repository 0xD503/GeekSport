/*
 * GeekSport.c
 *
 * Created: 4/13/2018 7:34:46 PM
 *  Author: Developer
 */ 

#include "GeekSport.h"

volatile uint8_t GeekSport_Mode = /*GEEKSPORT_WATCH_MODE;*/GEEKSPORT_START;
static uint8_t GeekSport_LastMode = GEEKSPORT_START;


volatile uint8_t Settings_Mode = SETTINGS_MODE;//SETTINGS_INFO_MODE;

void GeekSport_LCD_Nokia_UpdateScreen (uint8_t xPos, uint8_t yPos)
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
			//while ()
			
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
			DS1307_Settings();
			
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
	static uint8_t lastBuffer = GEEKSPORT_START;
	
	switch (mode)
	{
		case GEEKSPORT_START:
		{
			if (GeekSport_LastMode != GEEKSPORT_START)//GEEKSPORT_WATCH_MODE)
			{
				GeekSport_LastMode = GEEKSPORT_START;//GEEKSPORT_WATCH_MODE;
				if (lastBuffer != GeekSport_LastMode)		//?????????????????????????
				{
					lastBuffer = mode;
					LCD_Nokia_ClearBuffer();
				}
			}
			GeekSport_SettingsMenu();
		}
		
		case GEEKSPORT_WATCH_MODE:
		{
			if (GeekSport_LastMode != GEEKSPORT_WATCH_MODE)
			{
				GeekSport_LastMode = GEEKSPORT_WATCH_MODE;
				if (lastBuffer != GeekSport_LastMode)		//?????????????????????????
				{
					lastBuffer = mode;
					LCD_Nokia_ClearBuffer();
				}
			}
			DS1307_LCD_Nokia_Main();
			
			break;
		}
		
		case GEEKSPORT_SPEEDOMETER_MODE:
		{
			if (GeekSport_LastMode != GEEKSPORT_SPEEDOMETER_MODE)
			{
 				GeekSport_LastMode = GEEKSPORT_SPEEDOMETER_MODE;
 				if (lastBuffer != GeekSport_LastMode)		//?????????????????????????
				{
					lastBuffer = mode;
					LCD_Nokia_ClearBuffer();
				}
			}
			Speedometer_LCD_Nokia_Main();
			
			break;
		}
		
		case GEEKSPORT_BLUETOOTH_SCREEN_MODE:
		{
			if (GeekSport_LastMode != GEEKSPORT_BLUETOOTH_SCREEN_MODE)
			{
				GeekSport_LastMode = GEEKSPORT_BLUETOOTH_SCREEN_MODE;
				if (lastBuffer != GeekSport_LastMode)
				{
					lastBuffer = mode;
					LCD_Nokia_ClearBuffer();
				}
			}
			BLuetooth_LCD_Nokia_Main();
			
			break;
		}
		
		case GEEKSPORT_SETTINGS_MENU:
		{
			if (GeekSport_LastMode != GEEKSPORT_SETTINGS_MENU)
			{
				GeekSport_LastMode = GEEKSPORT_SETTINGS_MENU;
				if (lastBuffer != GeekSport_LastMode)
				{
					lastBuffer = mode;
					LCD_Nokia_ClearBuffer();
				}
			}
			GeekSport_SettingsMenu();
			
			break;
		}
		
		default:
		{
			//	Error code
			
			break;
		}
	}
}