#include "gd_dev_apogee_sp215.h"

void gd_dev_apogee_sp215_open(void){
    Wire.begin(9600);
}

uint32_t gd_dev_apogee_sp215_read(void){
    uint32_t value = 555;
    #ifndef SEN_STUB
    Wire.beginTransmission(_PIN_GD_APOGEE_SP215_);

    /* Enable ready-bit and select 16-bit sample rate */
    Wire.write(0x88);

    /* Communicate through I2C */
    Wire.requestFrom(_PIN_GD_APOGEE_SP215_, 3);
    if(Wire.available())
    {
        value = Wire.read();
        value = value << 8;
        value += Wire.read();
    }
    Wire.endTransmission();
    /* Analog to digital conversion with 16 bit-resolution */   
    /* Divide by 0xFFFF to convert 16-bit ADC reading, 5 V is reference voltage */
    /* Physical voltage divider implemented to prevent saturation of signal. Apogee SP215 has a maximum output voltage of 5V but the MCP3425 only allows a max input of 2.048V. Voltage divider over .973kohm and 2.167kohm resistor used to decrease amplitude of signal from Apogee. Use a scaling factor of 3.22713257965 to account for this voltage divider and calculate the value of the original signal. */
    value = (3.227 * (value*5000.00)/(0xFFFF)) * 1000; //Convert to microvolts
    #endif
    return value;
}
