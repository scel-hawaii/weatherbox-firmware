/*******************************
 *
 * File: gd_dev_apogee_SP215_irradiance.h
 *
 * Contains prototypes for solar irradiance functions
 *
 ******************************/

#include <Arduino.h>
#include <Wire.h>

#define _DEV_ADDR_GS_ADS1100_ 0x48

#ifndef GS_DEV_APOGEE_SP215_IRRADIANCE_H
#define GS_DEV_APOGEE_SP215_IRRADIANCE_H
void gs_dev_apogee_SP215_irradiance_open(void);
uint32_t gs_dev_apogee_SP215_irradiance_read(void);
void gs_dev_apogee_SP215_irradiance_test(void);
#endif
