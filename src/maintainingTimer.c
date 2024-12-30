#include "../inc/maintainingTimer.h"

bool maintainingPeriodPassed = false;
bool fiveSecPassed = false, oneSecPassed = false;
bool dayTime = true;

void timerInit(void) {

    TCCR0B |= (1 << CS00) | (1 << CS02); // prescaler 1024
    TIMSK0 |= (1 << TOIE0);
    TCNT0 = 0x64; // 10ms period
    uartTransmitStr("maintainingTimerInit_ok\r\n");
}

ISR(TIMER0_OVF_vect) {

    static uint32_t t0_counter = 0;
    TCNT0 = 0x64;

    if(t0_counter < (MAINTAINIG_PERIOD * 100)) {

        t0_counter++;

        if(!(t0_counter % 500)) {

            fiveSecPassed = true;
        }
        if(!(t0_counter % 100)) {

            oneSecPassed = true;
        }
    } else {

        t0_counter = 0;
        maintainingPeriodPassed = true;
    }
}

void climateMaining(void) {

    uartTransmitStr("climateMaintaining started\r\n");
    relayBoard(HEATING_LAMP, TURN_OFF);
    uartTransmitStr("heatingLamp OFF\r\n");
    relayBoard(FITO_LAMP, TURN_OFF);
    uartTransmitStr("fitoLamp OFF\r\n");

    _delay_ms(500);

    if(tempSensReadTemp() < TEMP_SETPOINT) {

        relayBoard(HEATING_LAMP, TURN_ON);
        uartTransmitStr("heatingLamp ON\r\n");
    } else {

        relayBoard(HEATING_LAMP, TURN_OFF);
        uartTransmitStr("heatingLamp OFF\r\n");
    }

    if(tempSensReadTemp() > (TEMP_SETPOINT + TEMP_SETPOINT_DELTA)) {

        relayBoard(COOLING_FAN, TURN_ON);
        uartTransmitStr("coolingFan ON\r\n");
    } else {

        relayBoard(COOLING_FAN, TURN_OFF);
        uartTransmitStr("coolingFan OFF\r\n");
    }

    rtcGetTimeWeekday();

    if((rtcGetData(RTC_HOUR) < 22) && (rtcGetData(RTC_HOUR) > 10)) {

        dayTime = true;
        uartTransmitStr("dayTime TRUE\r\n");

        if(getAnalogLight()) {

            relayBoard(FITO_LAMP, TURN_ON);
            uartTransmitStr("fitoLamp ON\r\n");
        } else {

            relayBoard(FITO_LAMP, TURN_OFF);
            uartTransmitStr("fitoLamp OFF\r\n");
        }
    } else {

        uartTransmitStr("dayTime FALSE\r\n");
        dayTime = false;
    }

    // waterPumpPour();

    uartTransmitStr("climateMaining finished\r\n");
}