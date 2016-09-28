#include "gd_dev_honeywell_HIH6131.h"

void gd_dev_honeywell_HIH6131_open(void)
{
    Wire.begin(9600);
}
int gd_dev_honeywell_HIH6131_read(void)
{
    int value = 555;
    #ifndef SEN_STUB
    hih6131.update();
    value = hih6131.getHumidity();
    #endif
    return value;
}
