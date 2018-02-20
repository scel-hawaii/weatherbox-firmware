/*******************************
 *
 * File: gd_dev_adafruit_MPL115A2_temperature.h
 *
 * Contains prototypes for temperature functions
 *
 ******************************/

#include "Adafruit_MPL115A2.h"

#ifndef _GS_ADAFRUIT_MPL115A2_PRESSURE_H
#define _GS_ADAFRUIT_MPL115A2_PRESSURE_H
void gs_dev_adafruit_MPL115A2_pressure_open(void);
uint32_t gs_dev_adafruit_MPL115A2_pressure_pa_read(void);
void gs_dev_adafruit_MPL115A2_pressure_pa_test(void);
#endif
