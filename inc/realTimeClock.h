#ifndef REAL_TIME_CLOCK_H_
#define REAL_TIME_CLOCK_H_

// DS1307 serial real-time clock (RTC)
// https://static.chipdip.ru/lib/834/DOC003834620.pdf
// connected to SDA and SCL of atmega

#include "common.h"

#define CLOCK_TWI_ADDRESS 0x68

#define RTC_SEC     0
#define RTC_MIN     1
#define RTC_HOUR    2
#define RTC_WEEKDAY 3

void rtcInit(void);
void rtcSetTime(uint8_t hour, uint8_t min, uint8_t sec, uint8_t weekday);
uint8_t rtcGetTimeWeekday(uint8_t hmswd);

#endif