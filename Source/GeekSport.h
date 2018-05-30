/*
 * GeekSport.h
 *
 * Created: 4/13/2018 7:33:32 PM
 *  Author: Developer
 */ 


#ifndef GEEKSPORT_H_
#define GEEKSPORT_H_

#include "main.h"
// 
// enum GEEKSPORT_MODES {GEEKSPORT_START, GEEKSPORT_WATCH_MODE, GEEKSPORT_SPEEDOMETER_MODE, GEEKSPORT_BLUETOOTH_SCREEN_MODE, GEEKSPORT_SETTINGS_MENU, GEEKSPORT_LAST_MODE, GEEKSPORT_SLEEP_MODE = 86};
// enum GEEKSPORT_SETTINGS_MODES {SETTINGS_INFO_MODE, SETTINGS_MODE};


void GeekSport_LCD_Nokia_UpdateDigitOnScreen (uint8_t xPos, uint8_t yPos);
void GeekSport_ChangeSettingDigit (void);

void GeekSport_Init (void);
void GeekSport_SettingsMenu (void);

void GeekSport_SetMode (uint8_t mode);

void Geeksport_GoToSleep (void);

#endif /* GEEKSPORT_H_ */