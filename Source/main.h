#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SPI_MASTER_MODE

volatile uint8_t GeekSport_Mode;
volatile uint16_t GeekSport_SettingsDigit;
volatile uint8_t Settings_Mode;

volatile /*static */bool Buttons_Button1WasPushed;
volatile /*static */bool Buttons_Button2WasPushed;
volatile /*static */bool Buttons_Button3WasPushed;
volatile /*static */bool Buttons_Button4WasPushed;
volatile bool Buttons_SetButtonState;
volatile bool Buttons_SetPressed;

/*extern*/volatile /*static*/ uint16_t Speedometer_WheelLength_mm;				//	Length of the wheel in mm

volatile /*static */bool Speedometer_IsRunning;				//	When this variable is true, speedometer is processing, OCR0B Interrupt is true
volatile uint8_t Speedometer_Mode;

volatile /*static */uint32_t Speedometer_Speed_mmps;				//	Value, in that stored current speed (mm per seconds)
volatile /*static */uint8_t Speedometer_RevolutionCounter;			//	Counter of the quantity of revolutions (External interrupts)
volatile /*static */uint8_t Speedometer_TimerCounter;				//

volatile /*static */uint32_t totalDistance;
volatile /*static */uint32_t totalTime;
volatile /*static */uint32_t averageVelocity;
volatile /*static */uint32_t maxVelocity;
volatile /*static */uint32_t currentDistance;

#include "I2C.h"
#include "SPI.h"
#include "USART.h"

#include "Buttons.h"
#include "DS1307.h"
#include "LCD_Nokia_5110.h"
#include "Speedometer.h"
#include "Bluetooth_Functions.h"

#include "DS1307_ProgramMemory.h"
#include "LCD_Nokia_5110_charset.h"
#include "GeekSport_Symbols.h"

#include "GeekSport.h"

#endif /* MAIN_H_ */