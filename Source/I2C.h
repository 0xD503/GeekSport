/*
 * I2C.h
 *
 * Created: 3/21/2018 4:15:41 PM
 *  Author: Developer
 */ 


#ifndef I2C_H_
#define I2C_H_

#include "main.h"

#define I2C_SCL_FREQUENCY 100000											// SCL frequency value
#define I2C_PRESCALER 1														// Prescaler value
#define I2C_PRESCALER_BITS 0x00
#define TWBR_VALUE ((F_CPU / I2C_SCL_FREQUENCY) - 16) / (2 * PRESCALER);	// Calculating TWBR value

#define I2C_STATUS_MASK 0xF8												// Set I2C status code mask

#define I2C_DDR DDRC
#define I2C_PORT PORTC
#define I2C_PINS PINC
#define I2C_SCL_PIN 5
#define I2C_SDA_PIN 4

#define I2C_TRANSMISSION_BUFFER_SIZE 16										// Set this to the largest message size that will be sent including address byte.
#define I2C_RECEPTION_BUFFER_SIZE 16

#define I2C_READ	0x01
#define I2C_WRITE	0x00

#define I2C_READ_BIT  0														//	Bit position for R/W bit in "address byte".
#define I2C_ADR_BITS  1														//	Bit position for LSB of the slave address bits in the initial byte.

#define I2C_GEN_CALL         0x00											//	The General Call address is 0

#define I2C_BUS_ERROR 0x00													//	Bus error due to an illegal START or STOP condition

#define I2C_START 0x08														//	START has been transmitted
#define I2C_REPEATED_START 0x10												//	Repeated START has been transmitted

// I2C Master Transmitter status codes
#define MT_SLA_ACK 0x18														//	SLA+W has been transmitted and ACK received
#define MT_SLA_NACK 0x20													//	SLA+W has been transmitted and NACK received
#define MT_DATA_ACK 0x28													//	Data byte has been transmitted and ACK received
#define MT_DATA_NACK 0x30													//	Data byte has been transmitted and NACK received

#define MT_MR_ARBITRATION_LOST 0x38											//	Arbitration lost

// I2C Master Receiver status codes
#define MR_SLA_ACK 0x40														// SLA+R has been transmitted and ACK received
#define MR_SLA_NACK 0x48													// SLA+R has been transmitted and NACK received
#define MR_DATA_ACK 0x50													// Data byte has been received and ACK transmitted
#define MR_DATA_NACK 0x58													// Data byte has been received and NACK transmitted

#define ST_ARBITRATION_LOST_SLA_ACK 0xB0

// I2C Slave Transmitter status codes
#define ST_SLA_ACK 0xA8														// Own SLA+R has been received; ACK has been returned
#define ST_DATA_ACK 0xB8													// Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
#define ST_DATA_NACK 0xC0													// Data byte in TWDR has been transmitted; NOT ACK has been received
#define ST_LAST_BYTE_ACK 0xC8												// Last data byte in TWDR has been transmitted (TWEA = ?0?); ACK has been received

#define SR_ARBITRATION_LOST_SLA_ACK 0x68									// Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned

// I2C Slave Receiver status codes
#define SR_SLA_ACK 0x60														// Own SLA+W has been received ACK has been returned
#define SR_GENERAL_CALL_ACK 0x70											// General call address has been received; ACK has been returned
#define SR_ARBITRATION_LOST_GENERAL_CALL_ACK 0x78							// Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
#define SR_PREVIOUSLY_ADDRESSEDD_DATA_ACK 0x80								// Previously addressed with own SLA+W; data has been received; ACK has been returned
#define SR_PREVIOUSLY_ADDRESSEDD_DATA_NACK 0x88								// Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
#define SR_PREVIOUSLY_ADDRESSEDD_GENERAL_CALL_DATA_ACK 0x90					// Previously addressed with general call; data has been received; ACK has been returned
#define SR_PREVIOUSLY_ADDRESSEDD_GENERAL_CALL_DATA_NACK 0x98				// Previously addressed with general call; data has been received; NOT ACK has been returned
#define SR_STOP_OR_REPEATED_START 0xA0										// A STOP condition or repeated START condition has been received while still addressed as Slave

// I2C Miscellaneous status codes
#define I2C_NO_RELEVANT_STATE_INFORMATION 0xF8

#define I2C_SUCCESS 0xFF
#define I2C_FAIL 0x00

#define I2C_SUCCESS_FLAG	1
#define I2C_FAIL_FLAG		0

union I2C_StatusFlags
{
	uint8_t all;
	
	struct
	{
		uint8_t transfer:1;
		uint8_t reception:1;
		uint8_t unusedBits:6;
	};
};

union I2C_StatusFlags I2C_StatusFlags;


void I2C_Init (uint32_t clockFrequency, uint8_t prescaler);

void I2C_MT_TransmitData (int8_t* messageToTransmit, uint8_t messageSize);
uint8_t I2C_MR_ReceiveData (int8_t* placeToReceive, uint8_t messageSize);
//uint8_t I2C_MR_ReceiveTimeStructData (struct DS1307_TimeRegisters* timeStructPointer);

#endif /* I2C_H_ */