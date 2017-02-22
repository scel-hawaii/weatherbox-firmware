#include "ga_dev_gps.h"

//Define true to echo gps data to serial monitor
//Used in GPS ready function
#define GPSECHO  false

/* Creates a tx/rx connection to be used for the GPS */
SoftwareSerial mySerial(_GA_PIN_GPS_TX, _GA_PIN_GPS_RX);
Adafruit_GPS GPS(&mySerial);

/* Enables GPS */
void ga_dev_gps_open(void) {

    pinMode(_GA_PIN_GPS_ENABLE , OUTPUT);

    digitalWrite(_GA_PIN_GPS_ENABLE , HIGH);

    //GPS baud rate should be 9600
    GPS.begin(9600);

    //RMC = Recommended Minimum Data for GPS
    //GGA = Fix Informtation
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

    //Update at 1Hz, Don't use anything higher to allow parsing
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

}

/* Function to turn GPS on or off */
void ga_dev_gps_power(void){
    if(digitalRead(_GA_PIN_GPS_ENABLE)== HIGH){
        digitalWrite(_GA_PIN_GPS_ENABLE, LOW);
        return;
    }

    else {
        digitalWrite(_GA_PIN_GPS_ENABLE, HIGH);
        return;
    }

}

/* Checks if GPS is ready and there is a fix*/
bool ga_dev_gps_ready(void){
    char c = GPS.read();

    if (GPSECHO){
      if (c) Serial.print(c);
    }

    if (GPS.newNMEAreceived()) {
        if (!GPS.parse(GPS.lastNMEA())){
            return false;
        }

        if(GPS.fix) {
            return true;
        }

        else {
            return false;
        }
    }

    return false;

  }

//Sets GPS latitude value
float ga_dev_gps_latitude(void){
      float value = 10075.89;

      #ifndef SEN_STUB
      value = GPS.latitude;
      #endif

      return value;
}

//sets GPS longitude value
float ga_dev_gps_longitude(void){
      float value = 7740.90;
      #ifndef SEN_STUB
        value = GPS.longitude;
      #endif

      return value;
}

//sets altitude value
float ga_dev_gps_altitude(void){
      float value = 203.75;
      #ifndef SEN_STUB
        value = GPS.altitude;
      #endif

      return value;
}
