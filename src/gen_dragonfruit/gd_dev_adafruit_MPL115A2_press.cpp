/*******************************
 *
 * File: gd_dev_adafruit_MPL115A2_press.cpp
 *
 * This module is a driver for the pressure sensor. It uses the Adafruit 
 * MPL115A2 sensor to measure pressure in Pa.
 * 
 ******************************/

#include "gd_dev_adafruit_MPL115A2_press.h"
#include "Adafruit_MPL115A2.h"

static Adafruit_MPL115A2 mpl115a2t1_press;

/******************************
 * 
 * Name:        gd_dev_adafruit_MPL115A2_press_open
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Initialize pressure sensor
 * 
 ******************************/
void gd_dev_adafruit_MPL115A2_press_open(void){
    mpl115a2t1_press.begin();
}

/******************************
 * 
 * Name:        gd_dev_adafruit_MPL115A2_press_read
 * Returns:     Pressure value in Pa 
 * Parameter:   Nothing
 * Description: Reads pressure sensor 
 * 
 ******************************/
uint32_t gd_dev_adafruit_MPL115A2_press_read(void){
  float value = 88;
  #ifndef SEN_STUB
  /* getPressure returns pressure value in kPa.
     Multiply by 1000 to convert to Pa. */
  value = mpl115a2t1_press.getPressure()*1000; 
  #endif
  return (uint32_t)value;
}
