/*
 * I2C.c
 *
 * Created: 3/21/2018 4:12:09 PM
 * Author : Developer
 * 
 * Timers in using: TIMER0 (OCR0A), TIMER1 (OCR1A), TIMER1 (OCR1B), TIMER3 (OCR3A).
 */ 

#include "main.h"

// 	volatile uint8_t GeekSport_Mode = GEEKSPORT_START;
//uint8_t GeekSport_LastMode = GEEKSPORT_START;
// 
// 	volatile bool sleepModeOn = false;
// 	volatile bool goToSleepMode = false;
// 	volatile uint16_t timeToSleep = 0;
// 
// 	volatile uint8_t Settings_Mode = SETTINGS_MODE;
// 	
// 
// 	volatile /*static*/ bool Buttons_Button1WasPushed = 0;
// 	volatile /*static*/ bool Buttons_Button2WasPushed = 0;
// 	volatile /*static*/ bool Buttons_Button3WasPushed = 0;
// 	volatile /*static*/ bool Buttons_Button4WasPushed = 0;
// 
// 	volatile bool Buttons_SetButtonState = true;
// 	
// 	
// 	
// 	volatile uint16_t Speedometer_WheelLength_mm = 2074;									//	Default value
 	volatile bool Speedometer_IsRunning = false;
 	volatile uint8_t Speedometer_Mode = SPEEDOMETER_SPEED_MEASUREMENT_MODE;
 //	volatile uint32_t Speedometer_Data[5] = {0};

void Global_Init (void)
{

}

void IO_Init (void)
{
	
}

void setup (void)
{
	Global_Init();
	IO_Init();
	Buttons_Init();											//	Inputs initialization; initialization of Timer 0 on OCR0A
	Speedometer_Init();										//	Inputs initialization; initialization of external interrupts on INT0 pin
	DS1307_Init();
		
	I2C_Init(I2C_SCL_FREQUENCY, I2C_PRESCALER);
	SPI_Master_Init();
	
	LCD_Nokia_Init();
	
//	GeekSport_Init();
	
	sei();
}

int main (void)
{
	setup();
	
	while (1)
	{
// 		LCD_Nokia_ClearBuffer();
// 		LCD_Nokia_Send5CharString((int8_t *) "yyy");
// 		LCD_Nokia_UpdateFullScreen();
// 		_delay_ms(2000);
// 		cli();
// 		LCD_Nokia_ClearBuffer();
// 		LCD_Nokia_UpdateFullScreen();
// 		sleepModeOn = true;
// 		Geeksport_GoToSleep();
		
		GeekSport_SetMode(GeekSport_Mode);
		//_delay_ms(2000);
	}
}