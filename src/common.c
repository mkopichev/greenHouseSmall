#include "../inc/common.h"

// #define CLOCK_SET_TIME_DATE

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
    rtcSetTime(18, 8, 0, 6);
#endif
    relayBoardInit();
    waterPumpInit();
    timerInit();
    // watchdogInit();
    _delay_ms(100);
    sei();
    climateMaining();
}