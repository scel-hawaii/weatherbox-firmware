/*******************************
 *
 * File: gc_dev_adafruit_FGPMMOPA6H_GPS.h
 *
 * Contains prototypes for GPS sensor functions
 *
 ******************************/

#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <NewSoftSerial.h>


#ifndef GC_DEV_FGPMMOPA6H
#define GC_DEV_FGPMMOPA6H

void gc_dev_adafruit_GPS_open(void);
uint8_t gc_dev_adafruit_GPS_longitude(void);
uint8_t gc_dev_adafruit_GPS_latitude(void);
uint8_t gc_dev_adafruit_GPS_altitude(void);
void gc_dev_adafruit_GPS_test(void);

SoftwareSerial mySerial(3, 2); /* Pins for arduino */5
