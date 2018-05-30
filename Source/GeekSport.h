/*
 * GeekSport.h
 *
 * Created: 4/13/2018 7:33:32 PM
 *  Author: Developer
 */ 


#ifndef GEEKSPORT_H_
#define GEEKSPORT_H_

#include "main.h"

enum GEEKSPORT_MODES {GEEKSPORT_START, GEEKSPORT_WATCH_MODE, GEEKSPORT_SPEEDOMETER_MODE, GEEKSPORT_BLUETOOTH_SCREEN_MODE, GEEKSPORT_SETTINGS_MENU, GEEKSPORT_LAST_MODE};
enum GEEKSPORT_SETTINGS_MODES {SETTINGS_INFO_MODE, SETTINGS_MODE};

volatile uint8_t GeekSport_Mode;
volatile uint16_t GeekSport_SettingsDigit;
volatile uint8_t Settings_Mode;

void GeekSport_LCD_Nokia_UpdateScreen (uint8_t xPos, uint8_t yPos);
void GeekSport_ChangeSettingDigit (void);

void GeekSport_SettingsMenu (void);

void GeekSport_SetMode (uint8_t mode);

#endif /* GEEKSPORT_H_ */