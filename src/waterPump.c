#include "../inc/waterPump.h"

void waterPumpInit(void) {

    DDRB |= (1 << 1);
    TCCR1A |= (1 << COM1A1) | (1 << WGM10);
    TCCR1B |= (1 << WGM12) | (1 << CS12);
}

void waterPumpPour(void) {

    while(getAnalogMoisture())

        OCR1A = 127;

    OCR1A = 0;
}