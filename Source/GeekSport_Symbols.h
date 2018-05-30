/*
 * GeekSport_Symbols.h
 *
 * Created: 4/16/2018 7:10:26 PM
 *  Author: Developer
 */ 


#ifndef GEEKSPORT_SYMBOLS_H_
#define GEEKSPORT_SYMBOLS_H_

#include <avr/pgmspace.h>

static const int8_t GeekSportString[] PROGMEM = "GeekSport";
static const int8_t developerInfo0[] PROGMEM = "Developer:";
static const int8_t developerInfo1[] PROGMEM = "RV Electronics";
static const int8_t developerInfo2[] PROGMEM = "2018";
static const int8_t connection[] PROGMEM = "Connection...";
static const int8_t success[] PROGMEM = "Success!!!";
static const int8_t fail[] PROGMEM = "Fail!";

static const uint8_t GeekSportSymbols[][6] PROGMEM = {
		{0x7E, 0x51, 0x4A, 0x4A, 0x51, 0x7E}	//	New message
	,	{0x42, 0x24, 0x18, 0xFF, 0xA5, 0x42}	//	Bluetooth label
	};


static const int8_t settings[] PROGMEM = "Settings";


static const int8_t timeSettings[] PROGMEM = "Time settings:";


static const int8_t setWheelLength[] PROGMEM = "Wheel (mm): 0000";
static const int8_t km[] PROGMEM = " km";
static const int8_t kmh[] PROGMEM = " km/h";
static const int8_t odoString[] PROGMEM = "Odo: ";
static const int8_t timeString[] PROGMEM = "Time: ";
static const int8_t maxVelocityString[] PROGMEM = "Max: ";
static const int8_t averageVelocityString[] PROGMEM = "Ave: ";
static const int8_t dstString[] PROGMEM = "Dst: ";

static const int8_t bluetoothString[] PROGMEM = "Bluetooth: ";
static const int8_t bluetoothOnString[] PROGMEM = "On";
static const int8_t bluetoothOffString[] PROGMEM = "Off";

#endif /* GEEKSPORT_SYMBOLS_H_ */