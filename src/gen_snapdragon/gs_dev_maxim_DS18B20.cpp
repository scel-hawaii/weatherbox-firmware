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

void gg_dev_maxim_DS18B20_temperature_open(void) {
  sensors.begin();
}

uint16_t gg_dev_maxim_DS18B20_temperature_read(void) {
  int value = 100;

  #ifndef SEN_STUB
   value = DallasTemperature::toFahrenheit(sensors.requestTemperatures());
  #endif

  return value;
}
