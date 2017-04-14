/*******************************
 *
 * File: gg_board.cpp
 *
 * Contains definitions for board initialization, PlatformIO POST
 * commands, and sampling sensors.
 *
 * ****************************/

#include "gg_board.h"

static void gg_board_print_build_opts();
static void gg_board_setup(struct gg_board* b);
static void gg_board_post();

static void gg_board_run_cmd(struct gg_board* b);
static int gg_board_ready_run_cmd(struct gg_board* b);

static void gg_board_sample(struct gg_board* b);
static int gg_board_ready_sample(struct gg_board* b);

static void gg_board_tx(struct gg_board* b);
static int gg_board_ready_tx(struct gg_board* b);

static int gg_board_ready_heartbeat_tx(struct gg_board* b);
static void gg_board_heartbeat_tx(struct gg_board* b);

/******************************
 *
 * Name:        gg_board_init
 * Returns:     Nothing
 * Parameter:   Function pointer to struct gg_board
 * Description: Initialize Guava board
 *
 ******************************/

void gg_board_init(gg_board *b){
    // Link functions to make them accessable
    b->print_build_opts = &gg_board_print_build_opts;
    b->setup = &gg_board_setup;
    b->post = &gg_board_post;

    // Sample and TX module
    b->tx = &gg_board_tx;
    b->ready_tx = &gg_board_ready_tx;
    b->sample = &gg_board_sample;
    b->ready_sample = &gg_board_ready_sample;

    // CMD Parsing Module
    b->run_cmd = &gg_board_run_cmd;
    b->ready_run_cmd = &gg_board_ready_run_cmd;

    // Heartbeat Module
    b->ready_heartbeat_tx = &gg_board_ready_heartbeat_tx;
    b->heartbeat_tx = &gg_board_heartbeat_tx;

    // State Variables
    b->sample_count = 0;
    b->node_address = 0;
    b->prev_sample_ms = 0;

    // Initialize the packet
    b->data_packet.schema = 4;
    b->data_packet.node_address = 0;
    b->data_packet.uptime_milliseconds = 0;
    b->data_packet.battery_millivolts = 0;
    b->data_packet.panel_millivolts = 0;
    b->data_packet.bme280_pressure_pascals = 0;
    b->data_packet.bme280_temperature_kelvin = 0;
    b->data_packet.bme280_humidity_percent = 0;
    b->data_packet.sp215_irradiance_watts_per_square_meter = 0;
}

/******************************
 *
 * Name:        gg_board_print_build_opts
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Initialize board generation and baudrate
 *
 ******************************/

static void gg_board_print_build_opts()
{
    Serial.begin(9600);
    Serial.println(F("Board Opts"));
    Serial.println(F("Gen: guava23"));
}

/******************************
 *
 * Name:        gg_board_setup
 * Returns:     Nothing
 * Parameter:   Function pointer to struct gg_board
 * Description: Enable sensor pin, initialize sensors,
 *              obtain node address from eeprom
 *
 ******************************/

static void gg_board_setup(struct gg_board* b){
    Serial.begin(9600);
    Serial.println(F("Board Setup Start"));

    // Open Devices
    gg_dev_digi_xbee_open();
    gg_dev_apogee_SP212_irradiance_open();
    gg_dev_battery_open();
    gg_dev_solar_panel_open();
    gg_dev_eeprom_node_address_open();
    gg_dev_adafruit_BME280_humidity_open();
    gg_dev_adafruit_BME280_temperature_open();
    gg_dev_adafruit_BME280_pressure_open();

    // load the address from the EEPROM into memory
    b->node_address = gg_dev_eeprom_node_address_read();

    delay(100);
    Serial.println(F("Board Setup Done"));
}

/******************************
 *
 * Name:        gg_board_post
 * Returns:     Nothing
 * Parameter:   Nothing
 * Description: Power on self test when board initially starts
 *              and poll each sensor. Also used to check
 *              sensor values on serial monitor.
 *
 ******************************/

static void gg_board_post(){
    Serial.println(F("POST Begin"));

    // Display node addr
    gg_dev_eeprom_node_address_test();

    // Check BME280 Humidity
    gg_dev_adafruit_BME280_humidity_test();

    // Check BME280 Pressure
    gg_dev_adafruit_BME280_pressure_test();

    // Check BME280 Temperature
    gg_dev_adafruit_BME280_temperature_test();

    // Check apogee_sp212
    gg_dev_apogee_SP212_irradiance_test();

    // Check batt
    gg_dev_battery_test();

    // check panel sensor value
    gg_dev_solar_panel_test();

    Serial.println(F("POST End"));

}

/******************************
 *
 * Name:        gg_board_sample
 * Returns:     Nothing
 * Parameter:   Function pointer to struct gg-board
 * Description: Sample each sensor and store into data packet
 *
 ******************************/

static void gg_board_sample(struct gg_board* b){
    Serial.print("[");
    Serial.print(millis());
    Serial.print("] ");
    Serial.println(F("Sample Start"));
    // Disabled this for guava deployment on 2016-10-06 with T=30s
    // Serial.println(b->sample_count);

    struct gg_packet* data_packet = &(b->data_packet);
    data_packet->uptime_milliseconds                     = millis();
    data_packet->battery_millivolts                      = gg_dev_battery_read();
    data_packet->panel_millivolts                        = gg_dev_solar_panel_read();
    data_packet->bme280_pressure_pascals                 = gg_dev_adafruit_BME280_pressure_read();
    data_packet->bme280_temperature_kelvin               = gg_dev_adafruit_BME280_temperature_read();
    data_packet->bme280_humidity_percent                 = gg_dev_adafruit_BME280_humidity_read();
    data_packet->sp215_irradiance_watts_per_square_meter = gg_dev_apogee_SP212_irradiance_read();
    data_packet->node_address                            = b->node_address;

    Serial.println(F("Sample End"));
    b->sample_count = 0;

    gg_board_tx(b);

    // Disabled this for guava deployment on 2016-10-06 with T=30s
    // b->sample_count++;
}

/******************************
 *
 * Name:        gg_board_ready_tx
 * Returns:     Integer indicating if ready to transmit
 * Parameter:   Function pointer to struct gg-board
 * Description: Checks to see if the board is ready
 *              transmit
 *
 ******************************/

static int gg_board_ready_tx(struct gg_board* b){
    // Disabled this for guava deployment on 2016-10-06 with T=30s
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
 * Name:        gg_board_ready_sample
 * Returns:     Integer indicating if ready to sample
 * Parameter:   Function pointer to struct gg-board
 * Description: Waits 30 seconds between sampling sensors
 *              and returns a 1 after thirty seconds. This
 *              is used in place of a delay because delay
 *              will block all other operations
 *
 ******************************/

static int gg_board_ready_sample(struct gg_board* b){
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
 * Name:        gg_board_ready_run_cmd
 * Returns:     Number of bytes available to read
 * Parameter:   Function pointer to struct gg-board
 * Description: Get the number of bytes avaiable for reading from the serial port
 *
 ******************************/

static int gg_board_ready_run_cmd(struct gg_board* b){
    return Serial.available();
}

/******************************
 *
 * Name:        gg_board_run_cmd
 * Returns:     Nothing
 * Parameter:   Function pointer to struct gc-board
 * Description: Poll sensors in CMD mode in serial monitor
 *
 ******************************/

 static void gg_board_run_cmd(struct gg_board* b){
     Serial.println(F("\nEnter CMD Mode"));
     Serial.println(F("[E] - Exit Command Mode"));
     Serial.println(F("[P] - Run Power On Self-Test"));
     Serial.println(F("[S] - Sensor Sampling Menu"));


     while(Serial.read() != '\n'); //In Arduino IDE, make sure line ending is \n
     while(1){
         if(Serial.available()){
             char input = Serial.read(), input2;

             Serial.print(F("GOT A CMD: "));
             Serial.println(input);
             while(Serial.read() != '\n');
             if(input == 'E') {
                 Serial.println(F("Leaving CMD Mode"));
                 break;
             }
             else{
                 switch(input){
                     case 'P':
                         Serial.println(F("Running POST"));
                         b->post();
                         break;
                     case 'S':
                         Serial.println(F("\nSensor Sampling Menu"));
                         Serial.println(F("[1] - Node Address"));
                         Serial.println(F("[2] - BME280 Temperature (cK)"));
                         Serial.println(F("[3] - BME280 Humidity (\%)"));
                         Serial.println(F("[4] - BME280 Pressure (Pa)"));
                         Serial.println(F("[5] - SP212 Solar Irradiance (mW)"));
                         Serial.println(F("[6] - Battery Voltage (mW)"));
                         Serial.println(F("[7] - Solar Panel Voltage (mW)"));
                         Serial.println(F("[E] - Exit to Main Menu"));

                         while(1){
                             if(Serial.available()){
                                 input2 = Serial.read();
                                 Serial.print(F("GOT A CMD: "));
                                 Serial.println(input2);
                                 while(Serial.read() != '\n');
                                     if(input2 == 'E'){
                                         Serial.println(F("Exiting to Main Menu"));
                                         break;
                                     }
                                     switch(input2){
                                         case '1':
                                             gg_dev_eeprom_node_address_test();
                                             break;
                                         case '2':
                                             gg_dev_adafruit_BME280_temperature_test();
                                             break;
                                         case '3':
                                             gg_dev_adafruit_BME280_humidity_test();
                                             break;
                                         case '4':
                                             gg_dev_adafruit_BME280_pressure_test();
                                             break;
                                         case '5':
                                             gg_dev_apogee_SP212_irradiance_test();
                                             break;
                                         case '6':
                                             gg_dev_battery_test();
                                             break;
                                         case '7':
                                             gg_dev_solar_panel_test();
                                             break;
                                         default:
                                             break;
                                   }
                               }
                           }
                     default:
                         break;
                 }
             }
         }
     }
 }

/******************************
 *
 * Name:        gg_board_ready_heartbeat_tx
 * Returns:     Integer indicating if ready to transmit
 * Parameter:   Function pointer to struct gg-board
 * Description: Waits 3 seconds between sampling sensors
 *              and returns a 1 after three seconds. This
 *              is used in place of a delay because delay
 *              will block all other operations
 *
 ******************************/

static int gg_board_ready_heartbeat_tx(struct gg_board* b){
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
 * Name:        gg_board_heartbeat_tx
 * Returns:     Nothing
 * Parameter:   Function pointer to struct gg-board
 * Description: Transmits heartbeat packet
 *
 ******************************/

static void gg_board_heartbeat_tx(struct gg_board* b){
    uint8_t payload[_GG_DEV_DIGI_XBEE_BUFSIZE_];
    struct gg_heartbeat_packet hb_packet;

    hb_packet.schema = 0;
    hb_packet.uptime_milliseconds = millis();
    hb_packet.battery_millivolts = gg_dev_battery_read();
    hb_packet.node_address = gg_dev_eeprom_node_address_read();

    int schema_len = sizeof(hb_packet);

    Serial.println(F("TX Heartbeat Start"));

    // We need to copy our struct data over to a byte array
    // to get a consistent size for sending over xbee.
    // Raw structs have alignment bytes that are in-between the
    // data bytes.
    memset(payload, '\0', sizeof(payload));
    memcpy(payload, &(hb_packet), schema_len);
    gg_dev_digi_xbee_write(payload, schema_len);

    Serial.println(F("TX Heartbeat End"));
}

/******************************
 *
 * Name:        gg_board_tx
 * Returns:     Nothing
 * Parameter:   Function pointer to struct gg-board
 * Description: Transmits sensor packet
 *
 ******************************/

static void gg_board_tx(struct gg_board* b){
    uint8_t payload[_GG_DEV_DIGI_XBEE_BUFSIZE_];
    int schema_len = sizeof(b->data_packet);

    Serial.println(F("Sample TX Start"));

    // We need to copy our struct data over to a byte array
    // to get a consistent size for sending over xbee.
    // Raw structs have alignment bytes that are in-between the
    // data bytes.
    memset(payload, '\0', sizeof(payload));
    memcpy(payload, &(b->data_packet), schema_len);
    gg_dev_digi_xbee_write(payload, schema_len);

    // Reset the board sample count so that
    // goes through the sample loop aggin.
    b->sample_count = 0;

    Serial.println(F("Sample TX End"));
}

static void gg_board_soft_rst(){
    asm volatile ("jmp 0");
}
