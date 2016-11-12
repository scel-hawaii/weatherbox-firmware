#include "ga_dev_gps.h"

#define GPSECHO  false //Define true to echo gps data to serial monitor

SoftwareSerial mySerial(_PIN_GPS_TX, _PIN_GPS_RX);
Adafruit_GPS GPS(&mySerial);

void ga_dev_gps_open(void) {

    pinMode(_PIN_GPS_ENABLE , OUTPUT);
    //Enable pin needs to be pulled low;
    digitalWrite(_PIN_GPS_ENABLE , HIGH);

    //GPS baud rate should be 9600
    GPS.begin(9600);

    //RMC = Recommended Minimum Data for GPS
    //GGA = Fix Informtation
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

    //Update at 1Hz, Don't use anything higher to allow parsing
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

}

void ga_dev_gps_high(void){
    if(digitalRead(_PIN_GPS_ENABLE) == HIGH){
      return;
    }
    digitalWrite(_PIN_GPS_ENABLE , HIGH);
}

void ga_dev_gps_low(void){
    digitalWrite(_PIN_GPS_ENABLE , LOW);
}

bool ga_dev_gps_ready(void){
    char c = GPS.read();

    if (GPSECHO){
      if (c) Serial.print(c);
    }

    if (GPS.newNMEAreceived()) {
        if (!GPS.parse(GPS.lastNMEA())){
            return false;
        }
    }

    if(GPS.fix) {
        return true;
    }

    else {
        return false;
    }

  }

float ga_dev_gps_latitude(void){
      float value = 10075.89;

      #ifndef SEN_STUB
      value = GPS.latitude;
      #endif

      return value;
}

float ga_dev_gps_longitude(void){
      float value = 7740.90;
      #ifndef SEN_STUB
        value = GPS.longitude;
      #endif

      return value;
}

float ga_dev_gps_altitude(void){
      float value = 203.75;
      #ifndef SEN_STUB
        value = GPS.altitude;
      #endif

      return value;
}
