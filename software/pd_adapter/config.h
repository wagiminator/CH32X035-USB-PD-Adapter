// ===================================================================================
// User Configurations
// ===================================================================================

#pragma once

// Pin definitions
#define PIN_RST             PA2       // pin connected to RESET button (active low)
#define PIN_DEC             PA3       // pin connected to DECREASE button (active low)
#define PIN_INC             PA4       // pin connected to INCREASE button (active low)
#define PIN_SCL             PA5       // pin connected to serial clock of the I2C bus
#define PIN_SDA             PA6       // pin connected to serial data of the I2C bus

// MCU supply voltage
#define USB_VDD             0         // 0: 3.3V, 1: 5V
