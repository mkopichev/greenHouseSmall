#include "../inc/lcd.h"

uint8_t data = 0;

void lcdSendByte(uint8_t byte) {

    twiStart();                                  //--- Start Condition
    twiTransmitByte((LCD_TWI_ADDRESS << 1) | W); //--- SLA+W is Send 0x40
    twiTransmitByte(byte);                       //--- Data to Slave Device
    twiStop();                                   //--- Stop Condition
}

/* Function to Write 4-bit data to LCD */

void lcdSend4Bits(uint8_t byte) {

    uint8_t temp = 0x00; //--- temp variable for data operation

    data &= 0x0F;         //--- Masking last four bit to prevent the RS, RW, EN, Backlight
    temp = (byte & 0xF0); //--- Masking higher 4-Bit of Data and send it LCD
    data |= temp;         //--- 4-Bit Data and LCD control Pin
    data |= (0x04);       //--- Latching Data to LCD EN = 1
    lcdSendByte(data);    //--- Send Data From PCF8574 to LCD PORT
    _delay_us(1);         //--- 1us Delay
    data &= ~(0x04);      //--- Latching Complete
    lcdSendByte(data);    //--- Send Data From PCF8574 to LCD PORT
    _delay_us(5);         //--- 5us Delay to Complete Latching

    temp = ((byte & 0x0F) << 4); //--- Masking Lower 4-Bit of Data and send it LCD
    data &= 0x0F;                //--- Masking last four bit to prevent the RS, RW, EN, Backlight
    data |= temp;                //--- 4-Bit Data and LCD control Pin
    data |= (0x04);              //--- Latching Data to LCD EN = 1
    lcdSendByte(data);           //--- Send Data From PCF8574 to LCD PORT
    _delay_us(1);                //--- 1us Delay
    data &= ~(0x04);             //--- Latching Complete
    lcdSendByte(data);           //--- Send Data From PCF8574 to LCD PORT
    _delay_us(5);                //--- 5us Delay to Complete Latching
}

/* Function to Write to LCD Command Register */

void lcdSendCmd(uint8_t byte) {

    data = 0x08;        //--- Enable Backlight Pin
    data &= ~(0x01);    //--- Select Command Register By RS = 0
    lcdSendByte(data);  //--- Send Data From PCF8574 to LCD PORT
    lcdSend4Bits(byte); //--- Function to Write 4-bit data to LCD
}

/* Function to Write to LCD Data Register */

void lcdSendData(uint8_t byte) {

    data |= 0x09;       //--- Enable Backlight Pin & Select Data Register By RS = 1
    lcdSendByte(data);  //--- Send Data From PCF8574 to LCD PORT
    lcdSend4Bits(byte); //--- Function to Write 4-bit data to LCD
}

/* Function to Execute Clear LCD Command */

void lcdClear(void) {

    lcdSendCmd(0x01);
    _delay_ms(2);
}

/* Function to Send String of Data */

void lcdSendStr(char *str) {

    while(*str != '\0')      //--- Check Pointer for Null
        lcdSendData(*str++); //--- Send the String of Data
}

/* Function to Send Integer of Data */

void lcdSendInt(int16_t value) {

    char buffer[20];         // Создаем буфер для хранения строки
    itoa(value, buffer, 10); // Преобразуем значение в строку с основанием 10
    lcdSendStr(buffer);      // Отправляем строку на LCD
}

/* Function to Send Float of Data */

void lcdSendFloat(float value) {

    uint8_t tmp = 0;
    tmp = ((uint16_t)(value * 10)) % 10;
    char buffer[20];         // Создаем буфер для хранения строки
    itoa(value, buffer, 10); // Преобразуем значение в строку с основанием 10
    lcdSendStr(buffer);      // Отправляем строку на LCD
    lcdSendData('.');
    itoa(tmp, buffer, 10); // Преобразуем значение в строку с основанием 10
    lcdSendStr(buffer);    // Отправляем строку на LCD
}

/* Function to set cursor on the screeen */

void lcdSetCursor(uint8_t position) {

    lcdSendCmd((1 << 7) | position);
}

/* Function to Initialize LCD in 4-Bit Mode, Cursor Setting and Row Selection */

void lcdInit(void) {

    data = 0x04; //--- EN = 1 for 25us initialize Sequence
    lcdSendByte(data);
    _delay_us(25);

    lcdSendCmd(0x03); //--- Initialize Sequence
    lcdSendCmd(0x03); //--- Initialize Sequence
    lcdSendCmd(0x03); //--- Initialize Sequence
    lcdSendCmd(0x02); //--- Return to Home
    lcdSendCmd(0x28); //--- 4-Bit Mode 2 - Row Select
    lcdSendCmd(0x0F); //--- Cursor on, Blinking on
    lcdSendCmd(0x01); //--- Clear LCD
    lcdSendCmd(0x06); //--- Auto increment Cursor
    lcdSendCmd(0x80); //--- Row 1 Column 1 Address
    _delay_ms(1000);  //--- 1s Delay
    lcdClear();       //--- Clear LCD
    lcdSendCmd(0x80); //--- Row 1 Column 1 Address
    lcdSendCmd(0x0C);
    lcdSendStr("Loading...");
    lcdSetCursor(0);
    uartTransmitStr("lcdInit_ok\r\n");
}