#include "../inc/maintainingTimer.h"

bool maintainingPeriodPassed = false;
bool fiveSecPassed = false, oneSecPassed = false;
bool dayTime = true;

void timerInit(void) {

    TCCR0B = (1 << CS00) | (1 << CS02); // prescaler 1024
    TIMSK0 = (1 << TOIE0);
    TCNT0 = 0x64; // 10ms period
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

    relayBoard(HEATING_LAMP, TURN_OFF);
    relayBoard(FITO_LAMP, TURN_OFF);

    _delay_ms(500);

    if(tempSensReadTemp() < TEMP_SETPOINT) {

        relayBoard(HEATING_LAMP, TURN_ON);
    } else {

        relayBoard(HEATING_LAMP, TURN_OFF);
    }

    if(tempSensReadTemp() > (TEMP_SETPOINT + TEMP_SETPOINT_DELTA)) {

        relayBoard(COOLING_FAN, TURN_ON);
    } else {

        relayBoard(COOLING_FAN, TURN_OFF);
    }

    if((rtcGetTimeWeekday(RTC_HOUR) < 22) && (rtcGetTimeWeekday(RTC_HOUR) > 10)) {

        dayTime = true;

        if(getAnalogLight()) {

            relayBoard(FITO_LAMP, TURN_ON);
        } else {

            relayBoard(FITO_LAMP, TURN_OFF);
        }
    } else {

        dayTime = false;
    }
}