/*******************************
 *
 * File: gd_dev_adafruit_MPL115A2_temperature.h
 *
 * Contains prototypes for temperature functions
 *
 ******************************/

#include "Adafruit_MPL115A2.h"

#ifndef _GS_ADAFRUIT_MPL115A2_TEMPERATURE_H
#define _GS_ADAFRUIT_MPL115A2_TEMPERATURE_H
void gs_dev_adafruit_MPL115A2_temperature_open(void);
uint16_t gs_dev_adafruit_MPL115A2_temperature_centik_read(void);
void gs_dev_adafruit_MPL115A2_temperature_centik_test(void);
#endif
