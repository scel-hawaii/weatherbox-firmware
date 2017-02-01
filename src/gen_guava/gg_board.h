#include "gg_dev_xbee.h"
#include "gg_dev_sht1x.h"
#include "gg_dev_bmp085.h"
#include "gg_dev_apogee_sp212.h"
#include "gg_dev_batt.h"
#include "gg_dev_spanel.h"
#include "gg_dev_eeprom_naddr.h"

#ifndef gg_BOARD_H
#define gg_BOARD_H

struct gg_packet{
    uint16_t schema;
    uint16_t node_addr;             // Address of Arduino
    uint32_t uptime_ms;             // Time since start of program
    uint16_t batt_mv;               // Battery Voltage (in milli volts)
    uint16_t panel_mv;              // Panel Voltage (in milli volts)
    uint32_t bmp085_press_pa;       // Pressure Value (in pascals)
    int16_t bmp085_temp_decic;      // Temperature Value (in celsius)
    uint16_t humidity_centi_pct;
    uint16_t apogee_w_m2;
};

struct gg_heartbeat_packet{
    uint16_t schema;
    uint16_t node_addr;             // Address of Arduino
    uint32_t uptime_ms;             // Time since start of program
    uint16_t batt_mv;               // Battery Voltage (in milli volts)
};


// Leggcy apple schema.
typedef struct {
    uint16_t schema;
    uint16_t address;               // Address of Arduino
    uint8_t overflow_num;           // Number of times millis overflowed (happens ~every 49 days)
    uint32_t uptime_ms;             // Time since start of program
    uint8_t n;                      // number of data points in packet 0..30
    uint16_t batt_mv[6];            // Battery Voltage (in milli volts)
    uint16_t panel_mv[6];           // Panel Voltage (in milli volts)
    uint32_t bmp085_press_pa;       // Pressure Value (in pascals)
    int16_t bmp085_temp_decic;      // Temperature Value (in celsius)
    uint16_t humidity_centi_pct;
    uint16_t apogee_w_m2[20];
} schema_3;


struct gg_board{
    void (*setup)(struct gg_board* b);
    void (*post)(void);
    void (*sample)(struct gg_board* b);
    void (*run_cmd)(struct gg_board* b);
    void (*print_build_opts)(void);
    void (*tx)(struct gg_board* b);
    int (*ready_tx)(struct gg_board* b);
    int (*ready_sample)(struct gg_board* b);
    int (*ready_run_cmd)(struct gg_board* b);

    int (*ready_heartbeat_tx)(struct gg_board* b);
    void (*heartbeat_tx)(struct gg_board* b);

    unsigned long prev_sample_ms;
    unsigned long prev_heartbeat_ms;
    int sample_count;
    uint16_t node_addr;
    struct gg_packet data_packet;
};


void gg_board_init(struct gg_board*);

#endif
