/*
 * Bluetooth_Functions.h
 *
 * Created: 5/8/2018 9:23:31 AM
 *  Author: Developer
 *
 *	Description:
 *	##	Connecting:			VCC - 5V;
 **							GND - GND;
 **							TX (MCU) - to Bluetooth RX through voltage divider (3.3V);
 **							RX (MCU) - to Bluetooth TX;
 **							EN (ENABLE) - 3.3V.
 *
 *	##	UART Settings:		BAUDRATE - 9600;
 **							Frame format: 8 bit, no parity check, 1 stop bit;
 *
 *	## Other:				In terminal mode need to use <CR + NL> sequence after every message;
 */


#ifndef BLUETOOTH_FUNCTIONS_H_
#define BLUETOOTH_FUNCTIONS_H_

#include "main.h"

#define BLUETOOTH_BUFFER_SIZE 64

volatile uint8_t Bluetooth_Mode;
volatile bool Bluetooth_State;

int8_t Bluetooth_TransmitBuffer[BLUETOOTH_BUFFER_SIZE];
int8_t Bluetooth_ReceiveBuffer[BLUETOOTH_BUFFER_SIZE];

enum BLUETOOTH_COMMANDS_SET {BLUETOOTH_CHECK_CONNECTION = 0x30, BLUETOOTH_GET_MESSAGE};

enum BLUETOOTH_MODES {BLUETOOTH_VIEW_MESSAGE_MODE, BLUETOOTH_ON_OFF_COMMAND_MODE, BLUETOOTH_LAST_MODE};


void Bluetooth_On (void);

void Bluetooth_GetMessage (void);
void BLuetooth_LCD_Nokia_Main (void);

#endif /* BLUETOOTH_FUNCTIONS_H_ */

/*
Command              Description

----------------------------------------------------------------

AT                   Check if the command terminal work normally

AT+RESET             Software reboot

AT+VERSION           Get firmware, bluetooth, HCI and LMP version

AT+HELP              List all the commands

AT+NAME              Get/Set local device name

AT+PIN               Get/Set pin code for pairing

AT+BAUD              Get/Set baud rate

AT+CLEAR             Remove the remembered remote address

AT+LADDR             Get local bluetooth address

AT+DEFAULT           Restore factory default

AT+COD               Get/Set local class of device

AT+IAC               Get/Set inquiry access code

AT+ROLE              Get/Set master or slave mode

AT+STATE             Get current state

AT+UARTMODE          Get/Set uart stop bits and parity

AT+ENABLEIND         Enable/Disable Indication print

AT+LSP               List Paired Device List

AT+RESETPDL          Reset Paired Device List

AT+REMOVEPDL         Remove one entry from Paired Device List

-----------------------------------------------------------------

Note: (M) = The command support master mode only, other commands

support both master and slave mode.

For more information, please visit http://www.bolutek.cn

Copyright@2012 www.bolutek.cn. All rights reserved.
*/