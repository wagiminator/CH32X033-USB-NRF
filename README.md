# USB2NRF - USB Controlled 2.4GHz Transceiver
NRF2USB is a simple development tool designed specifically for wireless applications that utilize the low-cost nRF24L01+ 2.4GHz transceiver module. The integrated CH32X033 microcontroller provides a USB interface for communication with the module. The versatility of this tool makes it an ideal choice for a wide range of wireless applications.

Depending on the firmware, it can be used, for example, to transfer serial data wirelessly between two PCs (USB CDC - Communications Device Class) or as a receiver for wireless keyboards, mice or joysticks (USB HID - Human Interface Device). It is also possible to receive sensor data or control remote actuators. This makes it ideal for IoT (Internet of Things) applications where data collected from sensors needs to be transmitted wirelessly to a central device for analysis.

![USB2NRF_pic1.jpg](https://raw.githubusercontent.com/wagiminator/CH32X033-USB-NRF/main/documentation/USB2NRF_pic1.jpg)

# Hardware
## Schematic
![USB2NRF_wiring.png](https://raw.githubusercontent.com/wagiminator/CH32X033-USB-NRF/main/documentation/USB2NRF_wiring.png)

## CH32X033 32-bit RISC-V USB Microcontroller
CH32X033 is a low-cost microcontroller that utilizes the QingKe 32-bit RISC-V4C core, supporting the RV32IMAC instruction set along with self-extending instructions. This microcontroller comes with a built-in USB PHY and supports USB2.0 full-speed device functions. It features a programmable protocol I/O controller (PIOC), 2 groups of operational amplifiers (OPA) with programmable gain (PGA), 2 groups of analog comparators (CMP), a 12-bit analog-to-digital converter (ADC), a 10-channel touch-key controller, 4 groups of USART, I2C, SPI, multiple timers, and various other peripheral resources. The device can operate at clock frequencies of up to 48MHz and is compatible with a supply voltage range of 2.0V to 5.5V. The CH32X033 includes 62KB of flash, 20KB of SRAM, and an embedded USB bootloader.

## nRF24L01+ 2.4GHz Transceiver Module
The nRF24L01+ is a highly integrated, ultra-low power (ULP) 2Mbps RF transceiver IC for the 2.4GHz ISM (Industrial, Scientific and Medical) band. It is designed to be used as a wireless communication module in a variety of applications, such as home automation, wireless gaming, and the Internet of Things (IoT). The module is equipped with an SPI interface, which makes it simple to connect to a variety of microcontrollers, such as the Arduino, Raspberry Pi, and others.

![USB2NRF_pic2.jpg](https://raw.githubusercontent.com/wagiminator/CH32X033-USB-NRF/main/documentation/USB2NRF_pic2.jpg)
![USB2NRF_pic3.jpg](https://raw.githubusercontent.com/wagiminator/CH32X033-USB-NRF/main/documentation/USB2NRF_pic3.jpg)

# Software
## Firmware Versions
### NRF to CDC
This firmware provides a serial interface for communication with the module via USB CDC. It can be used to transfer serial data wirelessly between two PCs, receive data from a remote sensor or trasmit commands to an actuator.

#### Operating Instructions:
Open a serial monitor and set the correct serial port (BAUD rate doesn't matter). Enter the text to be sent, terminated with a Newline (NL or '\ n'). A string that begins with an exclamation mark ('!') is recognized as a command. The command is given by the letter following the exclamation mark. Command arguments are appended as bytes in 2-digit hexadecimal directly after the command. The following commands can be used to set the NRF:

|Command|Description|Example|Example Description|
|-|:-|:-|:-|
|c|set channel|!c2A|set channel to 0x2A (0x00 - 0x7F)|
|t|set TX address|!t7B271F1F1F|addresses are 5 bytes, LSB first|
|r|set RX address|!r41C355AA55|addresses are 5 bytes, LSB first|
|s|set speed|!s02|data rate (00:250kbps, 01:1Mbps, 02:2Mbps)|

Enter just the exclamation mark ('!') for the actual NRF settings to be printed in the serial monitor.

### NRF to HID
- in development...

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
python3 tools/chprog.py bin/<firmware>.bin
```

### Other Operating Systems
Follow the instructions on [CNLohr's ch32v003fun page](https://github.com/cnlohr/ch32v003fun/wiki/Installation) to set up the toolchain on your respective operating system (for Windows, use WSL). Also, install [Python3](https://www.pythontutorial.net/getting-started/install-python/) and [pyusb](https://github.com/pyusb/pyusb). Compile and upload with "make flash". Note that I only have Debian-based Linux and have not tested it on other operating systems.

## Compiling and Uploading Firmware using PlatformIO
- Install [PlatformIO](https://platformio.org) and [platform-ch32v](https://github.com/Community-PIO-CH32V/platform-ch32v). Follow [these instructions](https://pio-ch32v.readthedocs.io/en/latest/installation.html) to do so. Linux/Mac users may also need to install [pyenv](https://realpython.com/intro-to-pyenv).
- Click on "Open Project" and select the firmware folder with the *platformio.ini* file.
- Press the BOOT button and keep it pressed while connecting the board to the USB port of your PC. Then click "Upload".

## Uploading pre-compiled Firmware Binary
WCH offers the free but closed-source software [WCHISPTool](https://www.wch.cn/downloads/WCHISPTool_Setup_exe.html) to upload firmware with Windows via the USB bootloader. Press the BOOT button and keep it pressed while connecting the board to the USB port of your PC. Release the BOOT button, open the *<firmware>.hex* file in the *bin* folder with WCHISPTool and upload it to the microcontroller.

## Installing CDC driver
On Linux you do not need to install a CDC driver. On Windows you may need the [Zadig tool](https://zadig.akeo.ie/) to install the correct driver for the CDC device. Click "Options" and "List All Devices" to select the CDC device, then install the CDC driver.

# References, Links and Notes
- [EasyEDA Design Files](https://oshwlab.com/wagiminator)
- [MCU Templates](https://github.com/wagiminator/MCU-Templates)
- [MCU Flash Tools](https://github.com/wagiminator/MCU-Flash-Tools)
- [CH32X033 Datasheets](http://www.wch-ic.com/products/CH32X035.html)
- [nRF24L01+ Datasheet](https://www.sparkfun.com/datasheets/Components/SMD/nRF24L01Pluss_Preliminary_Product_Specification_v1_0.pdf)
- [ATtiny814 USB2NRF](https://github.com/wagiminator/ATtiny814-NRF2USB)
- [CH552G USB2NRF](https://github.com/wagiminator/CH552-USB-NRF)
- [nRF24L01+ on Aliexpress](http://aliexpress.com/wholesale?SearchText=nrf24l01+plus+smd)

# License
![license.png](https://i.creativecommons.org/l/by-sa/3.0/88x31.png)

This work is licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License. 
(http://creativecommons.org/licenses/by-sa/3.0/)
