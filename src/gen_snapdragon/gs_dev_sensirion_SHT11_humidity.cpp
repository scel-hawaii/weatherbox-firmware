/*******************************
 *
 * File: gs_dev_sensirion_SHT1X_humidity.cpp
 *
 * This module is a driver for the SHT1x sensor. This
 * sensor measures humidity in percentage.
 *
 * Product page: https://www.sensirion.com/products/humidity-sensors/digital-humidity-sensors-for-accurate-measurements/
 * Datasheet: https://www.parallax.com/sites/default/files/downloads/28018-Sensirion-Temerature-Humidity-Sensor-Documentation-v1.0.pdf
 *
 ******************************/

#include "gs_dev_sensirion_SHT11_humidity.h"

#define _PIN_HUMID_CLK 7
#define _PIN_HUMID_DATA 8

static SHT1x sht1x(_PIN_HUMID_DATA, _PIN_HUMID_CLK);

/******************************
 *
 * Name:        gs_dev_sensirion_SHT1X_humidity_open
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Nothing (Used for consistency)
 *
 ******************************/

void gs_dev_sensirion_SHT11_humidity_open(void)
{

}

/******************************
 *
 * Name:        gs_dev_sensirion_SHT1X_humidity_read
 * Returns:     Value of humidity in percentage
 * Parameter:   Nothing
 * Description: Samples the SHT1x to get humidity value
 *
 ******************************/

int gs_dev_sensirion_SHT11_humidity_read(void)
{
    int value = 60;

    #ifndef SEN_STUB
    value =  sht1x.readHumidity();
    #endif

    return value;
}
void gs_dev_sensirion_SHT11_humidity_pct_test(void) {

  int sht11_val = gs_dev_sensirion_SHT11_humidity_read();
  Serial.print(F("[P] sht11 value: "));
  Serial.print(sht11_val);
  Serial.println("\%");

  if(sht11_val < 0){
      Serial.println(F("[P] \tError: Humidity out of range"));
  }
}