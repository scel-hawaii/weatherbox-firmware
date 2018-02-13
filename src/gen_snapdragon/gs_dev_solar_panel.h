/*******************************
 *
 * File: gs_dev_solar_panel.h
 *
 * Contains prototypes for solar panel functions
 *
 ******************************/

#include <Arduino.h>

#define _PIN_GS_SOLAR_PANEL_ A1

#ifndef GS_DEV_SOLAR_PANEL
#define GS_DEV_SOLAR_PANEL
void gs_dev_solar_panel_open(void);
int gs_dev_solar_panel_read(void);
#endif
