#include "gc_dev_batt.h"

static Adafruit_ADS1115 adc;

void gc_dev_batt_open(void){
    adc.begin();
}

uint16_t gc_dev_batt_read(void){
    uint16_t value = 4000;

    #ifndef SEN_STUB
    /* Scaling factor of 2.0 is used because of the voltage divider
    placed in the circuit (Note: Cranberry v3.5.0 Schematic incorrect) */
    value = 2.0*((float)adc.readADC_SingleEnded(2)*188.0)/(1000.0);
    #endif

    return value;
}
