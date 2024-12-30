#include "../inc/twi.h"

uint8_t twiMutex = VACANT;

void twiInit(void) {

    TWBR = (uint8_t)(((F_CPU / (SCL_FREQ * 1000)) - 16) / 2);
    uartTransmitStr("twiInit_ok\r\n");
}

void twiStart(void) {

    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT)))
        continue;
}

void twiStop(void) {

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void twiTransmitByte(uint8_t data) {

    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT)))
        continue;
}

uint8_t twiReceiveByte(bool isLastByte) {

    if(isLastByte)
        TWCR = (1 << TWINT) | (1 << TWEN);
    else
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while(!(TWCR & (1 << TWINT)))
        continue;
    return (TWDR);
}

void twiWriteMultipleData(uint8_t devAddr, uint8_t regAddr, void *txBuf, uint8_t length) {

    twiStart();
    twiTransmitByte((devAddr << 1) | W);
    twiTransmitByte(regAddr);
    for(uint8_t i = 0; i < length; i++) {

        twiTransmitByte(((uint8_t *)txBuf)[i]);
    }
    twiStop();
}

void twiReadMultipleData(uint8_t devAddr, uint8_t regAddr, void *rxBuf, uint8_t length) {

    // uint8_t tmp[length];
    // *rxBuf = 0; // without this rxBuf collection works incorrect
    twiStart();
    twiTransmitByte((devAddr << 1) | W);
    twiTransmitByte(regAddr);
    twiStart();
    twiTransmitByte((devAddr << 1) | R);
    for(uint8_t i = 0; i < length; i++) {
        if(i != (length - 1)) {

            ((uint8_t *)rxBuf)[i] = twiReceiveByte(false);
        } else {

            ((uint8_t *)rxBuf)[i] = twiReceiveByte(true);
        }
    }
    // for(uint8_t i = 0; i < length; i++) {

    //     if(i != (length - 1)) {

    //         tmp[i] = twiReceiveByte(false);
    //     } else {

    //         tmp[i] = twiReceiveByte(true);
    //     }
    //     *rxBuf |= (tmp[i] << (8 * ((length - 1) - i)));
    // }
    twiStop();
}

// void twiWriteData(uint8_t *tx_buf, uint8_t length) {

//     for(uint8_t i = 0; i < length; i++)
//         twiTransmitByte(*tx_buf++);
// }

// void twiReadData(uint8_t *rx_buf, uint8_t length) {

//     for(uint8_t i = 0; i < length; i++) {
//         if(i < (length - 1))
//             *rx_buf++ = twiReceiveByte(false);
//         else
//             *rx_buf++ = twiReceiveByte(true);
//     }
// }

void twiAcquire(void) {

    while(1) {
        if(twiMutex == VACANT) {
            twiMutex = OCCUPIED;
            break;
        }
    }
}

void twiRelease(void) {

    twiMutex = VACANT;
}