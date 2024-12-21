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
    uartTransmitStr("serviceLedInit_ok\r\n");
    uartInit();
    uartTransmitStr("uartInit_ok\r\n");
    twiInit();
    uartTransmitStr("twiInit_ok\r\n");
    lcdInit();
    uartTransmitStr("lcdInit_ok\r\n");
    tempSensInit();
    uartTransmitStr("tempSensInit_ok\r\n");
    adcInit();
    uartTransmitStr("adcInit_ok\r\n");
    rtcInit();
    uartTransmitStr("rtcInit_ok\r\n");
#ifdef CLOCK_SET_TIME_DATE
    rtcSetTime(18, 8, 0, 6);
    uartTransmitStr("rtcSetTime_ok\r\n");
#endif
    relayBoardInit();
    uartTransmitStr("relayBoardInit_ok\r\n");
    timerInit();
    uartTransmitStr("timerInit_ok\r\n");
    sei();
    _delay_ms(100);
    climateMaining();
}