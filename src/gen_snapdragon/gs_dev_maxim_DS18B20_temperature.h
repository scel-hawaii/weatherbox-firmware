/*******************************
 *
 * File: gs_dev_maxim_DS18B20_temperature.h
 *
 * Contains prototypes to get temperature from DS18B20
 *
 ******************************/

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 3

#ifndef GS_DEV_MAXIM_DS18B20_TEMPERATURE_H
#define GS_DEV_MAXIM_DS18B20_TEMPERATURE_H
void gs_dev_maxim_DS18B20_temperature_open(void);
uint16_t gs_dev_maxim_DS18B20_temperature_read(void);
void gs_dev_maxim_DS18B20_temperature_test(void);
#endif
