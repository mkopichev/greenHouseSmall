#ifndef COMMON_H_
#define COMMON_H_

// Once in MAINTAINING_PERIOD collect data from temperature and pressure sensor, photoresistors and moisture sensor.
// According to the received info either turn on or off heating lamp, cooling fan, fito lamp and water pump.
// Once in five sec refresh temperature, pressure, illumination and moisture info on the LCD.
// one in a sec refresh time stamp on the screen.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "serviceLed.h"
#include "twi.h"
#include "uart.h"
#include "lcd.h"
#include "twiTemperaturePressure.h"
#include "analogLightMoisture.h"
#include "realTimeClock.h"
#include "relayBoard.h"
#include "waterPump.h"
#include "maintainingTimer.h"

uint8_t digit(uint16_t d, uint8_t m);
void initAll(void);

#endif