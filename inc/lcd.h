#ifndef LCD_H_
#define LCD_H_

// 1602 LCD screen with TWI module on it connected to SDA and SCL of atmega

#include "common.h"

#define LCD_TWI_ADDRESS 0x27

void lcdInit(void);
void lcdClear(void);
void lcdSendStr(char *str);
void lcdSendInt(int16_t value);
void lcdSendFloat(float value);
void lcdSetCursor(uint8_t position);

#endif