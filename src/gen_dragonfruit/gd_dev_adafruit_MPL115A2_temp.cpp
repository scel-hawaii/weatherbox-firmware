/*******************************
 *
 * File: gd_dev_adafruit_MPL115A2_temp.cpp 
 *
 * This module is a driver for the temperature sensor. It uses the
 * Adafruit MPL115A2 sensor to measure temperature in cK.
 *
 ******************************/

#include "gd_dev_adafruit_MPL115A2_temp.h"
#include "Adafruit_MPL115A2.h"

static Adafruit_MPL115A2 mpl115a2t1;

/******************************
 * 
 * Name:        gd_dev_adafruit_MPL115A2_temp_open
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Initialize temperature sensor 
 * 
 ******************************/
void gd_dev_adafruit_MPL115A2_temp_open(void){
    mpl115a2t1.begin();
}

/******************************
 * 
 * Name:        gd_dev_adafruit_MPL115A2_temp_read
 * Returns:     Temperature value in centiKelvin (cK) 
 * Parameter:   Nothing
 * Description: Reads temperature sensor 
 * 
 ******************************/
uint16_t gd_dev_adafruit_MPL115A2_temp_read(void){
  uint16_t value = 0;
  float raw_value;
  #ifndef SEN_STUB
  raw_value = mpl115a2t1.getTemperature(); //Function returns floating point value in Celcius
  value = ((raw_value + 273.15) * 100); //Convert to centiKelvin (cK)
  #endif
  return (uint16_t)value;
}
