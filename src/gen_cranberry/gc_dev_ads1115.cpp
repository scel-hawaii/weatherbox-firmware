#include "gc_dev_ads1115.h"

static Adafruit_ADS1115 ADS;

void gc_dev_solar_open(void){
    ADS.begin();
}

uint16_t gc_dev_solar_read(void){
    uint16_t value;
    value = ((float)ADS.readADC_SingleEnded(0)*188.0)/(1000.0);
    return value;
}
