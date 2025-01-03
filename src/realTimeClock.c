#include "../inc/realTimeClock.h"

void rtcInit(void) {

    uint8_t tmp = 0x00;
    twiWriteMultipleData(CLOCK_TWI_ADDRESS, 0x07, &tmp, 1);
    uartTransmitStr("rtcInit_ok\r\n");
}

void rtcSetTime(uint8_t hour, uint8_t min, uint8_t sec, uint8_t weekday) {

    uint8_t tmp[] = {(((sec / 10) << 4) | (sec % 10)), (((min / 10) << 4) | (min % 10)), (((hour / 10) << 4) | (hour % 10)), weekday};
    twiWriteMultipleData(CLOCK_TWI_ADDRESS, 0x00, &tmp, 4);
    uartTransmitStr("rtcSetTime_ok\r\n");
}

uint8_t rtcConvertTime(uint8_t value) {

    return ((value >> 4) * 10 + (value & 0x0F));
}

uint8_t tmp[] = {0, 0, 0, 0};

int8_t rtcGetData(uint8_t hmswd) {
switch(hmswd) {
    case RTC_SEC:
        return (rtcConvertTime(tmp[0]));
        break;
    case RTC_MIN:
        return (rtcConvertTime(tmp[1]));
        break;
    case RTC_HOUR:
        return (rtcConvertTime(tmp[2]));
        break;
    case RTC_WEEKDAY:
        return (tmp[3]);
        break;
    default:
        return -1;
    }

}

void rtcGetTimeWeekday() {

    uartTransmitStr("inside rtcGetTimeWeekday\r\n");

    twiReadMultipleData(CLOCK_TWI_ADDRESS, 0x00, &tmp, 4);

    uartTransmitStr("data read from rtc\r\n");
}