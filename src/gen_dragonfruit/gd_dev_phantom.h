/*******************************
 *
 * File: gd_dev_phantom.h
 *
 * Contains prototypes for phantom battery reading functions
 *
 ******************************/

#include <Arduino.h>

#define _PIN_GD_PHANTOM_ A6

#ifndef GD_DEV_PHANTOM_H
#define GD_DEV_PHANTOM_H
void gd_dev_phantom_open(void);
int gd_dev_phantom_read(void);
void gd_dev_phantom_test(void);
#endif
