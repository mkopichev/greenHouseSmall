#include "inc/common.h"

extern bool maintainingPeriodPassed;
extern bool fiveSecPassed, oneSecPassed;
extern bool dayTime;

char tmpStr[18], tmpStrWeekDay[4];

int main(void) {

    bool flagLcd = false;

    wdt_enable(WDTO_8S);

    initAll();

    while(1) {

        if(maintainingPeriodPassed) {

            maintainingPeriodPassed = false;
            uartTransmitStr("maintainingPeriodPassed\r\n");
            climateMaining();
        }

        if(fiveSecPassed) {

            fiveSecPassed = false;
            uartTransmitStr("fiveSecPassed\r\n");

            if(flagLcd) {

                flagLcd = false;
                lcdSetCursor(0);
                if(dayTime) {

                    snprintf(tmpStr, sizeof(tmpStr), "%d.%d oC; ill:%s", (uint8_t)tempSensReadTemp(), (uint16_t)(tempSensReadTemp() * 10) % 10, (getAnalogLight() ? "low" : " ok"));
                } else {

                    snprintf(tmpStr, sizeof(tmpStr), "%d.%d oC; ill:%s", (uint8_t)tempSensReadTemp(), (uint16_t)(tempSensReadTemp() * 10) % 10, "off");
                }
                uartTransmitStr(tmpStr);
                uartTransmitStr("\r\n");
                lcdSendStr(tmpStr);
                uartTransmitStr("string sent to lcd\r\n");
            } else {

                flagLcd = true;
                lcdSetCursor(0);
                snprintf(tmpStr, sizeof(tmpStr), "%dmmHg; moi:%s", (uint16_t)tempSensReadPressure(), (getAnalogMoisture() ? "low" : " ok"));
                uartTransmitStr(tmpStr);
                uartTransmitStr("\r\n");
                lcdSendStr(tmpStr);
                uartTransmitStr("string sent to lcd\r\n");
            }
        }

        if(oneSecPassed) {

            oneSecPassed = false;
            uartTransmitStr("oneSecPassed\r\n");

            rtcGetTimeWeekday();

            if(!rtcGetData(RTC_HOUR)) {

                uartTransmitStr("another week day\r\n");

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
            } else {

                uartTransmitStr("same week day\r\n");
            }

            lcdSetCursor(64);
            uartTransmitStr("cursor set\r\n");
            rtcGetTimeWeekday();
            snprintf(tmpStr, sizeof(tmpStr), "%02d:%02d:%02d %s", rtcGetData(RTC_HOUR), rtcGetData(RTC_MIN), rtcGetData(RTC_SEC), tmpStrWeekDay);
            uartTransmitStr("string ready\r\n");
            uartTransmitStr(tmpStr);
            uartTransmitStr("\r\n");
            lcdSendStr(tmpStr);
            uartTransmitStr("string sent to lcd\r\n");
        }

        wdt_reset();
        _delay_ms(1);
    }
}