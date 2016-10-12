#include "gd_dev_apogee_sp215.h"

void gd_dev_apogee_sp215_open(void){
    Wire.begin(9600);
}

int gd_dev_apogee_sp215_read(void){
    int value = 555;
    #ifndef SEN_STUB
    Wire.beginTransmission(_PIN_GD_APOGEE_SP215_);

    /* Options */
    Wire.write(0x8C);

    /* Communicate through I2C */
    Wire.requestFrom(_PIN_GD_APOGEE_SP215_, 3);
    if(Wire.available())
    {
        value = Wire.read();
        value = value << 8;
        value += Wire.read();
    }
    Wire.endTransmission();
    /* Value received from Wire.read() returns (solar irridiance value)^8, multiply by five volts (5.0x10^3), then divide by 0x7FFF (2^15) to obtain mV. */
    value = (value*5000.00)/(0x7FFF);
    #endif
    return value;
}
