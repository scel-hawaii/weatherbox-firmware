#include "gd_dev_xbee.h"

void gd_dev_xbee_open(void)
{
    soft_serial.begin(9600);
    xbee.begin(soft_serial);
    // Enable voltage regulator pin to power the Xbee 
    digitalWrite(3, HIGH);

    // Enable DTR pin as output
    pinMode(A1, OUTPUT);
    digitalWrite(A1, LOW);

    // Set RSSI pin as input
    pinMode(A2, INPUT);

    // Set Xbee sleep pin as input
    pinMode(A3, INPUT);
}

int gd_dev_xbee_avail(void)
{
    return Serial.available();
}

int gd_dev_xbee_read(void)
{
    return Serial.read();
}

void gd_dev_xbee_write(uint8_t *data, int data_len)
{
    XBeeAddress64 addr64 = XBeeAddress64(0, 0);
    ZBTxRequest zbtx = ZBTxRequest(addr64, data, data_len);

    xbee.send(zbtx);
}

