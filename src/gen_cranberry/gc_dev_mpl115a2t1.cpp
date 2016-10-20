#include "gc_dev_mpl115a2t1.h"
#include "Adafruit_MPL115A2.h"

static Adafruit_MPL115A2 mpl115a2;

void gc_dev_mpl115a2t1_open(void){
    mpl115a2.begin();
}

uint32_t gc_dev_mpl115a2t1_press_pa_read(void){
    uint32_t value = 100000;

    #ifndef SEN_STUB
    value = mpl115a2.getPressure() * 1000;
    #endif

    return value;
}
