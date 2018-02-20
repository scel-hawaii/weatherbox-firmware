/*******************************
 *
 * File: gs_dev_digi_xbee.cpp
 *
 * This module is a driver for the XBee Transceiver. This
 * is used to radio communication with the server
 *
 * Product page: http://www.digi.com/support/productdetail?pid=4549
 * Datasheet: http://www.digi.com/resources/documentation/digidocs/PDFs/90000976.pdf
 *
 ******************************/

#include "gs_dev_digi_xbee.h"

/******************************
 *
 * Name:        gs_dev_digi_xbee_open
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Initialize XBee pins
 *
 ******************************/

void gs_dev_digi_xbee_open(void)
{
    soft_serial.begin(9600);
    xbee.begin(soft_serial);
}

/******************************
 *
 * Name:        gs_dev_digi_xbee_avail
 * Returns:     0 if busy, 1 if available
 * Parameter:   Nothing
 * Description: Check to see if the XBee is busy
 *
 ******************************/

int gs_dev_digi_xbee_avail(void)
{
    return Serial.available();
}

/******************************
 *
 * Name:        gs_dev_digi_xbee_read
 * Returns:     Data from XBee
 * Parameter:   Nothing
 * Description: Read data from the XBee
 *
 ******************************/

int gs_dev_digi_xbee_read(void)
{
    return Serial.read();
}

/******************************
 *
 * Name:        gs_dev_digi_xbee_write
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Transmits packet through XBee
 *
 ******************************/

void gs_dev_digi_xbee_write(uint8_t *data, int data_len)
{
    XBeeAddress64 addr64 = XBeeAddress64(0, 0);
    ZBTxRequest zbtx = ZBTxRequest(addr64, data, data_len);

    xbee.send(zbtx);
}
