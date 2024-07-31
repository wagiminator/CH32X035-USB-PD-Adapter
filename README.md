# USB PD Adapter
The USB PD Adapter is a compact variable power supply capable of utilizing nearly any USB Type-C PD power supply with PPS-feature as an input to power your projects with various selectable voltages and high currents. Key parameters such as voltage, current, power, and energy are conveniently displayed on the OLED screen. This adapter is built around the cost-effective (30 cents at the time of writing) and user-friendly CH32X035 32-bit RISC-V microcontroller, which features hardware USB PD support, along with the INA219 voltage and current sensor IC.

![USB_PD_Adapter_pic1.jpg](https://raw.githubusercontent.com/wagiminator/CH32X035-USB-PD-Adapter/main/documentation/USB_PD_Adapter_pic1.jpg)

# Hardware
## Schematic
![USB_PD_Adapter_wiring.png](https://raw.githubusercontent.com/wagiminator/CH32X035-USB-PD-Adapter/main/documentation/USB_PD_Adapter_wiring.png)

## CH32X035 F7P6 32-bit RISC-V USB Microcontroller
CH32X035F7P6 is a low-cost microcontroller that utilizes the QingKe 32-bit RISC-V4C core, supporting the RV32IMAC instruction set along with self-extending instructions. This microcontroller comes with a built-in USB PHY, supporting USB2.0 full-speed device functions and a USB PD PHY with source and sink capabilities. It features a programmable protocol I/O controller (PIOC), an operational amplifier (OPA) with programmable gain (PGA), an analog comparator (CMP), a 12-bit analog-to-digital converter (ADC), an 11-channel touch-key controller, 3 groups of USART, I2C, SPI, multiple timers, and various other peripheral resources. The device can operate at clock frequencies of up to 48MHz and is compatible with a supply voltage range of 2.0V to 5.5V. The CH32X035F7P6 includes 48KB of flash, 20KB of SRAM, and an embedded USB bootloader.

The CH32X035 microcontroller manages several tasks, including power negotiation with the USB PD power supply, INA219 management, user interface control, and computation and presentation of measured values. The user interface is comprised of three buttons and an SSD1306 I²C OLED display with a resolution of 128x32 pixels.

## INA219 Current/Power Monitor
The INA219 is a current shunt and power monitor with an I²C-compatible interface. The device monitors both the shunt voltage drop and bus supply voltage, with programmable conversion times and filtering. A programmable calibration value, combined with an internal multiplier, enables direct readouts of current in amperes.

The selected shunt resistance of 10mΩ enables both a very small influence on the circuit and a measurement with a resolution of 1mA. For accurate measurement, a shunt resistor with a low tolerance (1% or better) should be selected. The INA219 is used here to measure the output voltage and output current. It communicates with the MCU via I²C.

## HT7533-1 Voltage Regulator
The HT75xx-1 series comprises three-terminal high current low voltage regulators crafted using CMOS technology. They are capable of delivering an output current of 100mA and can handle input voltages as high as 30V. The utilization of CMOS technology guarantees minimal voltage drop and low quiescent current.

The HT7533-1 provides a stable 3.3V supply to all components of the circuit. Ensure that you select a version with a maximum input voltage rating of 30V.

## SSD1306 OLED Display Module
A low-cost SSD1306 4-pin I2C 128x32 pixels 0.91-inch OLED module is used as the display device. Make sure to acquire one with the correct pinout!

![USB_PD_Adapter_pic2.jpg](https://raw.githubusercontent.com/wagiminator/CH32X035-USB-PD-Adapter/main/documentation/USB_PD_Adapter_pic2.jpg)

# Software
## USB Bootloader
### Installing Drivers for the Bootloader
On Linux you do not need to install a driver. However, by default Linux will not expose enough permission to upload your code with the USB bootloader. In order to fix this, open a terminal and run the following commands:

```
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="4348", ATTR{idProduct}=="55e0", MODE="666"' | sudo tee /etc/udev/rules.d/99-ch55x.rules
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="1a86", ATTR{idProduct}=="55e0", MODE="666"' | sudo tee -a /etc/udev/rules.d/99-ch55x.rules
sudo udevadm
```

For Windows you can use the [Zadig Tool](https://zadig.akeo.ie/) to install the correct driver. Here, click "Options" -> "List All Devices" and select the USB module. Then install the libusb-win32 driver. To do this, the board must be connected and the microcontroller must be in bootloader mode.

### Entering Bootloader Mode
The bootloader must be started manually for new uploads. To do this, the board must first be disconnected from the USB port. Now press the BOOT button and keep it pressed while reconnecting the board to the USB port of your PC. The chip now starts in bootloader mode, the BOOT button can be released and new firmware can be uploaded via USB within the next couple of seconds.

## Compiling and Uploading Firmware using the Makefile
### Linux
Install the toolchain (GCC compiler, Python3, and PyUSB):
```
sudo apt install build-essential libnewlib-dev gcc-riscv64-unknown-elf
sudo apt install python3 python3-pip
python3 -m pip install pyusb
```

Open a terminal and navigate to the folder with the *makefile*. Press the BOOT button and keep it pressed while connecting the board to the USB port of your PC. Run the following command to compile and upload:
```
make flash
```

If you want to just upload the pre-compiled binary, run the following command instead:
```
python3 tools/chprog.py bin/pd_adapter.bin
```

### Other Operating Systems
Follow the instructions on [CNLohr's ch32v003fun page](https://github.com/cnlohr/ch32v003fun/wiki/Installation) to set up the toolchain on your respective operating system (for Windows, use WSL). Also, install [Python3](https://www.pythontutorial.net/getting-started/install-python/) and [pyusb](https://github.com/pyusb/pyusb). Compile and upload with "make flash". Note that I only have Debian-based Linux and have not tested it on other operating systems.

## Compiling and Uploading Firmware using PlatformIO
- Install [PlatformIO](https://platformio.org) and [platform-ch32v](https://github.com/Community-PIO-CH32V/platform-ch32v). Follow [these instructions](https://pio-ch32v.readthedocs.io/en/latest/installation.html) to do so. Linux/Mac users may also need to install [pyenv](https://realpython.com/intro-to-pyenv).
- Click on "Open Project" and select the firmware folder with the *platformio.ini* file.
- Press the BOOT button and keep it pressed while connecting the board to the USB port of your PC. Then click "Upload".

## Uploading pre-compiled Firmware Binary
WCH offers the free but closed-source software [WCHISPTool](https://www.wch.cn/downloads/WCHISPTool_Setup_exe.html) to upload firmware with Windows via the USB bootloader. Press the BOOT button and keep it pressed while connecting the board to the USB port of your PC. Release the BOOT button, open the *pd_adapter.hex* file in the *bin* folder with WCHISPTool and upload it to the microcontroller.

# Operating Instructions
1. Connect the USB PD Adapter to a USB Type-C PD power supply using a USB-C cable.
2. Use the INC/DEC buttons to select the desired output voltage in steps of 20mV.
3. Connect the device to the power consumer via the output screw terminal.
4. Use the RST button to clear the energy counter.

![USB_PD_Adapter_operation.png](https://raw.githubusercontent.com/wagiminator/CH32X035-USB-PD-Adapter/main/documentation/USB_PD_Adapter_operation.png)

# References, Links and Notes
- [EasyEDA Design Files](https://oshwlab.com/wagiminator)
- [MCU Templates](https://github.com/wagiminator/MCU-Templates)
- [MCU Flash Tools](https://github.com/wagiminator/MCU-Flash-Tools)
- [CH32X035 Datasheets](http://www.wch-ic.com/products/CH32X035.html)
- [INA219 Datasheet](https://www.ti.com/lit/ds/symlink/ina219.pdf?ts=1662832146107)
- [SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
- [HT7533-1 Datasheet](https://www.farnell.com/datasheets/2047678.pdf)
- [ATtiny814 USB PD Adapter](https://github.com/wagiminator/ATtiny814-USB-PD-Adapter)
- [ATtiny412 USB PD Inverter](https://github.com/wagiminator/ATtiny412-USB-PD-Inverter)
- [CH32X035 USB PD Tester](https://github.com/wagiminator/CH32X035-USB-PD-Tester)
- [TI Primer on USB PD](https://www.ti.com/lit/wp/slyy109b/slyy109b.pdf)
- [CH32X035 F7P6 on Aliexpress](https://aliexpress.com/item/1005006199310724.html)
- [128x32 OLED on Aliexpress](https://aliexpress.com/wholesale?SearchText=128+32+0.91+oled)

![USB_PD_Adapter_pic3.jpg](https://raw.githubusercontent.com/wagiminator/CH32X035-USB-PD-Adapter/main/documentation/USB_PD_Adapter_pic3.jpg)
![USB_PD_Adapter_pic4.jpg](https://raw.githubusercontent.com/wagiminator/CH32X035-USB-PD-Adapter/main/documentation/USB_PD_Adapter_pic4.jpg)

# License
![license.png](https://i.creativecommons.org/l/by-sa/3.0/88x31.png)

This work is licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License. 
(http://creativecommons.org/licenses/by-sa/3.0/)
