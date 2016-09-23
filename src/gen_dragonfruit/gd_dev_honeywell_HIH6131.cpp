#include "gd_dev_honeywell_HIH6131.h"

void gd_dev_honeywell_HIH6131_open(void)
{
    pinMode(_PIN_GD_HONEYWELL_HIH6131_, INPUT);
}
int gd_dev_honeywell_HIH6131_read(void)
{
    int value = 555;
    #ifndef SEN_STUB
    value = analogRead(_PIN_GD_HONEYWELL_HIH6131_);
    #endif
    return value;
}
