/*
 * I2C.c
 *
 * Created: 3/21/2018 4:12:09 PM
 * Author : Developer
 * 
 * Timers in using: TIMER0 (OCR0A), TIMER1 (OCR1A), TIMER1 (OCR1B).
 */ 

#include "main.h"

volatile uint8_t GeekSport_Mode = /*GEEKSPORT_WATCH_MODE;*/GEEKSPORT_START;
volatile uint8_t Settings_Mode = SETTINGS_MODE;//SETTINGS_INFO_MODE;

volatile /*static */bool Buttons_Button1WasPushed = 0;
volatile /*static */bool Buttons_Button2WasPushed = 0;
volatile /*static */bool Buttons_Button3WasPushed = 0;
volatile /*static */bool Buttons_Button4WasPushed = 0;

volatile bool Buttons_SetButtonState = true;

volatile /*static */uint16_t Speedometer_WheelLength_mm = 2074;									//	Default value
volatile uint8_t Speedometer_Mode = SPEEDOMETER_SPEED_MEASUREMENT_MODE;
volatile /*static */bool Speedometer_IsRunning = false;
volatile /*static */uint32_t totalDistance = 0;
volatile /*static */uint32_t totalTime = 0;
volatile /*static */uint32_t averageVelocity = 0;
volatile /*static */uint32_t maxVelocity = 0;
volatile /*static */uint32_t currentDistance = 0;

void IO_Init (void)
{
	
}

void setup (void)
{
	IO_Init();
	Buttons_Init();											//	Inputs initialization; initialization of Timer 0 on OCR0A
	Speedometer_Init();										//	Inputs initialization; initialization of external interrupts on INT0 pin
	
	I2C_Init(I2C_SCL_FREQUENCY, I2C_PRESCALER_BITS);
	SPI_Master_Init();
//	Bluetooth_On();											//	USART initialization
	
	LCD_Nokia_Init();
	
//	GeekSport_Init();										//	Setting time, length of the wheel in mm
	
	sei();
}

int main (void)
{
	setup();
	
	while (1)
	{
		GeekSport_SetMode(GeekSport_Mode);
		//_delay_ms(2000);
	}
}