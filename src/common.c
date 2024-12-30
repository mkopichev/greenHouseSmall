#include "../inc/common.h"

extern char tmpStr[18], tmpStrWeekDay[4];
extern bool dayTime;

#define CLOCK_SET_TIME_DATE_NO

uint8_t digit(uint16_t d, uint8_t m) {

    uint8_t i = 5, a;
    while(i) {      // цикл по разрядам числа
        a = d % 10; // выделяем очередной разряд
        // выделен заданный разряд - уходим
        if(i-- == m)
            break;
        d /= 10; // уменьшаем число в 10 раз
    }
    return (a);
}

void initAll(void) {

    serviceLedInit();
    uartInit();
    twiInit();
    lcdInit();
    tempSensInit();
    adcInit();
    rtcInit();
#ifdef CLOCK_SET_TIME_DATE
    rtcSetTime(19, 38, 20, 1);
#endif
    relayBoardInit();
    waterPumpInit();
    timerInit();
    _delay_ms(100);
    sei();
    climateMaining();
    
    lcdSetCursor(0);
    if(dayTime) {

        snprintf(tmpStr, sizeof(tmpStr), "%d.%d oC; ill:%s", (uint8_t)tempSensReadTemp(), (uint16_t)(tempSensReadTemp() * 10) % 10, (getAnalogLight() ? "low" : " ok"));
    } else {

        snprintf(tmpStr, sizeof(tmpStr), "%d.%d oC; ill:%s", (uint8_t)tempSensReadTemp(), (uint16_t)(tempSensReadTemp() * 10) % 10, "off");
    }
    uartTransmitStr(tmpStr);
    uartTransmitStr("\r\n");
    lcdSendStr(tmpStr);

    rtcGetTimeWeekday();

    switch(rtcGetData(RTC_WEEKDAY)) {
    case 1:
        snprintf(tmpStrWeekDay, sizeof(tmpStrWeekDay), "%s", "Mon");
        break;
    case 2:
        snprintf(tmpStrWeekDay, sizeof(tmpStrWeekDay), "%s", "Tue");
        break;
    case 3:
        snprintf(tmpStrWeekDay, sizeof(tmpStrWeekDay), "%s", "Wed");
        break;
    case 4:
        snprintf(tmpStrWeekDay, sizeof(tmpStrWeekDay), "%s", "Thu");
        break;
    case 5:
        snprintf(tmpStrWeekDay, sizeof(tmpStrWeekDay), "%s", "Fri");
        break;
    case 6:
        snprintf(tmpStrWeekDay, sizeof(tmpStrWeekDay), "%s", "Sat");
        break;
    case 7:
        snprintf(tmpStrWeekDay, sizeof(tmpStrWeekDay), "%s", "Sun");
        break;
    default:
        break;
    }
}