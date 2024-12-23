#include "inc/common.h"

extern bool maintainingPeriodPassed;
extern bool fiveSecPassed, oneSecPassed;
extern bool dayTime;

int main(void) {

    bool flagLcd = false;
    char tmpStr[17], tmpStrWeekDay[4];

    initAll();

    lcdSetCursor(0);
    if(dayTime) {

        snprintf(tmpStr, sizeof(tmpStr), "%d.%d oC; ill:%s", (uint8_t)tempSensReadTemp(), (uint16_t)(tempSensReadTemp() * 10) % 10, (getAnalogLight() ? "low" : " ok"));
    } else {

        snprintf(tmpStr, sizeof(tmpStr), "%d.%d oC; ill:%s", (uint8_t)tempSensReadTemp(), (uint16_t)(tempSensReadTemp() * 10) % 10, "off");
    }
    uartTransmitStr(tmpStr);
    uartTransmitStr("\r\n");
    lcdSendStr(tmpStr);

    while(1) {

        if(maintainingPeriodPassed) {

            maintainingPeriodPassed = false;
            climateMaining();
        }

        if(fiveSecPassed) {

            fiveSecPassed = false;

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
            } else {

                flagLcd = true;
                lcdSetCursor(0);
                snprintf(tmpStr, sizeof(tmpStr), "%dmmHg; moi:%s", (uint16_t)tempSensReadPressure(), (getAnalogMoisture() ? "low" : " ok"));
                uartTransmitStr(tmpStr);
                uartTransmitStr("\r\n");
                lcdSendStr(tmpStr);
            }
        }

        if(oneSecPassed) {

            oneSecPassed = false;

            switch(rtcGetTimeWeekday(RTC_WEEKDAY) & 0x07) {
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
            }

            lcdSetCursor(64);
            snprintf(tmpStr, sizeof(tmpStr), "%02d:%02d:%02d %s", rtcGetTimeWeekday(RTC_HOUR), rtcGetTimeWeekday(RTC_MIN), rtcGetTimeWeekday(RTC_SEC), tmpStrWeekDay);
            uartTransmitStr(tmpStr);
            uartTransmitStr("\r\n");
            lcdSendStr(tmpStr);
        }

        _delay_ms(1);
    }
}