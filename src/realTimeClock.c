#include "../inc/realTimeClock.h"

void rtcInit(void) {

    uint8_t tmp = 0x00;
    twiWriteMultipleData(CLOCK_TWI_ADDRESS, 0x07, &tmp, 1);
    uartTransmitStr("rtcInit_ok\r\n");
}

void rtcSetTimeDate(uint8_t hour, uint8_t min, uint8_t sec, uint8_t weekday, uint8_t date, uint8_t month, uint8_t year) {

    uint8_t tmp[] = {(((sec / 10) << 4) | (sec % 10)), (((min / 10) << 4) | (min % 10)), (((hour / 10) << 4) | (hour % 10)),
                     weekday,
                     (((date / 10) << 4) | (date % 10)), (((month / 10) << 4) | (month % 10)), year};
    twiWriteMultipleData(CLOCK_TWI_ADDRESS, 0x00, &tmp, 7);
    uartTransmitStr("rtcSetTime_ok\r\n");
}

uint8_t rtcConvertValue(uint8_t value) {

    return ((value >> 4) * 10 + (value & 0x0F));
}

uint8_t tmp[] = {0, 0, 0, 0, 0, 0, 0};

int8_t rtcGetData(uint8_t hmswddmy) {

    switch(hmswddmy) {
    case RTC_SEC:
        return (rtcConvertValue(tmp[RTC_SEC]));
        break;
    case RTC_MIN:
        return (rtcConvertValue(tmp[RTC_MIN]));
        break;
    case RTC_HOUR:
        return (rtcConvertValue(tmp[RTC_HOUR]));
        break;
    case RTC_WEEKDAY:
        return (tmp[RTC_WEEKDAY]);
        break;
    case RTC_DATE:
        return (rtcConvertValue(tmp[RTC_DATE]));
        break;
    case RTC_MONTH:
        return (rtcConvertValue(tmp[RTC_MONTH]));
        break;
    case RTC_YEAR:
        return (tmp[RTC_YEAR]);
        break;
    default:
        return -1;
    }
}

void rtcGetTimeDate() {

    uartTransmitStr("inside rtcGetTimeDate\r\n");
    twiReadMultipleData(CLOCK_TWI_ADDRESS, 0x00, &tmp, 7);
    uartTransmitStr("rtcGetTimeDate function finished\r\n");
}