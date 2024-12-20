#ifndef RELAY_BOARD_H_
#define RELAY_BOARD_H_

// relay board controls both heating (~220V) and fito (~220V) lamps and cooling fan (=12V)

#include "common.h"

#define TURN_ON      false
#define TURN_OFF     true
#define HEATING_LAMP 0
#define FITO_LAMP    1
#define COOLING_FAN  2

void relayBoardInit(void);
void relayBoard(uint8_t device, bool state);

#endif