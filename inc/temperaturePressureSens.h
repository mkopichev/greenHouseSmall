#ifndef TEMPERATURE_PRESSURE_SENS_H_
#define TEMPERATURE_PRESSURE_SENS_H_

// BMP180 digital temperature and pressure sensor
// https://www.micro-semiconductor.com/datasheet/c9-BMP180.pdf
// connected to SDA and SCL of atmega

#include "common.h"

#define TEMP_SENS_TWI_ADDRESS 0x77
#define TEMP_SENS_OSS         1

void tempSensInit(void);
float tempSensReadTemp(void);
float tempSensReadPressure(void);

#endif