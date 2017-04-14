/*******************************
 *
 * File: gg_dev_battery.cpp
 *
 * This module is a driver for checking the battery reading in mV.
 *
 ******************************/

#include "gg_dev_battery.h"

/******************************
 *
 * Name:        gg_dev_battery_open
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Initialize battery reading pins
 *
 ******************************/

void gg_dev_battery_open(void){
    pinMode(_PIN_GG_BATTERY_, INPUT);
}

/******************************
 *
 * Name:        gg_dev_battery_read
 * Returns:     Battery reading in mV
 * Parameter:   Nothing
 * Description: Reads battery voltage with conversion
 *
 ******************************/

int gg_dev_battery_read(void){
    int val = 555;

    #ifndef SEN_STUB
    float raw = (float)analogRead(_PIN_GG_BATTERY_) * (5.0/1023.0);
    val = raw * 1000;
    #endif

    return val;
}

void gg_dev_battery_test(void){
  int batt_val = gg_dev_battery_read();
  Serial.print(F("[P] battery value: "));
  Serial.print(batt_val);
  Serial.println(" mV");

  if(batt_val < 0){
      Serial.println(F("[P] \tError: battery out of range"));
  }
}
