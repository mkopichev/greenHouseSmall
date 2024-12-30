#include "../inc/analogLightMoisture.h"

uint16_t analogDataLight1 = 0, analogDataLight2 = 0, analogDataMoisture = 0;

void adcInit(void) {

    ADMUX = (1 << REFS0) | LIGHT_CHANNEL_1;
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    uartTransmitStr("adcInit_ok\r\n");
}

ISR(ADC_vect) {

    if((ADMUX & 0x0F) == LIGHT_CHANNEL_1) {

        analogDataLight1 = ADCL | (ADCH << 8);
        ADMUX = (1 << REFS0) | LIGHT_CHANNEL_2;
    } else if((ADMUX & 0x0F) == LIGHT_CHANNEL_2) {

        analogDataLight2 = ADCL | (ADCH << 8);
        ADMUX = (1 << REFS0) | MOISTURE_CHANNEL;
    } else {

        analogDataMoisture = ADCL | (ADCH << 8);
        ADMUX = (1 << REFS0) | LIGHT_CHANNEL_1;
    }
    ADCSRA |= (1 << ADSC);
}

bool getAnalogLight(void) {

    uint16_t illumination = 1023 - ((analogDataLight1 + analogDataLight2) / 2);

    if(illumination >= (((1 << 10) - 1) * LIGHT_SETPOINT)) {

        return false;
    } else {

        return true;
    }
}

bool getAnalogMoisture(void) {

    uint16_t soilMoisture = 1023 - analogDataMoisture;

    if(soilMoisture >= (((1 << 10) - 1) * MOISTURE_SETPOINT)) {

        return false;
    } else {

        return true;
    }
}