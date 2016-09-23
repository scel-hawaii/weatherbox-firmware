#include "gd_dev_apogee_sp215.h"

void gd_dev_apogee_sp215_open(void){
    pinMode(_PIN_GD_APOGEE_SP215_, INPUT);
}

int gd_dev_apogee_sp215_read(void){
    int value = 555;
    #ifndef SEN_STUB
    value = analogRead(_PIN_GD_APOGEE_SP215_);
    #endif
    return value;
}
