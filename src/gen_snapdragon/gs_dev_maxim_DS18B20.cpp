/*******************************
 *
 * File: gs_dev_maxim_DS18B20_temperature.cpp
 *
 * This module is a driver for the DS18B20 sensor that
 * will measure temperature. (Some relevant info here)
 *
 * Product Page: https://www.adafruit.com/product/374
 * Datasheet: https://cdn-shop.adafruit.com/datasheets/DS18B20.pdf
 *
 ******************************/

#include "gs_dev_maxim_DS18B20_temperature.h"
//DS18B20 uses a unique 1-wire interface for communication
/* IMPORTANT: The communication protocol must be the same in all drivers */
//#define I2C //Define communication protocol as seen below

/*#ifdef HardwareSPI
Adafruit_BME280 bme_temperature(BME_CS); // Hardware SPI
#endif

#ifdef SoftwareSPI
Adafruit_BME280 bme_temperature(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // Software SPI
#endif
*/

void gs_dev_maxim_DS18B20_temperature_open(void) {
  sensors.begin();
}

uint16_t gs_dev_maxim_DS18B20_temperature_read(void) {
  int value = 100;

  #ifndef SEN_STUB
   value = DallasTemperature::toFahrenheit(sensors.requestTemperatures());
  #endif

  return value;
}

void gs_dev_maxim_DS18B20_temperature_test(void) {

    Serial.println(F("[P] Check DS18B20_temperature_test value"));
    int DS18B20_temperature_val = gs_dev_maxim_DS18B20_temperature_read();

    Serial.print(F("[P] DS18B20_temperature value: "));
    Serial.print(DS18B20_temperature_val);
    Serial.println(F(" cF"));

    if(DS18B20_temperature_val < 0){
        Serial.println(F("[P] \tError: DS18B20 temp out of range"));
    }
}
