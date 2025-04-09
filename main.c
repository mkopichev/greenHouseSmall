#include "inc/common.h"

extern bool maintainingPeriodPassed;
extern bool tenSecPassed, oneSecPassed;
extern bool dayTime;
extern bool pouring;

char tmpStr[18], tmpStrWeekDay[4];

int16_t maintainingPeriodCounter;

int main(void) {

    bool flagLcd = false;

    maintainingPeriodCounter = MAINTAINIG_PERIOD - 1;

    initAll();

    while(1) {

        if(maintainingPeriodPassed) {

            maintainingPeriodPassed = false;
            uartTransmitStr("maintainingPeriodPassed\r\n");
            climateMaining();
        }

        if(tenSecPassed) {

            tenSecPassed = false;
            uartTransmitStr("tenSecPassed\r\n");

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
                uartTransmitStr("temperature and illumination data string sent to lcd\r\n");
            } else {

                flagLcd = true;
                lcdSetCursor(0);
                snprintf(tmpStr, sizeof(tmpStr), "%dmmHg; moi:%s", (uint16_t)tempSensReadPressure(), (getAnalogMoisture() ? "low" : " ok"));
                uartTransmitStr(tmpStr);
                uartTransmitStr("\r\n");
                lcdSendStr(tmpStr);
                uartTransmitStr("pressure and moisture data string sent to lcd\r\n");
            }
        }

        if(oneSecPassed) {

            oneSecPassed = false;
            uartTransmitStr("oneSecPassed\r\n");

            if(pouring) {

                pouring = false;
                waterPumpStop();
                uartTransmitStr("pouring OFF\r\n");
            }

            rtcGetTimeDate();

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
            if(!flagLcd) {

                if(maintainingPeriodCounter / 100) {

                    snprintf(tmpStr, sizeof(tmpStr), "%02d:%02d:%02d %s %03d", rtcGetData(RTC_HOUR), rtcGetData(RTC_MIN), rtcGetData(RTC_SEC), tmpStrWeekDay, maintainingPeriodCounter);
                } else if(maintainingPeriodCounter / 10) {

                    snprintf(tmpStr, sizeof(tmpStr), "%02d:%02d:%02d %s  %02d", rtcGetData(RTC_HOUR), rtcGetData(RTC_MIN), rtcGetData(RTC_SEC), tmpStrWeekDay, maintainingPeriodCounter);
                } else {

                    snprintf(tmpStr, sizeof(tmpStr), "%02d:%02d:%02d %s   %01d", rtcGetData(RTC_HOUR), rtcGetData(RTC_MIN), rtcGetData(RTC_SEC), tmpStrWeekDay, maintainingPeriodCounter);
                }
                uartTransmitStr(tmpStr);
                uartTransmitStr("\r\n");
                lcdSendStr(tmpStr);
                uartTransmitStr("timestamp string sent to lcd\r\n");
            } else {

                if(maintainingPeriodCounter / 100) {

                    snprintf(tmpStr, sizeof(tmpStr), "%02d.%02d.%02d %s %03d", rtcGetData(RTC_DATE), rtcGetData(RTC_MONTH), rtcGetData(RTC_YEAR), tmpStrWeekDay, maintainingPeriodCounter);
                } else if(maintainingPeriodCounter / 10) {

                    snprintf(tmpStr, sizeof(tmpStr), "%02d.%02d.%02d %s  %02d", rtcGetData(RTC_DATE), rtcGetData(RTC_MONTH), rtcGetData(RTC_YEAR), tmpStrWeekDay, maintainingPeriodCounter);
                } else {

                    snprintf(tmpStr, sizeof(tmpStr), "%02d.%02d.%02d %s   %01d", rtcGetData(RTC_DATE), rtcGetData(RTC_MONTH), rtcGetData(RTC_YEAR), tmpStrWeekDay, maintainingPeriodCounter);
                }
                uartTransmitStr(tmpStr);
                uartTransmitStr("\r\n");
                lcdSendStr(tmpStr);
                uartTransmitStr("datestamp string sent to lcd\r\n");
            }
            if(maintainingPeriodCounter-- <= 0) {

                maintainingPeriodCounter = MAINTAINIG_PERIOD - 1;
            }
        }
        wdt_reset();
        _delay_ms(1);
    }
}