#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SPI_MASTER_MODE

#include "Global.h"

enum GEEKSPORT_MODES {GEEKSPORT_START, GEEKSPORT_WATCH_MODE, GEEKSPORT_SPEEDOMETER_MODE, GEEKSPORT_BLUETOOTH_SCREEN_MODE, GEEKSPORT_SETTINGS_MENU, GEEKSPORT_LAST_MODE, GEEKSPORT_SLEEP_MODE = 86};
enum GEEKSPORT_SETTINGS_MODES {SETTINGS_INFO_MODE, SETTINGS_MODE};

volatile uint8_t GeekSport_Mode;
volatile uint16_t GeekSport_SettingsDigit;
//uint8_t GeekSport_LastMode;

volatile bool sleepModeOn;
volatile bool goToSleepMode;
volatile uint16_t timeToSleep;

volatile uint8_t Settings_Mode;



volatile /*static*/ bool Buttons_Button1WasPushed;
volatile /*static*/ bool Buttons_Button2WasPushed;
volatile /*static*/ bool Buttons_Button3WasPushed;
volatile /*static*/ bool Buttons_Button4WasPushed;

volatile bool Buttons_SetButtonState;
volatile bool Buttons_SetPressed;



enum SPEEDOMETER_MODES {SPEEDOMETER_SPEED_MEASUREMENT_MODE, SPEEDOMETER_SCREEN_MODE, SPEEDOMETER_WHEEL_LENGTH_SETING_MODE, SPEEDOMETER_LAST_MODE};

//volatile uint16_t Speedometer_WheelLength_mm;
volatile bool Speedometer_IsRunning;					//	When this variable is true, speedometer is processing, OCR0B Interrupt is true
volatile uint8_t Speedometer_Mode;

//volatile uint32_t Speedometer_Speed_mmps;				//	Value, in that stored current speed (mm per seconds)
volatile int8_t Speedometer_RevolutionCounter;			//	Counter of the quantity of revolutions (External interrupts)
volatile uint8_t Speedometer_TimerCounter;				//

//volatile uint32_t Speedometer_Data[5];


//

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