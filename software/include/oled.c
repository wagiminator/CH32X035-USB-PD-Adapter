// ===================================================================================
// Application-Specific SSD1306 128x32 Pixels I2C OLED Functions              * v1.0 *
// ===================================================================================
// 2021 by Stefan Wagner:   https://github.com/wagiminator

#include "oled.h"

// OLED init settings
const uint8_t OLED_INIT_CMD[] = {
  0xA8, 0x1F,                   // set multiplex for 128x32
  0x20, 0x01,                   // set vertical memory addressing mode
  0xDA, 0x02,                   // set COM pins hardware configuration to sequential
  0x8D, 0x14,                   // enable charge pump
  0xAF                          // switch on OLED
};

// OLED 5x16 font
const uint8_t OLED_FONT[] = {
  0x7C, 0x1F, 0x02, 0x20, 0x02, 0x20, 0x02, 0x20, 0x7C, 0x1F, // 0  0
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x1F, // 1  1
  0x00, 0x1F, 0x82, 0x20, 0x82, 0x20, 0x82, 0x20, 0x7C, 0x00, // 2  2
  0x00, 0x00, 0x82, 0x20, 0x82, 0x20, 0x82, 0x20, 0x7C, 0x1F, // 3  3
  0x7C, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x7C, 0x1F, // 4  4
  0x7C, 0x00, 0x82, 0x20, 0x82, 0x20, 0x82, 0x20, 0x00, 0x1F, // 5  5
  0x7C, 0x1F, 0x82, 0x20, 0x82, 0x20, 0x82, 0x20, 0x00, 0x1F, // 6  6
  0x7C, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x7C, 0x1F, // 7  7
  0x7C, 0x1F, 0x82, 0x20, 0x82, 0x20, 0x82, 0x20, 0x7C, 0x1F, // 8  8
  0x7C, 0x00, 0x82, 0x20, 0x82, 0x20, 0x82, 0x20, 0x7C, 0x1F, // 9  9
  0x7C, 0x3F, 0x82, 0x00, 0x82, 0x00, 0x82, 0x00, 0x7C, 0x3F, // A 10
  0x7C, 0x03, 0x00, 0x0C, 0x00, 0x30, 0x00, 0x0C, 0x7C, 0x03, // V 11
  0x7C, 0x1F, 0x00, 0x20, 0x00, 0x3F, 0x00, 0x20, 0x7C, 0x1F, // W 12
  0x7C, 0x3F, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x3F, // h 13
  0x00, 0x3F, 0x80, 0x00, 0x80, 0x3F, 0x80, 0x00, 0x00, 0x3F, // m 14
  0x7C, 0x1F, 0x82, 0x20, 0x82, 0x20, 0x82, 0x20, 0x00, 0x00, // E 15
  0x7C, 0x1F, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x7C, 0x1F, // N 16
  0x7C, 0x1F, 0x82, 0x00, 0x82, 0x00, 0x82, 0x00, 0x7C, 0x00, // P 17
  0x02, 0x00, 0x02, 0x00, 0x7E, 0x3F, 0x02, 0x00, 0x02, 0x00, // T 18
  0x00, 0x00, 0x30, 0x06, 0x30, 0x06, 0x00, 0x00, 0x00, 0x00, // : 19
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  //   20 SPACE
};

// Some "strings"
const uint8_t OLED_mA[]  = { 14, 10, 255 };                   // "mA"
const uint8_t OLED_mV[]  = { 14, 11, 255 };                   // "mV"
const uint8_t OLED_mW[]  = { 14, 12, 20, 255 };               // "mW "
const uint8_t OLED_Ah[]  = { 10, 13, 20, 255 };               // "Ah "
const uint8_t OLED_mAh[] = { 14, 10, 13, 255 };               // "mAh"
const uint8_t OLED_Wt[]  = { 12, 20, 20, 255 };               // "W  "
const uint8_t OLED_Wh[]  = { 12, 13, 20, 255 };               // "Wh "
const uint8_t OLED_mWh[] = { 14, 12, 13, 255 };               // "mWh"
const uint8_t OLED_PPS[] = { 16,  0, 20, 17, 17,  5, 255 };   // "NO PPS"

// OLED init function
void OLED_init(void) {
  I2C_start(OLED_ADDR);                           // start transmission to OLED
  I2C_write(OLED_CMD_MODE);                       // set command mode
  for (uint8_t i = 0; i < sizeof(OLED_INIT_CMD); i++)
    I2C_write(OLED_INIT_CMD[i]);                  // send the command bytes
  I2C_stop();                                     // stop transmission
}

// OLED set the cursor
void OLED_setCursor(uint8_t xpos, uint8_t ypos) {
  I2C_start(OLED_ADDR);                           // start transmission to OLED
  I2C_write(OLED_CMD_MODE);                       // set command mode
  I2C_write(0x22);                                // command for min/max page
  I2C_write(ypos); I2C_write(ypos+1);             // min: ypos; max: ypos+1
  I2C_write(xpos & 0x0F);                         // set low nibble of start column
  I2C_write(0x10 | (xpos >> 4));                  // set high nibble of start column
  I2C_write(0xB0 | (ypos));                       // set start page
  I2C_stop();                                     // stop transmission
}

// OLED clear a line
void OLED_clearLine(uint8_t ypos) {
  OLED_setCursor(0, ypos);                        // set cursor at line start
  I2C_start(OLED_ADDR);                           // start transmission to OLED
  I2C_write(OLED_DAT_MODE);                       // set data mode
  uint8_t i = 0;                                  // count variable
  do {I2C_write(0x00);} while(--i);               // clear upper half
  I2C_stop();                                     // stop transmission
}

// OLED clear screen
void OLED_clearScreen(void) {
  OLED_clearLine(0); OLED_clearLine(2);           // clear both lines
}

// OLED plot a single character
void OLED_plotChar(uint8_t ch) {
  ch = (ch << 1) + (ch << 3);                     // calculate position of character in font array
  for(uint8_t i=6;  i; i--) I2C_write(0x00);      // print spacing between characters
  for(uint8_t i=10; i; i--) I2C_write(OLED_FONT[ch++]); // print character
}

// OLED print a character
void OLED_printChar(uint8_t ch) {
  I2C_start(OLED_ADDR);                           // start transmission to OLED
  I2C_write(OLED_DAT_MODE);                       // set data mode
  OLED_plotChar(ch);                              // plot the character
  I2C_stop();                                     // stop transmission
}

// OLED print a "string"; terminator: 255
void OLED_printStr(const uint8_t* p) {
  I2C_start(OLED_ADDR);                           // start transmission to OLED
  I2C_write(OLED_DAT_MODE);                       // set data mode
  while(*p < 255) OLED_plotChar(*p++);            // plot each character of the string
  I2C_stop();                                     // stop transmission
}

// OLED print value
void OLED_printVal(uint16_t value) {
  uint8_t digitval;                               // current digit value
  uint8_t leadflag = 0;                           // flag for leading spaces
  uint16_t divider = 10000;                       // current divider
  I2C_start(OLED_ADDR);                           // start transmission to OLED
  I2C_write(OLED_DAT_MODE);                       // set data mode
  while(divider) {                                // for all digits
    digitval = value / divider;                   // calculate digit value
    value    = value % divider;                   // set value to division remainder
    divider /= 10;                                // calculate next divider
    if(digitval) leadflag++;                      // end of leading spaces
    if(!divider) leadflag++;                      // least digit has to be printed
    if(leadflag) OLED_plotChar(digitval);         // print the digit
    else         OLED_plotChar(OLED_SPACE);       // or print leading space
  }
  I2C_stop();                                     // stop transmission
}

// OLED print 8-bit value as 2-digit decimal
void OLED_printDec(uint8_t value, uint8_t lead) {
  I2C_start(OLED_ADDR);                           // start transmission to OLED
  I2C_write(OLED_DAT_MODE);                       // set data mode
  uint8_t first = value / 10;
  if(first) OLED_plotChar(first);                 // print first digit
  else      OLED_plotChar(lead);
  OLED_plotChar(value % 10);                      // print second digit
  I2C_stop();                                     // stop transmission
}
