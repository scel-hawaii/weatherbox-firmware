#include "gd_dev_spanel.h"

void gd_dev_spanel_open(void){
    pinMode(_PIN_GD_SPANEL_, INPUT);
}

int gd_dev_spanel_read(void){
    float value = 555.0;
    #ifndef SEN_STUB
    /* Physical voltage divider implemented to prevent saturation of signal. Solar panel has a maximum output voltage of 6 V but ATmega328P has maximum input of 5 V. Use a scaling factor of 2 to account for voltage divider. */
    value = 2.0 * (float)analogRead(_PIN_GD_SPANEL_) *(5000.0/1023);
    #endif
    return value;
}

