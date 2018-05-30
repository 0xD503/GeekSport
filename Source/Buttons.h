/*
 * Buttons.h
 *
 * Created: 4/9/2018 3:26:31 PM
 *  Author: Developer
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "main.h"

#define BUTTONS_DDR_1 DDRC
#define BUTTONS_PORT_1 PORTC
#define BUTTONS_PINS_1 PINC
#define BUTTONS_DDR_1_BUTTON_1_PIN 2
#define BUTTONS_DDR_1_BUTTON_2_PIN 3

#define BUTTONS_DDR_2 DDRD
#define BUTTONS_PORT_2 PORTD
#define BUTTONS_PINS_2 PIND
#define BUTTONS_DDR_2_BUTTON_1_PIN 4
#define BUTTONS_DDR_2_BUTTON_2_PIN 7

#define BUTTONS_DELAY_US 15256
#define BUTTONS_PRESCALER 256

volatile bool Buttons_SetButtonState;
volatile bool Buttons_SetPressed;

void Buttons_Init (void);

#endif /* BUTTONS_H_ */