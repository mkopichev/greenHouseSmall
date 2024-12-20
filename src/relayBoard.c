#include "../inc/relayBoard.h"

void relayBoardInit(void) {

    DDRD |= (1 << 7); // heating lamp
    DDRB |= (1 << 0); // fitolamp
    DDRD |= (1 << 4); // cooling fan
    relayBoard(HEATING_LAMP, TURN_OFF);
    relayBoard(FITO_LAMP, TURN_OFF);
    relayBoard(COOLING_FAN, TURN_OFF);
}

void relayBoard(uint8_t device, bool state) {

    switch(device) {
    case HEATING_LAMP:
        if(state) {

            PORTD |= (1 << 7);
        } else {

            PORTD &= ~(1 << 7);
        }
        break;
    case FITO_LAMP:
        if(state) {

            PORTB |= (1 << 0);
        } else {

            PORTB &= ~(1 << 0);
        }
        break;
    case COOLING_FAN:
        if(state) {

            PORTD |= (1 << 4);
        } else {

            PORTD &= ~(1 << 4);
        }
        break;
    default:
        break;
    }
}