#include <Arduino.h>

#define _PIN_GC_BATT_ A3

#ifndef GC_DEV_BATT_H
#define GC_DEV_BATT_H
void gc_dev_batt_open(void);
int gc_dev_batt_read_raw(void);
int gc_dev_batt_read(void);
#endif