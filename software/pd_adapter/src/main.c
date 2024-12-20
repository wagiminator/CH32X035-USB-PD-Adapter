// ===================================================================================
// Project:   USB PD Adapter for CH32X035
// Version:   v1.2
// Year:      2024
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// Description:
// ------------
// With the USB PD Adapter you can use almost any PPS-capable USB Type-C PD power 
// supply to power your projects with different selectable voltages and high currents.
// Important values such as voltage, current, power and energy are displayed on the
// OLED. The USB PD Adapter is based on the cheap and easy-to-use CH32X035 32-bit
// RISC-V microcontroller with hardware USB PD support and the INA219 voltage and
// current sensor IC.
//
// References:
// -----------
// - WCH Nanjing Qinheng Microelectronics: http://wch.cn
//
// Compilation Instructions:
// -------------------------
// - Make sure GCC toolchain (gcc-riscv64-unknown-elf, newlib) and Python3 with chprog
//   are installed. In addition, Linux requires access rights to the USB bootloader.
// - Press the BOOT button on the MCU board and keep it pressed while connecting it
//   via USB to your PC.
// - Run 'make flash'.
//
// Operating Instructions:
// -----------------------
// 1. Connect the USB PD Adapter to a USB Type-C PD power supply using a USB-C cable.
// 2. Use the INC/DEC buttons to select the desired output voltage in steps of 20mV.
// 3. Connect the device to the power consumer via the output screw terminal.
// 4. Use the RST button to clear the energy counter.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================
#include <config.h>                               // user configurations
#include <system.h>                               // system functions
#include <gpio.h>                                 // GPIO functions
#include <millis.h>                               // millis functions
#include <i2c_soft.h>                             // software I2C functions
#include <oled.h>                                 // OLED functions
#include <ina219.h>                               // INA219 functions
#include <usbpd_sink.h>                           // USB PD sink functions


// ===================================================================================
// Main Function
// ===================================================================================
int main(void) {
  // Local variables
  uint16_t volt, curr;                            // voltage in mV, current in mA
  uint32_t power;                                 // power in mW
  uint32_t energy = 0, charge = 0;                // counter for energy and charge
  uint32_t interval, nowmillis, lastmillis = 0;   // for timing calculation in millis
  uint32_t duration = 0;                          // total duration in ms
  uint16_t seconds = 0;                           // total duration in seconds
  uint16_t minvolt = 5000;                        // minimum selectable voltage in mV
  uint16_t maxvolt = 5000;                        // maximum selectable voltage in mV
  uint16_t setvolt = 5000;                        // selected volatge
  uint8_t keydelay;                               // key repeat delay
  uint8_t incdecpressed = 0;                      // INC/DEC key pressed flag
  uint8_t refreshcounter;                         // PD negotiation refresh counter
  uint8_t i;

  // Setup button pins
  PIN_input_PU(PIN_RST);
  PIN_input_PU(PIN_DEC);
  PIN_input_PU(PIN_INC);

  // Setup milis and I2C devices
  MIL_init();                                     // setup millis counter
  I2C_init();                                     // setup I2C
  INA_init();                                     // setup INA219
  OLED_init();                                    // setup OLED

  // Setup USB Power Delivery
  OLED_clearScreen();                             // clear screen
  if((!PD_connect()) || (!PD_getPPSNum())) {      // init USB PD, if failed:
    OLED_setCursor(36, 1);                        // set cursor
    OLED_printStr(OLED_PPS);                      // print "NO PPS"
    while(1);
  }

  // Get max/min voltage of connected USB PD power supply
  for(i = PD_getFixedNum() + 1; i <= PD_getPDONum(); i++) {
    if(PD_getPDOMinVoltage(i) < minvolt) minvolt = PD_getPDOMinVoltage(i);
    if(PD_getPDOMaxVoltage(i) > maxvolt) maxvolt = PD_getPDOMaxVoltage(i);
  }

  // Loop
  while(1) {
    // Read sensor values
    volt = INA_readVoltage();                     // read voltage in mV from INA219
    curr = INA_readCurrent();                     // read current in mA from INA219  
    if(curr > 6000) curr = 0;                     // avoid negative values

    // Calculate timings
    nowmillis   = MIL_read();                     // read millis counter
    interval    = nowmillis - lastmillis;         // calculate recent time interval
    lastmillis  = nowmillis;                      // reset lastmillis
    duration   += interval;                       // calculate total duration in millis
    seconds     = duration / 1000;                // calculate total duration in seconds
  
    // Calculate power, capacity and energy
    power   = (uint32_t)volt * curr / 1000;       // calculate power  in mW
    energy += interval * power / 3600;            // calculate energy in uWh
    charge += interval * curr  / 3600;            // calculate charge in uAh

    // Check RESET button
    if(!PIN_read(PIN_RST)) {
      duration = 0; seconds = 0; energy = 0; charge = 0;
    }

    // Check INCREASE button
    if(!PIN_read(PIN_INC)) {
      setvolt += 20;
      if(setvolt > maxvolt) setvolt = maxvolt;
      incdecpressed = 1;
    }

    // Check DECREASE button
    else if(!PIN_read(PIN_DEC)) {
      setvolt -= 20;
      if(setvolt < minvolt) setvolt = minvolt;
      incdecpressed = 1;
    }

    // Reset key repeat delay
    else keydelay = 50;

    // Display values on the OLED
    OLED_setCursor(0,0);
    OLED_printVal(setvolt); OLED_printStr(OLED_mV);
    OLED_printChar(OLED_SPACE); OLED_printChar(OLED_SPACE);
    OLED_printVal(volt); OLED_printStr(OLED_mV);

    OLED_setCursor(0,2);
    switch(seconds & 0x0C) {
      case 0x00:  if(power > 65535) {
                    OLED_printVal(power / 1000);
                    OLED_printStr(OLED_Wt);
                  } else {
                    OLED_printVal(power);
                    OLED_printStr(OLED_mW);
                  }
                  break;
      case 0x04:  if(energy > 65535000) {
                    OLED_printVal(energy / 1000000);
                    OLED_printStr(OLED_Wh);
                  } else {
                    OLED_printVal(energy / 1000);
                    OLED_printStr(OLED_mWh);
                  }
                  break;
      case 0x08:  if(charge > 65535000) {
                    OLED_printVal(charge / 1000000);
                    OLED_printStr(OLED_Ah);
                  } else {
                    OLED_printVal(charge / 1000);
                    OLED_printStr(OLED_mAh);
                  }
                  break;
      case 0x0C:  OLED_printDec(seconds / 3600, 0); OLED_printChar(OLED_COLON);
                  seconds %= 3600;
                  OLED_printDec(seconds / 60  , 0); OLED_printChar(OLED_COLON);            
                  OLED_printDec(seconds % 60  , 0);
                  break;
      default:    break;
    }
    OLED_printChar(OLED_SPACE);
    OLED_printVal(curr); OLED_printStr(OLED_mA);

    // Key repeat delays and voltage requests
    i = keydelay; keydelay = 0;
    if(incdecpressed) {
      while((i--) && ((!PIN_read(PIN_INC)) | (!PIN_read(PIN_DEC)))) DLY_ms(10);
      incdecpressed = 0;
      if(!--refreshcounter) PD_negotiate();
    }
    else {
      keydelay = 50;
      PD_setVoltage(setvolt);
      DLY_ms(10);
    }
  }
}
