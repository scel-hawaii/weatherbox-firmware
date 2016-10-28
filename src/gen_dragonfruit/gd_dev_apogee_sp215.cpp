#include "gd_dev_apogee_sp215.h"

void gd_dev_apogee_sp215_open(void){
    
    /* Initiate Wire library and join I2C bus as slave */
    Wire.begin(0x48);
    
    /* Enable ready-bit and select 16-bit sample rate */
    Wire.write(0x8C);

}

uint32_t gd_dev_apogee_sp215_read(void){
    uint32_t value = 555;
    #ifndef SEN_STUB

    /* Begin transmission to the I2C slave device with the given address */
    Wire.beginTransmission(0x48);

    /* Communicate through I2C */
    Wire.requestFrom(_PIN_GD_APOGEE_SP215_, 3);
    if(Wire.available())
    {
        value = Wire.read();
        value = value << 8;
        value += Wire.read();
    }
    Wire.endTransmission();
    
    /* Analog to digital conversion with 16 bit-resolution. Multiply by 5V reference voltage then divide by 0xFFFF to convert the 16-bit ADC reading to voltage. The Apogee SP215 sensor has a maximum output voltage of 5V but the MCP3425 ADC converter only allows a max input reading of 2.048V. To prevent a saturated signal reading, A physical voltage divider over R1 (.973 kohm) and R2 (2.167 kohm) is used to decrease the signal amplitude from the Apogee SP215. To account for this voltage divider, a scaling factor of 3.227 is used. */
    //value = (3.227 * (value*5000.00)/(0xFFFF)) * 1000; //Convert to microvolts
    value = (value*5000.0)/(0x7FFF);
    #endif
    return value;
}
