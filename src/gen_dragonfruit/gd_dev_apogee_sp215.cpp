#include "gd_dev_apogee_sp215.h"

void gd_dev_apogee_sp215_open(void){

    /* Initiate Wire library and join I2C bus as slave */
    Wire.begin();

    /* Begin transmission to the I2C slave device with the given address */
    Wire.beginTransmission(_DEV_ADDR_GD_APOGEE_SP215_);

    /* Enable read bit and select 16-bit sample rate and select sampling rate
    of 8 SPS. */
    Wire.write(0x8C);

    Wire.endTransmission();
}

uint32_t gd_dev_apogee_sp215_read(void){
    uint32_t value = 555;
    #ifndef SEN_STUB

    /* Begin transmission to the I2C slave device with the given address */
    Wire.beginTransmission(_DEV_ADDR_GD_APOGEE_SP215_);

    /* Communicate through I2C */
    Wire.requestFrom(_DEV_ADDR_GD_APOGEE_SP215_, 3);
    if(Wire.available())
    {
        value = Wire.read();
        value = value << 8;
        value += Wire.read();
    }
    Wire.endTransmission();

    /* Analog to digital conversion with 16 bit-resolution. Multiply by 5V
    reference voltage then divide by 0x7FFF to convert the 16-bit ADC reading
    to voltage. */
    value = (value*5000.0)/(0x7FFF);
    #endif
    return value;
}
