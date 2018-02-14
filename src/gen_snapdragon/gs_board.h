/*******************************
 *
 * File: gs_board.h
 *
 * Contains struct for Snapdragon packet, heartbeat, and board
 *
 ******************************/

#include "gs_dev_digi_xbee.h"
#include "gs_dev_apogee_SP215_irradiance.h"
#include "gs_dev_battery.h"
#include "gs_dev_solar_panel.h"
#include "gs_dev_eeprom_node_address.h"
#include "gs_dev_maxim_DS18B20.h"
#include "gs_dev_sensirion_SHT1X_humidity.h"
#include "gs_dev_adafruit_MPL115A2_pressure.h"

#ifndef gs_BOARD_H
#define gs_BOARD_H

struct gs_packet{
    uint16_t schema;
    uint16_t node_address;                             // Address of Arduino
    uint32_t uptime_milliseconds;                       // Time since start of program
    uint16_t battery_millivolts;                        // Battery Voltage (in milli volts)
    uint16_t panel_millivolts;                          // Panel Voltage (in milli volts)
    uint32_t mpl115a2t1_pressure_pascals;                  // Pressure Value (in pascals)
    uint16_t DS18B20_temperature_kelvin;                // Temperature Value (in Kelvin)
    uint16_t SHT1X_humidity_percent;                  // Humidity Value (in percentage)
    uint16_t sp215_irradiance_watts_per_square_meter;  // Solar Irradiance Value (in W/m^2)
};

struct gs_heartbeat_packet{
    uint16_t schema;
    uint16_t node_address;       // Address of Arduino
    uint32_t uptime_milliseconds;       // Time since start of program
    uint16_t battery_millivolts;         // Battery Voltage (in milli volts)
};

struct gs_board{
    void (*setup)(struct gs_board* b);
    void (*post)(void);
    void (*sample)(struct gs_board* b);
    void (*run_cmd)(struct gs_board* b);
    void (*print_build_opts)(void);
    void (*tx)(struct gs_board* b);
    int (*ready_tx)(struct gs_board* b);
    int (*ready_sample)(struct gs_board* b);
    int (*ready_run_cmd)(struct gs_board* b);

    int (*ready_heartbeat_tx)(struct gs_board* b);
    void (*heartbeat_tx)(struct gs_board* b);

    unsigned long prev_sample_ms;
    unsigned long prev_heartbeat_ms;
    int sample_count;
    uint16_t node_address;
    struct gs_packet data_packet;
};


void gs_board_init(struct gs_board*);

#endif
