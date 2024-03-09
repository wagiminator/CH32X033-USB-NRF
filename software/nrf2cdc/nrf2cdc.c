// ===================================================================================
// Project:   Example for CH32X035/X034/X033
// Version:   v1.0
// Year:      2023
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// Description:
// ------------
// Blink example.
//
// References:
// -----------
// - WCH Nanjing Qinheng Microelectronics: http://wch.cn
//
// Compilation Instructions:
// -------------------------
// - Make sure GCC toolchain (gcc-riscv64-unknown-elf, newlib) and Python3 with PyUSB
//   is installed. In addition, Linux requires access rights to the USB bootloader.
// - Press the BOOT button on the MCU board and keep it pressed while connecting it
//   via USB to your PC.
// - Run 'make flash'.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================
#include <config.h>
#include <system.h>                               // system functions
#include <gpio.h>                                 // GPIO functions
#include <usb_cdc.h>
#include <nrf24l01.h>


// Global variables
uint8_t buffer[NRF_PAYLOAD];                      // rx/tx buffer

// ===================================================================================
// Print Functions and String Conversions
// ===================================================================================

// Convert an array of bytes into hex string and print via CDC
void CDC_printBytes(uint8_t *ptr, uint8_t len) {
  while(len--) CDC_printB(*ptr++);
}

// Convert character representing a hex nibble into 4-bit value
uint8_t hexDigit(uint8_t c) {
  if     ((c >= '0') && (c <= '9')) return(c - '0');
  else if((c >= 'a') && (c <= 'f')) return(c - 'a' + 10);
  else if((c >= 'A') && (c <= 'F')) return(c - 'A' + 10); 
  return 0;
}

// Convert string containing a hex byte into 8-bit value
uint8_t hexByte(uint8_t *ptr) {
  return((hexDigit(*ptr++) << 4) + hexDigit(*ptr));
}

// Convert string containing 5 hex bytes into address array
void hexAddress(uint8_t *sptr, uint8_t *aptr) {
  uint8_t i;
  for(i=5; i; i--) {
    *aptr++ = hexByte(sptr);
    sptr += 2;
  }
}

// Print the current NRF settings via CDC
void CDC_printSettings(void) {
  CDC_println("# nRF24L01+ Configuration:");
  CDC_print  ("# RF channel: "); CDC_printB    (NRF_channel);    CDC_write('\n');
  CDC_print  ("# TX address: "); CDC_printBytes(NRF_tx_addr, 5); CDC_write('\n');
  CDC_print  ("# RX address: "); CDC_printBytes(NRF_rx_addr, 5); CDC_write('\n');
  CDC_print  ("# Data rate:  "); CDC_print(NRF_STR[NRF_speed]);  CDC_println("bps");
}

// ===================================================================================
// Command Parser
// ===================================================================================
void parse(void) {
  uint8_t cmd = buffer[1];                          // read the command
  switch(cmd) {                                     // what command?
    case 'c': NRF_channel = hexByte(buffer + 2) & 0x7F;
              break;
    case 't': hexAddress(buffer + 2, NRF_tx_addr);
              break;
    case 'r': hexAddress(buffer + 2, NRF_rx_addr);
              break;
    case 's': NRF_speed = hexByte(buffer + 2);
              if(NRF_speed > 2) NRF_speed = 2;
              break;
    default:  break;
  }
  NRF_configure();                                  // reconfigure the NRF
  CDC_printSettings();                              // print settings via CDC
}

// ===================================================================================
// Main Function
// ===================================================================================
int main(void) {
  // Variables
  uint8_t buflen;                                   // data length in buffer
  uint8_t bufptr;                                   // buffer pointer

  // Setup
  CDC_init();                                       // init USB CDC
  NRF_init();                                       // init nRF24L01+
  PIN_output(PIN_LED);                              // set LED pin to output

  // Loop
  while(1) {
    PIN_high(PIN_LED);                              // switch off LED

    if(NRF_available()) {                           // something coming in via NRF?
      PIN_low(PIN_LED);                             // switch on LED
      bufptr = 0;                                   // reset buffer pointer
      buflen = NRF_readPayload(buffer);             // read payload into buffer
      while(buflen--) CDC_write(buffer[bufptr++]);  // write buffer via USB CDC
      CDC_flush();                                  // flush CDC
    }

    buflen = CDC_available();                       // get number of bytes in CDC IN
    if(buflen) {                                    // something coming in via USB?
      bufptr = 0;                                   // reset buffer pointer
      if(buflen > NRF_PAYLOAD) buflen = NRF_PAYLOAD;// restrict length to max payload
      while(buflen--) buffer[bufptr++] = CDC_read();// get data from CDC
      if(buffer[0] == CMD_IDENT) parse();           // is it a command? -> parse
      else {                                        // not a command?
        PIN_low(PIN_LED);                           // switch on LED
        NRF_writePayload(buffer, bufptr);           // send the buffer via NRF
      }
    }
  }
}
