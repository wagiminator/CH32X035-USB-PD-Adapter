// ===================================================================================
// Application-Specific SSD1306 128x32 Pixels I2C OLED Functions              * v1.0 *
// ===================================================================================
//
// OLED_init()              OLED init function
// OLED_setCursor(x, y)     set the cursor
// OLED_clearLine(y)        clear a line
// OLED_clearScreen()       clear screen
// OLED_printChar(ch)       print a character
// OLED_printStr(ptr)       print a "string"; terminator: 255
// OLED_printVal(val)       print integer value
// OLED_printDec(val, lead) print 8-bit value as 2-digit decimal
//
// 2021 by Stefan Wagner:   https://github.com/wagiminator

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c_soft.h"

// OLED definitions
#define OLED_ADDR       0x78      // OLED write address
#define OLED_CMD_MODE   0x00      // set command mode
#define OLED_DAT_MODE   0x40      // set data mode

// Character definitions
#define OLED_COLON      19
#define OLED_SPACE      20

// Some "strings"
extern const uint8_t OLED_mA[];   // "mA"
extern const uint8_t OLED_mV[];   // "mV"
extern const uint8_t OLED_mW[];   // "mW "
extern const uint8_t OLED_Ah[];   // "Ah "
extern const uint8_t OLED_mAh[];  // "mAh"
extern const uint8_t OLED_Wt[];   // "W  "
extern const uint8_t OLED_Wh[];   // "Wh "
extern const uint8_t OLED_mWh[];  // "mWh"
extern const uint8_t OLED_PPS[];  // "NO PPS"

// OLED functions
void OLED_init(void);                             // OLED init function
void OLED_setCursor(uint8_t xpos, uint8_t ypos);  // set the cursor
void OLED_clearLine(uint8_t ypos);                // clear a line
void OLED_clearScreen(void);                      // clear screen
void OLED_printChar(uint8_t ch);                  // print a character
void OLED_printStr(const uint8_t* p);             // print a "string"; terminator: 255
void OLED_printVal(uint16_t value);               // print integer value
void OLED_printDec(uint8_t value, uint8_t lead);  // print 8-bit value as 2-digit decimal

#ifdef __cplusplus
};
#endif
