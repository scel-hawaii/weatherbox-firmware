/*******************************
 *
 * File: gg_dev_apogee_SP212_irradiance.cpp
 *
 * This module is a driver for the solar irradiance sensor. It uses the
 * Apogee sp215 analog sensor to measure solar irradiance in millivolts (mV) and the ADS1100
 * ADC converter to convert the Apogee sp215 reading into a digital value.
 *
 * Product page: http://www.apogeeinstruments.com/sp-215-amplified-0-5-volt-pyranometer/
 * Datasheet: http://www.apogeeinstruments.com/content/SP-100-200-spec-sheet.pdf
 *
 * Product page: http://www.ti.com/product/ADS1100
 * Datasheet: http://www.ti.com/lit/ds/symlink/ads1100.pdf
 *
 * ****************************/

#include "gg_dev_apogee_SP212_irradiance.h"

/******************************
 *
 * Name:        gg_dev_apogee_SP212_irradiance_open
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Initialize solar irradiance sensor
 *
 ******************************/

void gg_dev_apogee_SP212_irradiance_open(void){
    pinMode(_PIN_GG_APOGEE_SP212_IRRADIANCE_, INPUT);
}

/******************************
 *
 * Name:        gg_dev_apogee_SP212_irradiance_read
 * Returns:     Solar irradiance value in millivolts (mV)
 * Parameter:   Nothing
 * Description: Reads solar irradiance sensor with conversion
 *
 ******************************/

int gg_dev_apogee_SP212_irradiance_read(void){
    int value = 555;

    #ifndef SEN_STUB
    value = (float)analogRead(_PIN_GG_APOGEE_SP212_IRRADIANCE_)*(5000.0/1023.0);
    #endif

    return value;
}

void gg_dev_apogee_SP212_irradiance_test(void){
  int apogee_sp212_val = gg_dev_apogee_SP212_irradiance_read();
  Serial.print(F("[P] apogee_sp212 solar irradiance value: "));
  Serial.print(apogee_sp212_val);
  Serial.println(" mV");

  if(apogee_sp212_val < 0){
      Serial.println(F("[P] \tError: apogee solar irradiance out of range"));
  }
}
