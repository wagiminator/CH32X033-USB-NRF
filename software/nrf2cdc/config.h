// ===================================================================================
// User Configurations
// ===================================================================================

#pragma once

// Pin definitions
#define PIN_CE              PA1       // NRF cable enable pin
#define PIN_CSN             PA4       // NRF SPI slave select pin
#define PIN_LED             PA11      // pin connected to LED

// USB2NRF Settings
#define CMD_IDENT           '!'       // command string identifier

// MCU supply voltage
#define USB_VDD             0         // 0: 3.3V, 1: 5V

// USB device descriptor
#define USB_VENDOR_ID       0x16C0    // VID (shared www.voti.nl)
#define USB_PRODUCT_ID      0x27DD    // PID (shared CDC-ACM)
#define USB_DEVICE_VERSION  0x0100    // v1.0 (BCD-format)
#define USB_LANGUAGE        0x0409    // US English

// USB configuration descriptor
#define USB_MAX_POWER_mA    50        // max power in mA 

// USB descriptor strings
#define MANUF_STR           "wagiminator"
#define PROD_STR            "CH32X035-NRF2USB"
#define SERIAL_STR          "CH32X035CDC"
#define INTERF_STR          "CDC-Serial"
