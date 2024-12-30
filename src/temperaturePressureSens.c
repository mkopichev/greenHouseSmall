#include "../inc/temperaturePressureSens.h"

int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
uint16_t ac4, ac5, ac6;

int32_t x1, x2, x3, b3, b5, b6, b7, p;
uint32_t b4;

int32_t rawTemp, rawPressure;

void switchEndian16(uint16_t *ptr) {
    *ptr = ((*ptr & 0xFF00) >> 8) | ((*ptr & 0xFF) << 8);
}

void switchEndian24(uint32_t *ptr) {
    *ptr = ((*ptr & 0xFF0000) >> 16) | ((*ptr & 0xFF) << 16);
}


void tempSensReadCalibrationData(void) {

    twiReadMultipleData(TEMP_SENS_TWI_ADDRESS, 0xAA, &ac1, 2);
    twiReadMultipleData(TEMP_SENS_TWI_ADDRESS, 0xAC, &ac2, 2);
    twiReadMultipleData(TEMP_SENS_TWI_ADDRESS, 0xAE, &ac3, 2);
    twiReadMultipleData(TEMP_SENS_TWI_ADDRESS, 0xB0, &ac4, 2);
    twiReadMultipleData(TEMP_SENS_TWI_ADDRESS, 0xB2, &ac5, 2);
    twiReadMultipleData(TEMP_SENS_TWI_ADDRESS, 0xB4, &ac6, 2);
    twiReadMultipleData(TEMP_SENS_TWI_ADDRESS, 0xB6, &b1, 2);
    twiReadMultipleData(TEMP_SENS_TWI_ADDRESS, 0xB8, &b2, 2);
    twiReadMultipleData(TEMP_SENS_TWI_ADDRESS, 0xBA, &mb, 2);
    twiReadMultipleData(TEMP_SENS_TWI_ADDRESS, 0xBC, &mc, 2);
    twiReadMultipleData(TEMP_SENS_TWI_ADDRESS, 0xBE, &md, 2);
    switchEndian16(&ac1);
    switchEndian16(&ac2);
    switchEndian16(&ac3);
    switchEndian16(&ac4);
    switchEndian16(&ac5);
    switchEndian16(&ac6);
    switchEndian16(&b1);
    switchEndian16(&b2);
    switchEndian16(&mb);
    switchEndian16(&mc);
    switchEndian16(&md);
}

void tempSensReadRawTemp(void) {

    uint8_t tmp = 0x2E;
    twiWriteMultipleData(TEMP_SENS_TWI_ADDRESS, 0xF4, &tmp, 1);
    _delay_ms(4.5);
    twiReadMultipleData(TEMP_SENS_TWI_ADDRESS, 0xF6, &rawTemp, 2);
    switchEndian16(&rawTemp);
}

void tempSensReadRawPressure(void) {

    uint8_t tmp = 0x34 + (TEMP_SENS_OSS << 6);
    twiWriteMultipleData(TEMP_SENS_TWI_ADDRESS, 0xF4, &tmp, 1);
    _delay_ms(7.5);
    twiReadMultipleData(TEMP_SENS_TWI_ADDRESS, 0xF6, &rawPressure, 3);
    switchEndian24(&rawPressure);
}

float tempSensReadTemp(void) {

    tempSensReadRawTemp();
    x1 = ((rawTemp - ac6) * ac5) >> 15;
    x2 = (mc * pow(2, 11)) / (x1 + md);
    b5 = x1 + x2;
    return ((b5 + 8) / pow(2, 4)) / 10;
}

float tempSensReadPressure(void) {

    tempSensReadRawPressure();
    b6 = b5 - 4000;
    x1 = b2 * (b6 * b6 / pow(2, 12)) / pow(2, 11);
    x2 = ac2 * b6 / pow(2, 11);
    x3 = x1 + x2;
    b3 = ((((ac1 * 4) + x3) << TEMP_SENS_OSS) + 2) / 4;
    x1 = ac3 * b6 / pow(2, 13);
    x2 = (b1 * (b6 * b6 / pow(2, 12))) / pow(2, 16);
    x3 = ((x1 + x2) + 2) / pow(2, 2);
    b4 = ac4 * (uint32_t)(x3 + 32768) / pow(2, 15);
    b7 = ((uint32_t)(rawPressure >> (8 - TEMP_SENS_OSS)) - b3) * (50000 >> TEMP_SENS_OSS);
    if((uint32_t)b7 < 0x80000000) {

        p = (b7 * 2) / b4;
    } else {

        p = (b7 / b4) * 2;
    }
    x1 = (p / pow(2, 8)) * (p / pow(2, 8));
    x1 = (x1 * 3038) / pow(2, 16);
    x2 = (-7357 * p) / pow(2, 16);
    return (p + (x1 + x2 + 3791) / pow(2, 4)) / 133.3F;
}

void tempSensInit(void) {

    tempSensReadCalibrationData();
    uartTransmitStr("tempSensInit_ok\r\n");
}