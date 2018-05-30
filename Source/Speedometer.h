/*
 * Speedometer.h
 *
 * Created: 4/9/2018 2:57:48 PM
 *  Author: Developer
 */ 


#ifndef SPEEDOMETER_H_
#define SPEEDOMETER_H_

#include "main.h"

#define SPEEDOMETER_HALL_SWITCH_DDR	DDRD
#define SPEEDOMETER_HALL_SWITCH_PORT PORTD
#define SPEEDOMETER_HALL_SWITCH_PINS PIND
#define SPEEDOMETER_HALL_SWITCH_PIN 2

#define SPEEDOMETER_TIMER_PRESCALER 256
#define SPEEDOMETER_MAX_DELAY 5
#define SPEEDOMETER_TIMER_OCR_VVALUE 1																						//	Determine period and frequency of the timer
#define SPEEDOMETER_RESOLUTION_TIME_US (SPEEDOMETER_TIMER_PRESCALER * SPEEDOMETER_TIMER_OCR_VVALUE)
#define SPEEDOMETER_MAX_TIMER_COUNTER_VALUE (((F_CPU / SPEEDOMETER_TIMER_PRESCALER) / SPEEDOMETER_TIMER_OCR_VVALUE) * SPEEDOMETER_MAX_DELAY)

#define SPEEDOMETER_GENERAL_SAMPLE_TIME 5

/*extern*/volatile static uint16_t Speedometer_WheelLength_mm;				//	Length of the wheel in mm

volatile static bool Speedometer_IsRunning;					//	When this variable is true, speedometer is processing, OCR0B Interrupt is true
volatile uint8_t Speedometer_Mode;

volatile static uint32_t Speedometer_Speed_mmps;				//	Value, in that stored current speed (mm per seconds)
volatile static uint8_t Speedometer_RevolutionCounter;			//	Counter of the quantity of revolutions (External interrupts)
volatile static uint8_t Speedometer_TimerCounter;				//	



enum SPEEDOMETER_MODES {SPEEDOMETER_SPEED_MEASUREMENT_MODE, SPEEDOMETER_WHEEL_LENGTH_SETING_MODE, SPEEDOMETER_LAST_MODE};


void Speedometer_Init (void);
void Speedometer_Settings (void);

void Speedometer_LCD_Nokia_Main (void);

#endif /* SPEEDOMETER_H_ */