#include <Arduino.h>
#include <Adafruit_GPS.h>
//#include <TinyGPS++.h> Secondary GPS Library, maybe to implemeneted later
#include <SoftwareSerial.h>

#define _GA_PIN_GPS_RX 4
#define _GA_PIN_GPS_TX 5
#define _GA_PIN_GPS_ENABLE 10
#define _GPS_POST_TEST 0

#ifndef GA_DEV_GPS_H
#define GA_DEV_GPS_H
void ga_dev_gps_open(void);
void ga_dev_gps_power(void);
bool ga_dev_gps_ready(void);

float ga_dev_gps_latitude(void);
float ga_dev_gps_longitude(void);
float ga_dev_gps_altitude(void);

#endif
