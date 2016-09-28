#include "HIH613x.h"

#define _PIN_GD_HONEYWELL_HIH6131_ 0x27

#ifndef _PIN_GD_HONEYWELL_HIH6131_H
#define _PIN_GD_HONEYWELL_HIH6131_H

HIH613x hih6131(_PIN_GD_HONEYWELL_HIH6131_);
void gd_dev_honeywell_HIH6131_open(void);
int gd_dev_honeywell_HIH6131_read(void);
#endif
