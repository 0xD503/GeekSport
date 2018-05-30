/*
 * SPI.h
 *
 * Created: 12/30/2017 10:04:34 AM
 *  Author: Developer
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdbool.h>

#include "main.h"

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_PINS PINB

#define SPI_SS_1 2
#define SPI_MOSI 3
#define SPI_MISO 4
#define SPI_SCK 5

#define SPI_SELECT_SLAVE_1 SPI_PORT &= ~(1 << SPI_SS_1)
#define SPI_UNSELECT_SLAVE_1 SPI_PORT |= 1 << SPI_SS_1

#define SPI_ACK 0xAA			//	This is acknowledge code, can be arbitrary

#define SPI_BUFFER_SIZE 64		//	

extern int8_t SPI_TransmitBuffer[SPI_BUFFER_SIZE];
extern int8_t SPI_ReceiveBuffer[SPI_BUFFER_SIZE];

#ifdef SPI_MASTER_MODE			// Master functions

void SPI_Master_Init (void);

void SPI_PutByte (int8_t byte);
int8_t SPI_GetByte (void);

void SPI_SendData (int8_t* data);

#endif							//


#ifdef SPI_SLAVE_MODE			// Slave functions

void SPI_Slave_Init (void);

void SPI_ReceiveData(void);

#endif							//


#endif /* SPI_H_ */