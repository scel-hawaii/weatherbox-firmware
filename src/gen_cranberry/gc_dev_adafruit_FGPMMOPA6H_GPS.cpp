/*******************************
 *
 * File: gc_dev_adafruit__GPS.cpp
 *
 * This module is a driver for the FGPMMOPA6H device using
 * the MT3339 Chipset.
 *
 * Product page: https://www.adafruit.com/product/790
 * Datasheet: https://cdn-shop.adafruit.com/datasheets/GlobalTop-FGPMMOPA6H-Datasheet-V0A.pdf
 *
 ******************************/

#include "gc_dev_adafruit_FGPMMOPA6H_GPS.h"



static Adafruit_FGMMOPA6H GPS;

/******************************
 *
 * Name:        gc_dev_adafruit_GPS_open
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Initializes GPS sensor
 *
 ******************************/
void gc_dev_adafruit_GPS_open(void){

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  useInterrupt(true);

}


/******************************
 *
 * Name:        gc_dev_adafruit_longitude
 * Returns:     Longitude value in Degrees
 * Parameter:   Nothing
 * Description: Reads GPS data
 *
 ******************************/

uint8_t gc_dev_adafruit_GPS_longitude(void){

uint8_t value;

#ifndef SEN_STUB
  if (GPS.newNMEAreceived()){
    GPS.parse(GPS.lastNMEA());
  }
      if(GPS.fix){
        value = GPS.longitudeDegrees, 4;
      }
      else{
        Serial.print("Waiting for fix...");
      }
#endif
  return value;
}

/******************************
 *
 * Name:        gc_dev_adafruit_latitude_read
 * Returns:     Latitude value in Degrees
 * Parameter:   Nothing
 * Description: Reads GPS data
 *
 ******************************/

uint8_t gc_dev_adafruit_GPS_latitude(void){

uint8_t value;

#ifndef SEN_STUB
   if (GPS.newNMEAreceived()){
     GPS.parse(GPS.lastNMEA());
  }
      if(GPS.fix){
        value = GPS.latitudeDegrees;
      }
      else{
        Serial.print("Waiting for fix...");
      }
#endif

return value;
}
/******************************
 *
 * Name:        gc_dev_adafruit_altitude_read
 * Returns:     Altitude value in meters
 * Parameter:   Nothing
 * Description: Reads GPS data
 *
 ******************************/

uint8_t gc_dev_adafruit_GPS_altitude(void){

uint8_t value;

#ifndef SEN_STUB
     if (GPS.newNMEAreceived()){
       GPS.parse(GPS.lastNMEA());
     }
       if(GPS.fix){
    //Converting GPS altitude in CM to M
        value = GPS.altitude / 100;
      }
      else{
        Serial.print("Waiting for fix...");
      }
#endif
  return value;
}
/******************************
 *
 * Name:        gc_dev_adafruit_GPS_test
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Used by the POST function to sample the
 *              sensor and displays the sample to Serial Monitor
 *
 ******************************/

void gc_dev_adafruit_GPS_test(void){

      Serial.println(F("[GPS] Check fgpmmopa6h_gps"))
      if (GPS.newNMEAreceived()){
        GPS.parse(GPS.lastNMEA());
      }
        if(GPS.fix){
        Serial.print("Latitude:");
          Serial.print(GPS.latitudeDegrees, 4);
          Serial.print("Longitude:");
          Serial.println(GPS.longitudeDegrees, 4);
          Serial.print("altitude:");
           Serial.println(GPS.altitude);
         }
         else{
         Serial.print("Waiting for fix...");
        }
}
