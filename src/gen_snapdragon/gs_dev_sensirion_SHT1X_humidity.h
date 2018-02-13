/*******************************
 *
 * File: gs_dev_sht1x.h
 *
 * Contains prototypes for SHT1x sensor functions
 *
 ******************************/

#include <Arduino.h>
#include <SHT1x.h>

#ifndef GS_DEV_SENSIRION_SHT1X_HUMIDITY_H
#define GS_DEV_SENSIRION_SHT1X_HUMIDITY_H
void gs_dev_sensirion_SHT1X_humidity_open(void);
int gs_dev_sensirion_SHT1X_humidity_read(void);
#endif
