/*
 * DS1307.c
 *
 * Created: 4/5/2018 8:40:50 AM
 *  Author: Developer
 */ 

#include "DS1307.h"

volatile uint8_t DS1307_Mode = DS1307_WATCH_MODE;

struct DS1307_TimeRegisters TimeRegistersTransmitBuffer = {0};
struct DS1307_TimeRegisters TimeRegistersReceiveBuffer = {0};

struct DS1307_TimeRegisters *TimeRegPointer = NULL;


uint8_t binToBCD (uint8_t binNumber)
{
	uint8_t highDigit = (binNumber / 10) << 4;
	uint8_t lowDigit = binNumber % 10;
	return (highDigit | lowDigit);
}

uint8_t BCDToBin (uint8_t BCDNumber)
{
	uint8_t highDigit = BCDNumber >> 4;
	uint8_t lowDigit = BCDNumber & (~(0xF0));
	return (highDigit * 10 + lowDigit);
}



void DS1307_PutByte (int8_t byte, uint8_t DS1307_RAMAddress)
{
	int8_t *DS1307_DataString = NULL;
	DS1307_DataString[0] = DS1307_WRITE_ADDRESS;
	DS1307_DataString[1] = DS1307_RAMAddress;
	DS1307_DataString[2] = byte;
	I2C_MT_TransmitData(DS1307_DataString, 3);
}

// void DS1307_SendData (int8_t *dataPointer, uint8_t DS1307_RAMAddress)
// {
// 	int8_t *DS1307_DataString = NULL;
// 	DS1307_DataString[0] = DS1307_WRITE_ADDRESS;
// 	DS1307_DataString[1] = DS1307_RAMAddress;
// 	for (uint8_t i = 0; dataPointer[i] != 0; i++)	DS1307_DataString[i + 2] = dataPointer[i];
// 	I2C_MT_TransmitData(DS1307_DataString, i + 3);
// }

void DS1307_SendBCDTimeData_WithoutSeconds_24 (int8_t *BCDTimeDataPointer)
{
	int8_t *DS1307_DataString = NULL;
	DS1307_DataString[0] = DS1307_WRITE_ADDRESS;
	DS1307_DataString[1] = 0x00;
	DS1307_DataString[2] = 0x00;
	for (uint8_t i = 0; i < 6; i++)	DS1307_DataString[i + 3] = BCDTimeDataPointer[i];
	I2C_MT_TransmitData(DS1307_DataString, 9);
}

void DS1307_PutTimeByte (uint8_t byte, uint8_t DS1307_RAMAddress)
{
	byte = binToBCD(byte);
	DS1307_PutByte((uint8_t) byte, DS1307_RAMAddress);
}

void DS1307_SendBCDTimeStructData_WithoutSeconds_24(void)
{
	int8_t *DS1307_DataString = NULL;
	DS1307_DataString[0] = DS1307_WRITE_ADDRESS;
	DS1307_DataString[1] = 0x00;
	DS1307_DataString[2] = 0x00;
	DS1307_DataString[3] = TimeRegistersTransmitBuffer.minutes;
	DS1307_DataString[4] = TimeRegistersTransmitBuffer.hours;
	DS1307_DataString[5] = TimeRegistersTransmitBuffer.weekday;
	DS1307_DataString[6] = TimeRegistersTransmitBuffer.date;
	DS1307_DataString[7] = TimeRegistersTransmitBuffer.month;
	DS1307_DataString[8] = TimeRegistersTransmitBuffer.year;
	I2C_MT_TransmitData(DS1307_DataString, 9);
}


void DS1307_SetSeconds (uint8_t seconds)
{
	DS1307_PutTimeByte(seconds, 0x00);
}

void DS1307_SetMinutes (uint8_t minutes)
{
	DS1307_PutTimeByte(minutes, 0x01);
}

void DS1307_SetHours (uint8_t hours)
{
	DS1307_PutTimeByte(hours, 0x02);
}

void DS1307_SetWeekday (uint8_t weekday)
{
	DS1307_PutTimeByte(weekday, 0x03);
}

void DS1307_SetDate (uint8_t date)
{
	DS1307_PutTimeByte(date, 0x04);
}

void DS1307_SetMonth (uint8_t month)
{
	DS1307_PutTimeByte(month, 0x05);
}

void DS1307_SetYear (uint8_t year)
{
	DS1307_PutTimeByte(year, 0x06);
}




void DS1307_ReceiveData (int8_t* placeToReceive, uint8_t DS1307_RAMAddress, uint8_t numberOfBytes)
{
	static int8_t* DS1307_DataString = NULL;
	DS1307_DataString[0] = DS1307_WRITE_ADDRESS;
	DS1307_DataString[1] = DS1307_RAMAddress;
	I2C_MT_TransmitData(DS1307_DataString, 2);
	DS1307_DataString[0] = DS1307_READ_ADDRESS;
	I2C_MT_TransmitData(DS1307_DataString, numberOfBytes);
	
	I2C_MR_ReceiveData(placeToReceive, numberOfBytes);
}

void DS1307_ReceiveTimeData (void)
{
	TimeRegPointer = &TimeRegistersReceiveBuffer;
	DS1307_ReceiveData((int8_t* ) TimeRegPointer, 0, 8);
	TimeRegPointer->seconds = BCDToBin(TimeRegPointer->seconds);
	TimeRegPointer->minutes = BCDToBin(TimeRegPointer->minutes);
	TimeRegPointer->hours = BCDToBin(TimeRegPointer->hours);
	TimeRegPointer->weekday = BCDToBin(TimeRegPointer->weekday);
	TimeRegPointer->date = BCDToBin(TimeRegPointer->date);
	TimeRegPointer->month = BCDToBin(TimeRegPointer->month);
	TimeRegPointer->year = BCDToBin(TimeRegPointer->year);
}

// void DS1307_UpdateTime_12 (void)
// {
// 	TimeRegPointer = &TimeRegistersTransmitBuffer;
// 	DS1307_SetSeconds(TimeRegPointer->seconds);
// 	DS1307_SetMinutes(TimeRegPointer->minutes);
// 	DS1307_SetHours(TimeRegPointer->hours);// | DS1307_SET_12_HOUR_FORMAT);
// 	DS1307_SetWeekday(TimeRegPointer->weekday);
// 	DS1307_SetDate(TimeRegPointer->date);
// 	DS1307_SetMonth(TimeRegPointer->month);
// 	DS1307_SetYear(TimeRegPointer->year);
// }

void DS1307_UpdateTime_24 (void)
{
	TimeRegPointer = &TimeRegistersTransmitBuffer;
	DS1307_SetSeconds(TimeRegPointer->seconds);
	DS1307_SetMinutes(TimeRegPointer->minutes);
	DS1307_SetHours(TimeRegPointer->hours);
	DS1307_SetWeekday(TimeRegPointer->weekday);
	DS1307_SetDate(TimeRegPointer->date);
	DS1307_SetMonth(TimeRegPointer->month);
	DS1307_SetYear(TimeRegPointer->year);
}


void DS1307_SetTime (void)											//	Before setting time, put the time value to the TimeReceiveBuffer
{
	//DS1307_ReceiveTimeData();							//					?????????????????????????????????????????????
	TimeRegPointer = &TimeRegistersReceiveBuffer;
	TimeRegistersTransmitBuffer.seconds = TimeRegPointer->seconds;
	TimeRegistersTransmitBuffer.minutes = TimeRegPointer->minutes;
	TimeRegistersTransmitBuffer.hours = TimeRegPointer->hours;
	TimeRegistersTransmitBuffer.weekday = TimeRegPointer->weekday;
	TimeRegistersTransmitBuffer.date = TimeRegPointer->date;
	TimeRegistersTransmitBuffer.month = TimeRegPointer->month;
	TimeRegistersTransmitBuffer.year = TimeRegPointer->year;
	
	DS1307_UpdateTime_24();
}

void DS1307_SelectWeekday (uint8_t weekday)
{
	switch (weekday)
	{
		
	}
}

void DS1307_LCD_Nokia_SetTimeRegisterInline (uint8_t xPos, uint8_t yPos, uint8_t timeRegister)
{
	uint8_t tempWatchDigit;
	
	GeekSport_SettingsDigit = 0;
	Buttons_SetButtonState = false;
	while (!Buttons_SetButtonState)	GeekSport_LCD_Nokia_UpdateScreen(xPos, yPos);
	
	Buttons_SetButtonState = false;
	tempWatchDigit = GeekSport_SettingsDigit << 4;
	
	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState)	GeekSport_LCD_Nokia_UpdateScreen(xPos + 6, yPos);
	
	tempWatchDigit |= GeekSport_SettingsDigit;
	timeRegister = tempWatchDigit;
}



void DS1307_Settings (void)
{
// 	DS1307_LCD_Nokia_SetTimeRegisterInline(36, 21, TimeRegistersTransmitBuffer.hours);
// 	DS1307_LCD_Nokia_SetTimeRegisterInline(54, 21, TimeRegistersTransmitBuffer.minutes);
// 	DS1307_LCD_Nokia_SetTimeRegisterInline(36, 30, TimeRegistersTransmitBuffer.date);
// 	DS1307_LCD_Nokia_SetTimeRegisterInline(54, 30, TimeRegistersTransmitBuffer.month);
// 	DS1307_LCD_Nokia_SetTimeRegisterInline(72, 30, TimeRegistersTransmitBuffer.year);
 	
	 uint8_t tempWatchDigit;
	
	GeekSport_SettingsDigit = 0;
	Buttons_SetButtonState = false;
	while (!Buttons_SetButtonState)
	{
		if (GeekSport_SettingsDigit > 2)	GeekSport_SettingsDigit = 0;
		GeekSport_LCD_Nokia_UpdateScreen(37, 21);
	}
	Buttons_SetButtonState = false;
	tempWatchDigit = (GeekSport_SettingsDigit << 4);
	
	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState)	GeekSport_LCD_Nokia_UpdateScreen(43, 21);
	
	Buttons_SetButtonState = false;
	tempWatchDigit |= GeekSport_SettingsDigit;
	TimeRegistersTransmitBuffer.hours = tempWatchDigit;										//	Set hours
	
	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState)
	{
		if (GeekSport_SettingsDigit > 5)	GeekSport_SettingsDigit = 0;
		GeekSport_LCD_Nokia_UpdateScreen(55, 21);
	}
	Buttons_SetButtonState = false;
	tempWatchDigit = (GeekSport_SettingsDigit << 4);
	
	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState)	GeekSport_LCD_Nokia_UpdateScreen(61, 21);
	
	Buttons_SetButtonState = false;
	tempWatchDigit |= GeekSport_SettingsDigit;
	TimeRegistersTransmitBuffer.minutes = tempWatchDigit;									//	Set minutes
	
	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState)
	{
		if (GeekSport_SettingsDigit > 3)	GeekSport_SettingsDigit = 0;
		GeekSport_LCD_Nokia_UpdateScreen(37, 30);
	}
	Buttons_SetButtonState = false;
	tempWatchDigit = (GeekSport_SettingsDigit << 4);
	
	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState)	GeekSport_LCD_Nokia_UpdateScreen(43, 30);
	
	Buttons_SetButtonState = false;
	tempWatchDigit |= GeekSport_SettingsDigit;
	TimeRegistersTransmitBuffer.date = tempWatchDigit;										//	Set	date
	
	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState)
	{
		if (GeekSport_SettingsDigit > 1)	GeekSport_SettingsDigit = 0;
		GeekSport_LCD_Nokia_UpdateScreen(55, 30);
	}
	Buttons_SetButtonState = false;
	tempWatchDigit = (GeekSport_SettingsDigit << 4);
	
	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState)	GeekSport_LCD_Nokia_UpdateScreen(61, 30);
	
	Buttons_SetButtonState = false;
	tempWatchDigit |= GeekSport_SettingsDigit;
	TimeRegistersTransmitBuffer.month = tempWatchDigit;											//	Set	month
	
	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState)	GeekSport_LCD_Nokia_UpdateScreen(73, 30);
	
	Buttons_SetButtonState = false;
	tempWatchDigit = (GeekSport_SettingsDigit << 4);
	
	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState)	GeekSport_LCD_Nokia_UpdateScreen(79, 30);
	
	Buttons_SetButtonState = false;
	tempWatchDigit |= GeekSport_SettingsDigit;
	TimeRegistersTransmitBuffer.year = tempWatchDigit;											//	Set	year
	

	GeekSport_SettingsDigit = 0;
	while (!Buttons_SetButtonState)
	{
		if (GeekSport_SettingsDigit > 6)	GeekSport_SettingsDigit = 0;
		LCD_Nokia_SetPixelPosition(60, 39);
		LCD_Nokia_Send6CharString_P((int8_t *) WeekdaysPointer[GeekSport_SettingsDigit]);
		LCD_Nokia_UpdateFullScreen();
	}
	
	tempWatchDigit = GeekSport_SettingsDigit + 1;
	TimeRegistersTransmitBuffer.weekday = tempWatchDigit;										//	Set	the day of week
	
	DS1307_SendBCDTimeStructData_WithoutSeconds_24();
}

void DS1307_LCD_Nokia_Main (void)
{
	switch (DS1307_Mode)
	{
		case DS1307_WATCH_MODE:
		{
			DS1307_ReceiveTimeData();
			
			LCD_Nokia_DrawUnfilledRectangle(0, 7, 83, 47, 4);
			LCD_Nokia_SetPixelPosition(12, 12);
			LCD_Nokia_Send12Integer(TimeRegPointer->hours);
			LCD_Nokia_Put12Char(':');
			LCD_Nokia_Send12Integer(TimeRegPointer->minutes);
			LCD_Nokia_SetPixelPosition(6, 30);
			LCD_Nokia_Send6CharString_P((int8_t* ) WeekdaysPointer[TimeRegPointer->weekday - 1]);
			LCD_Nokia_Send6CharString_P((int8_t* ) PSTR (", "));
			LCD_Nokia_Send6CharString_P((int8_t* ) MonthsPointer[TimeRegPointer->month - 1]);
			LCD_Nokia_Put6Char(' ');
			LCD_Nokia_Send5Integer(TimeRegPointer->date);
			LCD_Nokia_UpdateFullScreen();
			
			break;
		}
		case DS1307_TIME_SETTING_MODE:
		{
			LCD_Nokia_ClearBuffer();
			
			LCD_Nokia_SetPixelPosition(0, 0);
			LCD_Nokia_Send6CharString_P((int8_t *) timeSettings);
			
			LCD_Nokia_SetPixelPosition(0, 21);
			LCD_Nokia_Send6CharString_P((int8_t* ) timeSettingString);
			
			LCD_Nokia_SetPixelPosition(0, 30);
			LCD_Nokia_Send6CharString_P((int8_t* ) dateSettingString);
			
			LCD_Nokia_SetPixelPosition(0, 39);
			LCD_Nokia_Send6CharString_P((int8_t* ) weekdaySettingString);
			
			DS1307_Settings();
			
			LCD_Nokia_ClearBuffer();
			DS1307_Mode = DS1307_WATCH_MODE;
			break;
		}
		
		default:	break;
	}
}


void DS1307_SetTwelveHourMode (void)
{
	//	Set time format to 12 or 24 hour
	DS1307_ReceiveTimeData();
	DS1307_SetHours(TimeRegPointer->hours | DS1307_SET_12_HOUR_FORMAT);				//	Set 6 bit to high
}

// void DS1307_Clock (void)
// {
// 	//	Set CH bit to 1 or 0 in the 00h RTC register
// 	DS1307_PutByte(DS1307_CLOCK_HALT, 0x00);
// }

// void DS1307_EnableSQW (uint8_t frequency)
// {
	//	
	
// }