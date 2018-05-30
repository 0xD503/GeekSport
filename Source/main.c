/*
 * I2C.c
 *
 * Created: 3/21/2018 4:12:09 PM
 * Author : Developer
 * 
 * Timers in using: TIMER0 (OCR0A), TIMER1 (OCR1A), TIMER1 (OCR1B).
 */ 

#include "main.h"

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