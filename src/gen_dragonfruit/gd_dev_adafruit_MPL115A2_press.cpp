#include "gd_dev_adafruit_MPL115A2_press.h"
#include "Adafruit_MPL115A2.h"

static Adafruit_MPL115A2 mpl115a2t1_press;

void gd_dev_adafruit_MPL115A2_press_open(void){
    mpl115a2t1_press.begin();
}

int gd_dev_adafruit_MPL115A2_press_read(void){
    uint16_t value = 80;

    #ifndef SEN_STUB
    value = mpl115a2t1_press.getPressure();
    #endif

    return value;
}
