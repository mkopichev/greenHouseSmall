#include "../inc/waterPump.h"

void waterPumpInit(void) {

    DDRB |= (1 << 1);
    TCCR1A |= (1 << COM1A1) | (1 << WGM10);
    TCCR1B |= (1 << WGM12) | (1 << CS12);
    void waterPumpStop();
    uartTransmitStr("waterPumpInit_ok\r\n");
}

void waterPumpPour(void) {

    OCR1A = (uint8_t)(255 * 0.4);
    uartTransmitStr("pouring started\r\n");
}

void waterPumpStop(void) {

    OCR1A = 0;
    uartTransmitStr("pouring stopped\r\n");
}