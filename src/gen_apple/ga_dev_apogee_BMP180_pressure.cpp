/*******************************
 *
 * File: ga_dev_apogee_BM180_pressure.cpp
 *
 * This module is a driver for the pressure and
 * temperature sensor.
 *
 * Product Page: https://www.adafruit.com/product/1603
 * Datasheet: https://cdn-shop.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf
 *
 ******************************/

#include "ga_dev_apogee_BMP180_pressure.h"
static Adafruit_BMP085 bmp180;

/******************************
 *
 * Name:        ga_dev_apogee_BM180_pressure_open
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Initialize BMP180 sensor
 *
 ******************************/

void ga_dev_apogee_BMP180_pressure_open(void){
    bmp180.begin();
}

/******************************
 *
 * Name:        ga_dev_apogee_BMP180_pressure_read
 * Returns:     Pressure in Pascals (Pa)
 * Parameter:   Nothing
 * Description: Read the pressure from the BMP180 sensor
 *
 ******************************/

uint32_t ga_dev_apogee_BMP180_pressure_read(void){
    uint32_t value = 100000;

    #ifndef SEN_STUB
    value = bmp180.readPressure();
    #endif

    return value;
}

void ga_dev_apogee_BMP180_pressure_test(void){
  int32_t bmp085_val = ga_dev_apogee_BMP180_pressure_read();
  Serial.print(F("[P] BMP180 value: "));
  Serial.print(bmp085_val/100);
  Serial.print(F("."));
  Serial.print((bmp085_val-bmp085_val/10)/1000);
  Serial.println(" mb");

  if(bmp085_val < 80000){
      Serial.println(F("[P] \tError: BMP180 pressure out of range"));
  }
}
