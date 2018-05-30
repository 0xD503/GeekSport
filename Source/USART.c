#include "USART.h"

int8_t USART_TransmitBuffer[USART_BUFFER_SIZE];			//	Initialize the Transmit and Receive buffers for data storage
static volatile uint8_t USART_TxHead;					//	And their pointers
static volatile uint8_t USART_TxTail;					//	
volatile int8_t USART_ReceiveBuffer[USART_BUFFER_SIZE];			//	
static volatile uint8_t USART_RxHead;					//	
static volatile uint8_t USART_RxTail;					//	

void USART_FlushBuffers (void)
{
	USART_TxHead = 0;
	USART_TxTail = 0;
	USART_RxHead = 0;
	USART_RxTail = 0;
}

void USART_Init (void)
{																//	
	UBRR0 = USART_UBRR;
	UCSR0B |= 1 << RXCIE0 | 1 << RXEN0 | 1 << TXEN0;									//	Turn on transceiver, receiver; enable Rx Complete Interrupt (when there are unread data in the receive buffer detected)
	UCSR0C |= 3 << UCSZ00;																//	Asynchronous operation, selected frame format - 8 bit, no parity bit, 1 stop bit;
	
	USART_FlushBuffers();																//	Reset buffers
}

void USART_PutByte (int8_t byte)
{
	uint8_t tempTxTail = (USART_TxTail + 1) & USART_BUFFER_MASK;						//	Creating temporary incremented tail (for interruptions)
	while(USART_TxHead == tempTxTail);													//	Wait for free space in the buffer
	USART_TxTail = tempTxTail;															//	Now update tail
	USART_TransmitBuffer[tempTxTail] = byte;											//	And put byte in the Transmit buffer, which will handle the interrupt
	
	UCSR0B |= 1 << UDRIE0;																//	Enable interrupt handler
}

int8_t USART_GetByte (void)
{
	while (USART_RxHead == USART_RxTail);												//	Waiting for reception
	USART_RxHead = (USART_RxHead + 1) & USART_BUFFER_MASK;								//	When the first byte received, increment ReceiverHead
	
	return (USART_ReceiveBuffer[USART_RxHead]);											//	Return byte by the received pointer
}

void USART_SendString (int8_t* data)
{
	for (uint8_t i = 0; data[i] != 0; i++)	USART_PutByte(data[i]);						//	Send by byte, until we detect NULL character (or the end of the line)
}

void USART_ReceiveString (int8_t* data)
{
	for (uint8_t i = 0; data[i] != 0; i++)	data[i] = USART_GetByte();					//	Read by byte, until we detect NULL character (or the end of the line)
}

ISR (USART0_UDRE_vect)
{
	if (USART_TxHead != USART_TxTail)													//	If there's a clean place in the buffer
	{
		uint8_t tempTxHead = (USART_TxHead + 1) & USART_BUFFER_MASK;					//	Increment head
		USART_TxHead = tempTxHead;
		UDR0 = USART_TransmitBuffer[tempTxHead];										//	And place in the USART Data Register byte
	}
	else	UCSR0B &= ~(1 << UDRIE0);													//	Else disable interrupt
}

ISR (USART0_RX_vect)
{
	int8_t byte = UDR0;																	//	Saving UDR value
	uint8_t tempRxTail = (USART_RxTail + 1) & USART_BUFFER_MASK;						//	Increment RxTail value
	USART_RxTail = tempRxTail;															//	Saving RxTail value
	if (USART_RxHead == tempRxTail)																	
	{
		//	Buffer overflow
		//
		
		//	PORTD |= 0x80;
	}
	USART_ReceiveBuffer[tempRxTail] = byte;												//	Saving UDR value in the receive buffer
}