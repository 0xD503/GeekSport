/*
 * DS1307_ProgramMemory.h
 *
 * Created: 4/9/2018 8:49:53 AM
 *  Author: Developer
 */ 


#ifndef DS1307_PROGRAMMEMORY_H_
#define DS1307_PROGRAMMEMORY_H_

#include <avr/pgmspace.h>


//	Weekdays in FLASH	//
static const int8_t Sunday[] PROGMEM = "Sun";
static const int8_t Monday[] PROGMEM = "Mon";
static const int8_t Tuesday[] PROGMEM = "Tue";
static const int8_t Wednesday[] PROGMEM = "Wed";
static const int8_t Thursday[] PROGMEM = "Thu";
static const int8_t Friday[] PROGMEM = "Fri";
static const int8_t Saturday[] PROGMEM = "Sat";

static const int8_t* const WeekdaysPointer[7] = {Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday};


//	Months	//
static const int8_t January[] PROGMEM = "Jan";
static const int8_t February[] PROGMEM = "Feb";
static const int8_t March[] PROGMEM = "Mar";
static const int8_t April[] PROGMEM = "Apr";
static const int8_t May[] PROGMEM = "May";
static const int8_t June[] PROGMEM = "June";
static const int8_t Jule[] PROGMEM = "Jule";
static const int8_t August[] PROGMEM = "Aug";
static const int8_t Septemper[] PROGMEM = "Sep";
static const int8_t October[] PROGMEM = "Oct";
static const int8_t November[] PROGMEM = "Nov";
static const int8_t December[] PROGMEM = "Dec";

static const int8_t* const MonthsPointer[12]/* PROGMEM*/ = {January, February, March, April, May, June, Jule, August, Septemper, October, November, December};


static const int8_t timeSettingString[] PROGMEM = "Time: 00:00";
static const int8_t dateSettingString[] PROGMEM = "Date: 00.00.00";
static const int8_t weekdaySettingString[] PROGMEM = "Weekday - ";

#endif /* DS1307_PROGRAMMEMORY_H_ */