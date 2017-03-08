/*******************************
 *
 * File: gd_dev_phantom.cpp
 *
 * This module is a driver for checking the phantom battery reading in mV.
 *
 ******************************/

#include "gd_dev_phantom.h"

/******************************
 *
 * Name:        gd_dev_phantom_open
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Initialize battery reading pins
 *
 ******************************/
void gd_dev_phantom_open(void){
    pinMode(_PIN_GD_PHANTOM_, INPUT);
}

/******************************
 *
 * Name:        gd_dev_phantom_read
 * Returns:     Battery reading in mV
 * Parameter:   Nothing
 * Description: Reads battery voltage
 *
 ******************************/
int gd_dev_phantom_read(void){
    int value = 555;

    #ifndef SEN_STUB
    // Function returns the battery reading as an integer in the range from 0 to 1023.
    // Divide by 1023 to offset and then convert to mV.
    value = analogRead(_PIN_GD_PHANTOM_)*5000.0/1023;
    #endif

    return value;
}

/******************************
 *
 * Name:        gd_dev_phantom_test
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Used by the POST function to sample the
 *              sensor and displays the sample to the Serial Monitor
 *
 ******************************/

void gd_dev_phantom_test(void){
    Serial.println(F("[P] Check phantom battery value"));
    uint16_t battery_val = gd_dev_phantom_read();

    Serial.print(F("[P] Phantom battery value: "));
    Serial.print(battery_val);
    Serial.println(F(" mV"));

    if(battery_val < 0){
        Serial.println(F("[P] \tError: battery value out of range"));
    }
}
