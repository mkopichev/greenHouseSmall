#ifndef ANALOG_LIGHT_MOISTURE_H_
#define ANALOG_LIGHT_MOISTURE_H_

// read ADC channels of MC and return boolean value - either parameter is ok or not

#include "common.h"

#define LIGHT_CHANNEL_1  1
#define LIGHT_CHANNEL_2  2
#define MOISTURE_CHANNEL 3

void adcInit(void);
bool getAnalogLight(void);
bool getAnalogMoisture(void);

#endif