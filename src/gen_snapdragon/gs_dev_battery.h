/*******************************
 *
 * File: gs_dev_battery.h
 *
 * Contains prototypes for battery reading functions
 *
 ******************************/

#include <Arduino.h>

#define _PIN_GS_BATTERY_ A3

#ifndef GS_DEV_BATTERY_H
#define GS_DEV_BATTERY_H
void gs_dev_battery_open(void);
int gs_dev_battery_read(void);
void gs_dev_battery_test(void);
#endif
