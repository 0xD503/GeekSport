#include "SPI.h"

int8_t SPI_TransmitBuffer[SPI_BUFFER_SIZE];										//	Buffers initialization
int8_t SPI_ReceiveBuffer[SPI_BUFFER_SIZE];										//	

volatile uint8_t SPI_CharacterCounter = 0;										//	Initializing the counter


#ifdef SPI_MASTER_MODE

volatile bool SPI_TransmitSuccess = true;										//	Initializing the success flag
//volatile uint8_t SPI_CharacterCounter;											//	Initializing the counter

void SPI_Master_Init (void)
{
	volatile uint8_t temp __attribute__((unused));
	
	SPI_DDR |= 1 << SPI_MOSI | 0 << SPI_MISO | 1 << SPI_SCK | 1 << SPI_SS_1;	//	SS pin as output
	SPI_DDR &= ~(1 <<SPI_MISO);
	//SPCR |= 1 << SPIE | 1 << SPE | 1 << MSTR | 0 << SPR1 | 1 << SPR0;			//	SPI Interrupt enabled
	SPCR0 |= 1 << SPE | 1 << MSTR | 0 << SPR1 | 0 << SPR0;
	temp = SPSR0;																//	Clear the interrupt flag
	temp = SPDR0;																//	Clear the interrupt flag
}

void SPI_PutByte (int8_t byte)
{
	SPDR0 = byte;
	while (!(SPSR0 & (1 << SPIF)));
}

int8_t SPI_GetByte (void)
{
	SPDR0 = 0xEE;							//	Any byte
	while (!(SPSR0 & (1 << SPIF)));
	
	return (SPDR0);
}

void SPI_SendData (int8_t* data)
{
	for (uint8_t i = 0; data[i] != 0; i++)	SPI_TransmitBuffer[i] = data[i];	//	Putting the data in the buffer
	SPI_CharacterCounter = 0;													//	Reset character counter
	SPI_TransmitSuccess = false;												//	Clear the success flag before sending
	SPCR0 |= (1 << SPIE);														//	Activate SPI interrupt
 	volatile uint8_t temp = SPSR0;												//	Clear the interrupt flag
 	temp = SPDR0;																//	Clear the interrupt flag
	temp = 0x00;																//	Here assign any "garbage" value to temp
	SPDR0 = temp;																//	and put it in the SPDR to activate the transfer process
}

ISR (SPI0_STC_vect)
{
//	SPI_ReceiveBuffer[SPI_CharacterCounter] = SPDR;								//	Receive byte from the Slave	(for full-duplex communication)
	if (SPI_TransmitBuffer[SPI_CharacterCounter] != 0)							//	If not end of data
	{
		SPDR0 = SPI_TransmitBuffer[SPI_CharacterCounter++];						//	Put the next byte in the SPI data register
	}
	else
	{
		SPCR0 &= ~(1 << SPIE);													//	Disable SPI interrupt
		SPI_TransmitSuccess = true;												//	Set the success flag
		SPI_CharacterCounter = 0;												//	Reset character counter
	}
}

#endif


#ifdef SPI_SLAVE_MODE						//	Slave functions

volatile bool SPI_ReceiveSuccess = true;													//	Initializing the success flag

void SPI_Slave_Init (void)
{
	volatile uint8_t temp;
	SPI_DDR |= 0 << SPI_MOSI | 1 << SPI_MISO | 0 << SPI_SCK | 0 << SPI_SS_1;				//	SS pin as input
	SPCR0 |= 1 << SPIE | 1 << SPE;															//	Enable SPI interrupt
	temp = SPSR0;	//	Clear the interrupt flag
	temp = SPDR0;	//
}

void SPI_ReceiveData(void)
{
	//	
}

ISR (SPI0_STC_vect)
{
	SPI_ReceiveBuffer[SPI_CharacterCounter] = SPDR0;											//	Receive byte from the Master
//	SPDR = SPI_TransmitBuffer[SPI_CharacterCounter];										//	(For full-duplex communication)
	if (SPI_ReceiveBuffer[SPI_CharacterCounter] != 0)	SPI_CharacterCounter++;				//	If not the end of data, increment the size of the buffer
	else	SPI_CharacterCounter = 0;														//	Reset character counter
}

#endif