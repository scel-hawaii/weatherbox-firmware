/*******************************
 *
 * File: gs_board.cpp
 *
 * Contains definitions for board initialization, PlatformIO POST
 * commands, and sampling sensors.
 *
 * ****************************/

#include "gs_board.h"

static void gs_board_print_build_opts();
static void gs_board_setup(struct gs_board* b);
static void gs_board_post();

static void gs_board_run_cmd(struct gs_board* b);
static int gs_board_ready_run_cmd(struct gs_board* b);

static void gs_board_sample(struct gs_board* b);
static int gs_board_ready_sample(struct gs_board* b);

static void gs_board_tx(struct gs_board* b);
static int gs_board_ready_tx(struct gs_board* b);

static int gs_board_ready_heartbeat_tx(struct gs_board* b);
static void gs_board_heartbeat_tx(struct gs_board* b);

/******************************
 *
 * Name:        gs_board_init
 * Returns:     Nothing
 * Parameter:   Function pointer to struct gs_board
 * Description: Initialize Dragonfruit board
 *
 ******************************/

void gs_board_init(gs_board *b){
    // Link functions to make them accessable
    b->print_build_opts = &gs_board_print_build_opts;
    b->setup = &gs_board_setup;
    b->post = &gs_board_post;

    // Sample and TX module
    b->tx = &gs_board_tx;
    b->ready_tx = &gs_board_ready_tx;
    b->sample = &gs_board_sample;
    b->ready_sample = &gs_board_ready_sample;

    // CMD Parsing Module
    b->run_cmd = &gs_board_run_cmd;
    b->ready_run_cmd = &gs_board_ready_run_cmd;

    // Heartbeat Module
    b->ready_heartbeat_tx = &gs_board_ready_heartbeat_tx;
    b->heartbeat_tx = &gs_board_heartbeat_tx;

    // State Variables
    b->sample_count = 0;
    b->node_address = 0;
    b->prev_sample_ms = 0;

    // Initialize the packet
    b->data_packet.schema = 3;
    b->data_packet.node_address = gs_dev_eeprom_node_address_read();
    b->data_packet.uptime_milliseconds = 0;
    b->data_packet.battery_millivolts = 0;
    b->data_packet.panel_millivolts = 0;
    b->data_packet.sp215_irradiance_watts_per_square_meter = 0;
    b->data_packet.mpl115a2t1_temperature_kelvin = 0;
    b->data_packet.hih6131_humidity_percent = 0;
    b->data_packet.mpl115a2t1_pressure_pascals = 0;
}

/******************************
 *
 * Name:        gs_board_print_build_opts
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Initialize board generation and baudrate
 *
 ******************************/

static void gs_board_print_build_opts()
{
    Serial.begin(9600);
    Serial.println(F("Board Opts"));
    Serial.println(F("Gen: snapdragon"));
}

/******************************
 *
 * Name:        gs_board_setup
 * Returns:     Nothing
 * Parameter:   Function pointer to struct gs_board
 * Description: Enable sensor pin, initialize sensors,
 *              obtain node address from eeprom
 *
 ******************************/

static void gs_board_setup(struct gs_board* b){
    Serial.begin(9600);
    Serial.println(F("Board Setup Start"));

    //Sensor On/Off, sets enable pin HIGH
    digitalWrite(_PIN_SEN_EN_, HIGH);

    //Open Devices
    gs_dev_sensirion_SHT11_humidity_open();
    gs_dev_nxpusa_MPL115A2T1_pressure_open();
    gs_dev_maxim_DS18B20_temperature_open();
    gs_dev_apogee_SP215_irradiance_open();
    gs_dev_digi_xbee_open();
    gs_dev_battery_open();
    gs_dev_solar_panel_open();
    gs_dev_eeprom_node_address_open();

    // Load the address from the hardware
    b->node_address = gs_dev_eeprom_node_address_read();

    delay(100);
    Serial.println(F("Board Setup Done"));
}

/******************************
 *
 * Name:        gs_board_post
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Power on self test when board initially starts
 *              and poll each sensor. Also used to check
 *              sensor values on serial monitor.
 *
 ******************************/

static void gs_board_post(){
     Serial.println(F("POST Begin"));

     //Display node address
     gs_dev_eeprom_node_address_test();

     //Check MPL115A2T1 Temperature
     gs_dev_maxim_DS18B20_temperature_centik_test();

     //Check SHT11 Humidity
     gs_dev_sensirion_SHT11_humidity_pct_test();

     //Check MPL115A2T1 Pressure
     gs_dev_nxpusa_MPL115A2T1_pressure_pa_test();

     //Check apogee_sp215
     gs_dev_apogee_SP215_irradiance_test();

     //Check battery Voltage
     gs_dev_battery_test();

     //Check panel sensor values
     gs_dev_solar_panel_test();

     Serial.println(F("POST End"));
}

/******************************
 *
 * Name:        gs_board_sample
 * Returns:     Nothing
 * Parameter:   Function pointer to struct gs-board
 * Description: Sample each sensor and store into data packet
 *
 ******************************/

static void gs_board_sample(struct gs_board* b){
    Serial.print("[");
    Serial.print(millis());
    Serial.print("] ");
    Serial.println(F("Sample Start"));
    // Disabled this for apple deployment on 2016-10-06 with T=30s
    // Serial.println(b->sample_count);

    struct gs_packet* data_packet = &(b->data_packet);
    data_packet->uptime_milliseconds                      = millis();
    data_packet->battery_millivolts                       = gs_dev_battery_read();
    data_packet->panel_millivolts                         = gs_dev_solar_panel_read();
    data_packet->SP215_irradiance_watts_per_square_meter  = gs_dev_apogess_SP215_irradiance_read();
    data_packet->DS18B20_temperature_kelvin               = gs_dev_maxim_DS18B20_temperature_centik_read();
    data_packet->SHT11_humidity_percent                   = gs_dev_sensirion_SHT11_humidity_pct_read();
    data_packet->MPL115A2T1_pressure_pascals              = gs_dev_nxpusa_MPL115A2T1_pressure_pa_read();

    Serial.println(F("Sample End"));
    b->sample_count = 0;

    // Disabled this for dragonfruit deployment on 2016-10-20 with T=30s
    // b->sample_count++;
    gs_board_tx(b);
}

/******************************
 *
 * Name:        gs_board_ready_tx
 * Returns:     Integer indicating if ready to transmit
 * Parameter:   Function pointer to struct gs-board
 * Description: Checks if board is ready to transmit
 *
 ******************************/

 static int gs_board_ready_tx(struct gs_board* b){
     // Disabled this for dragonfruit deployment on 2016-10-06 with T=30s
     /*
     const int max_samples = 20;
     if(b->sample_count > max_samples-1){
         return 1;
     }
     else{
         return 0;
     }
     */
     return 0;
 }

 /******************************
  *
  * Name:        gs_board_ready_sample
  * Returns:     Integer indicating if ready to sample
  * Parameter:   Function pointer to struct gs-board
  * Description: Waits 30 seconds between sampling sensors
  *              and returns a "1" after 30 seconds. This
  *              implementation is used instead of a delay
  *              since delay will block all other operations.
  *
  ******************************/

 static int gs_board_ready_sample(struct gs_board* b){
     const unsigned long wait_ms = 1000*30;
     const unsigned long sample_delta = millis() - b->prev_sample_ms;

     if( sample_delta >= wait_ms){
         b->prev_sample_ms = millis();
         return 1;
     }
     else{
         return 0;
     }
 }

 /******************************
  *
  * Name:        gs_board_ready_run_cmd
  * Returns:     Number of bytes available to read
  * Parameter:   Function pointer to struct gs-board
  * Description: Get the number of bytes avaiable for reading from the serial port
  *
  ******************************/

 static int gs_board_ready_run_cmd(struct gs_board* b){
     return Serial.available();
 }

 /******************************
  *
  * Name:        gs_board_run_cmd
  * Returns:     Nothing
  * Parameter:   Function pointer to struct gs-board
  * Description: Poll sensors in CMD mode in serial monitor
  *
  ******************************/

 static void gs_board_run_cmd(struct gs_board* b){
     Serial.println(F("Enter CMD Mode"));
     while(Serial.read() != '\n');
     while(1){
         if(Serial.available()){
             char input = Serial.read();
             Serial.print(F("GOT A CMD: "));
             Serial.println(input);
             while(Serial.read() != '\n');
             if(input == 'E') {
                 break;
             }
             else{
                 switch(input){
                     case 'T':
                         Serial.println(F("CMD Mode cmd"));
                         break;
                     case 'P':
                         Serial.println(F("Running POST"));
                         b->post();
                         break;
                     default:
                         break;
                 }
             }
         }
     }
 }

 /******************************
  *
  * Name:        gs_board_ready_heartbeat_tx
  * Returns:     Integer indicating if ready to transmit
  * Parameter:   Function pointer to struct gs-board
  * Description: Waits 30 seconds between sampling sensors
  *              and returns a "1" after 30 seconds. This
  *              implementation is used instead of a delay
  *              since delay will block all other operations.
  *
  ******************************/

 static int gs_board_ready_heartbeat_tx(struct gs_board* b){
     const int wait_ms = 3000;
     int sample_delta = millis() - b->prev_heartbeat_ms;

     unsigned long max_heartbeat_ms = (unsigned long) 1000*69*5;

     int heartbeat_enable = 1;

     #ifndef HB_FOREVER
     heartbeat_enable = millis() < max_heartbeat_ms;
     #endif

     // Heartbeats are only enabled for 5 minutes after the
     // device boots up.
     if( heartbeat_enable ){
         if( sample_delta >= wait_ms){
             b->prev_heartbeat_ms = millis();
             return 1;
         }
         else{
             return 0;
         }
     }
     return 0;
 }

 /******************************
  *
  * Name:        gs_board_heartbeat_tx
  * Returns:     Nothing
  * Parameter:   Function pointer to struct gs-board
  * Description: Transmits heartbeat packet
  *
  ******************************/

 static void gs_board_heartbeat_tx(struct gs_board* b){
     uint8_t payload[_GS_DEV_DIGI_XBEE_BUFSIZE_];
     struct gs_heartbeat_packet hb_packet;

     hb_packet.schema = 0;
     hb_packet.uptime_milliseconds = millis();
     hb_packet.battery_millivolts = gs_dev_battery_read();
     hb_packet.node_address = gs_dev_eeprom_node_address_read();

     int schema_len = sizeof(hb_packet);

     Serial.println(F("TX Heartbeat Start"));

     // We need to copy our struct data over to a byte array
     // to get a consistent size for sending over xbee.
     // Raw structs have alignment bytes that are in-between the
     // data bytes.
     memset(payload, '\0', sizeof(payload));
     memcpy(payload, &(hb_packet), schema_len);
     gs_dev_digi_xbee_write(payload, schema_len);

     Serial.println(F("TX Heartbeat End"));
 }

 /******************************
  *
  * Name:        gs_board_tx
  * Returns:     Nothing
  * Parameter:   Function pointer to struct gs-board
  * Description: Transmits sensor packet
  *
  ******************************/

 static void gs_board_tx(struct gs_board* b){
     uint8_t payload[_GS_DEV_DIGI_XBEE_BUFSIZE_];
     int schema_len = sizeof(b->data_packet);

     Serial.println(F("Sample TX Start"));

     // We need to copy our struct data over to a byte array
     // to get a consistent size for sending over xbee.
     // Raw structs have alignment bytes that are in-between the
     // data bytes.
     memset(payload, '\0', sizeof(payload));
     memcpy(payload, &(b->data_packet), schema_len);
     gs_dev_digi_xbee_write(payload, schema_len);

     // Reset the board sample count so that
     // goes through the sample loop again.
     b->sample_count = 0;

     Serial.println(F("Sample TX End"));
 }

 static void gs_board_soft_rst(){
     asm volatile ("jmp 0");
 }
