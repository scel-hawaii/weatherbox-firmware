#include "gd_dev_adafruit_MPL115A2.h"
#include "Adafruit_MPL115A2.h"

static Adafruit_MPL115A2 mpl115a2t1;

void gd_dev_adafruit_MPL115A2_open(void){
    mpl115a2t1.begin();
}

int gd_dev_adafruit_MPL115A2_press_kpa_read(void){
    uint16_t value = 80;

    #ifndef SEN_STUB
    value = mpl115a2t1.getPressure();
    #endif

    return value;
}
