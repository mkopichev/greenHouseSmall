#ifndef TWI_H_
#define TWI_H_

// low level header for TWI interaction
// able to generate START and STOP, transmit and receive one byte and transmit and receive multiple bytes
// TWI MUTEX also ready to use

#include "common.h"

#define W 0
#define R 1

#define SCL_FREQ 100L // kHz

#define VACANT   0
#define OCCUPIED 1

void twiInit(void);
void twiStart(void);
void twiStop(void);
void twiTransmitByte(uint8_t data);
uint8_t twiReceiveByte(bool isLastByte);
void twiReadMultipleData(uint8_t devAddr, uint8_t regAddr, void *rxBuf, uint8_t length);
void twiWriteMultipleData(uint8_t devAddr, uint8_t regAddr, void *txBuf, uint8_t length);

void twiAcquire(void);
void twiRelease(void);

#endif