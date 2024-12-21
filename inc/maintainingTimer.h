#ifndef MAINTAINING_TIMER_H_
#define MAINTAINING_TIMER_H_

// MAINTAINING_PERIOD - period for data collecting and peripheral modules switching
// TEMP_SETPOINT - if temperature below that value, heating lamp turns on
// TEMP_SETPOINT_DELTA - if temperature over TEMP_SETPOIT+TEMP_SETPOINT_DELTA, cooling fan turns on
// LIGHT SETPOINT - if photoresistor return value less, fitolamp turns on
// MOISTURE_SETPOINT  -if maisture sensor return value less, plant watering should be turned on

#include "common.h"

#define MAINTAINIG_PERIOD   600L  // sec
#define TEMP_SETPOINT       22.5F // oC
#define TEMP_SETPOINT_DELTA 3.5F  // oC
#define LIGHT_SETPOINT      0.6F  // %
#define MOISTURE_SETPOINT   0.4F  // %

void timerInit(void);
void climateMaining(void);

#endif