#include "../inc/twi.h"

bool twiMutex = VACANT;

void twiInit(void) {

    TWBR = (uint8_t)(((F_CPU / (SCL_FREQ * 1000)) - 16) / 2);
    int16_t sclFreq = F_CPU / (16 + (2 * TWBR));
    uartTransmitStr("twiInit_ok; SCL freq = ");
    uartTransmitDec(sclFreq);
    uartTransmitStr("\r\n");
}

void twiStart(void) {

    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    uint32_t timeout = 0;
    while(!(TWCR & (1 << TWINT))) {

        if(timeout++ > (F_CPU / 16UL)) {

            uartTransmitStr("twiStart timeout\r\n");
            maintainingPeriodCounter--;
            wdt_reset();
            break;
        } else {

            continue;
        }
    }
}

void twiStop(void) {

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void twiTransmitByte(uint8_t data) {

    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    uint32_t timeout = 0;
    while(!(TWCR & (1 << TWINT))) {

        if(timeout++ > (F_CPU / 16UL)) {

            uartTransmitStr("twiTransmitByte timeout\r\n");
            maintainingPeriodCounter--;
            wdt_reset();
            break;
        } else {

            continue;
        }
    }
}

uint8_t twiReceiveByte(bool isLastByte) {

    if(isLastByte) {

        TWCR = (1 << TWINT) | (1 << TWEN);
    } else {

        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    }
    uint32_t timeout = 0;
    while(!(TWCR & (1 << TWINT))) {

        if(timeout++ > (F_CPU / 16UL)) {

            uartTransmitStr("twiReceiveByte timeout\r\n");
            maintainingPeriodCounter--;
            wdt_reset();
            return 0;
        } else {

            continue;
        }
    }

    return TWDR;
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

    twiStart();
    twiTransmitByte((devAddr << 1) | W);
    twiTransmitByte(regAddr);
    twiStart();
    twiTransmitByte((devAddr << 1) | R);
    for(uint8_t i = 0; i < (length - 1); i++) {

        ((uint8_t *)rxBuf)[i] = twiReceiveByte(false);
    }
    ((uint8_t *)rxBuf)[length - 1] = twiReceiveByte(true);
    twiStop();
}

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