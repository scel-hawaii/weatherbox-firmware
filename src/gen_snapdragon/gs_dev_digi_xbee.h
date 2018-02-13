/*******************************
 *
 * File: gs_dev_digi_xbee.h
 *
 * Contains prototypes for XBee Transceiver functions
 *
 ******************************/

#include <Arduino.h>
#include <XBee.h>
#include <SoftwareSerial.h>

#define _GS_DEV_DIGI_XBEE_BUFSIZE_ 150

#ifndef GS_DEV_DIGI_XBEE
#define GS_DEV_DIGI_XBEE
void gs_dev_digi_xbee_open(void);
int gs_dev_digi_xbee_avail(void);
int gs_dev_digi_xbee_read(void);
void gs_dev_digi_xbee_write(uint8_t* data, int data_len);

static XBee xbee = XBee();

const byte rx_pin = 2;
const byte tx_pin = 9;
static SoftwareSerial soft_serial(rx_pin, tx_pin);
#endif
