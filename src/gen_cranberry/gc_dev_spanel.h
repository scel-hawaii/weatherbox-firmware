#include <Arduino.h>

#define _PIN_GC_SPANEL_ A1

#ifndef GC_DEV_SPANEL
#define GC_DEV_SPANEL
void gc_dev_spanel_open(void);
uint16_t gc_dev_spanel_read_raw(void);
uint16_t gc_dev_spanel_read(void);
#endif
