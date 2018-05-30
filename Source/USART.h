/*
 * USART.h
 *
 * Created: 12/15/2017 3:21:42 PM
 *  Author: Developer
 *
 *	Description:
 *
 *	How to use:
 *	
 *	USART_RXC Interrupt handler:	
 */ 


#ifndef USART_H_
#define USART_H_

#include "main.h"

#define USART_BAUD 9600UL
#define USART_UBRR ((F_CPU / (16 * USART_BAUD)) - 1)

#define USART_BUFFER_SIZE 64									//	Must be a multiple of a degree two (2)
#define USART_BUFFER_MASK (USART_BUFFER_SIZE - 1)

extern int8_t USART_TransmitBuffer[USART_BUFFER_SIZE];
extern volatile int8_t USART_ReceiveBuffer[USART_BUFFER_SIZE];

void USART_Init (void);

void USART_PutByte (int8_t byte);
int8_t USART_GetByte (void);

void USART_SendString (int8_t* data);
void USART_ReceiveString (int8_t* data);

#endif /* USART_H_ */