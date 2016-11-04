#include "gc_board.h"

static void gc_board_print_build_opts();
static void gc_board_setup(struct gc_board* b);
static void gc_board_post();

static void gc_board_run_cmd(struct gc_board* b);
static int gc_board_ready_run_cmd(struct gc_board* b);

static void gc_board_sample(struct gc_board* b);
static int gc_board_ready_sample(struct gc_board* b);

static void gc_board_tx(struct gc_board* b);
static int gc_board_ready_tx(struct gc_board* b);

static int gc_board_ready_heartbeat_tx(struct gc_board* b);
static void gc_board_heartbeat_tx(struct gc_board* b);

void gc_board_init(gc_board *b){
    // Link functions to make them accessable
    b->print_build_opts = &gc_board_print_build_opts;
    b->setup = &gc_board_setup;
    b->post = &gc_board_post;

    // Sample and TX module
    b->tx = &gc_board_tx;
    b->ready_tx = &gc_board_ready_tx;
    b->sample = &gc_board_sample;
    b->ready_sample = &gc_board_ready_sample;

    // CMD Parsing Module
    b->run_cmd = &gc_board_run_cmd;
    b->ready_run_cmd = &gc_board_ready_run_cmd;

    // Heartbeat Module
    b->ready_heartbeat_tx = &gc_board_ready_heartbeat_tx;
    b->heartbeat_tx = &gc_board_heartbeat_tx;

    // State Variables
    b->sample_count = 0;
    b->node_addr = 0;
    b->prev_sample_ms = 0;

    // Initialize the packet
    b->data_packet.schema = 2;
    b->data_packet.node_addr = gc_dev_eeprom_naddr_read();
    b->data_packet.uptime_ms = 0;
    b->data_packet.batt_mv = 0;
    b->data_packet.panel_mv = 0;
    b->data_packet.apogee_w_m2 = 0;
    b->data_packet.hih6131_temp_centik = 0;
    b->data_packet.hih6131_humidity_pct = 0;
    b->data_packet.mpl115a2t1_press_pa = 0;
}

static void gc_board_print_build_opts()
{
    Serial.begin(9600);
    Serial.println(F("Board Opts"));
    Serial.println(F("Gen: cranberry"));
}

static void gc_board_setup(struct gc_board* b){
    Serial.begin(9600);
    Serial.println(F("Board Setup Start"));

    // Open Devices
    digitalWrite(_PIN_SEN_EN, HIGH);
    gc_dev_xbee_open();
    gc_dev_apogee_SP212_open();
    gc_dev_batt_open();
    gc_dev_spanel_open();
    gc_dev_eeprom_naddr_open();
    gc_dev_honeywell_HIH6131_open();
    gc_dev_adafruit_MPL115A2_open();

    // Load the address from the hardware
    b->node_addr = gc_dev_eeprom_naddr_read();

    delay(100);
    Serial.println(F("Board Setup Done"));
}

// Power on self test
static void gc_board_post(){
    Serial.println(F("POST Begin"));

    // Display node addr
    gc_dev_eeprom_naddr_test();

    // Check hih6131 temperature
    gc_dev_honeywell_HIH6131_temp_centik_test();

    // Check hih6131 humidity
    gc_dev_honeywell_HIH6131_humidity_pct_test();

    // Check mpl115a2t1 pressure
    gc_dev_adafruit_MPL115A2_press_pa_test();

    // Check apogee_sp212
    gc_dev_apogee_SP212_solar_irr_test();

    // Check battery voltage
    gc_dev_batt_test();

    // check panel sensor value
    gc_dev_spanel_test();

    Serial.println(F("POST End"));
}

static void gc_board_sample(struct gc_board* b){
    Serial.print("[");
    Serial.print(millis());
    Serial.println("]");

    Serial.println(F("Sample Start"));

    //Disabled for Cranberry Deployment, T=30s
    //Serial.println(b->sample_count);

    struct gc_packet* data_packet = &(b->data_packet);
    data_packet->uptime_ms           = millis();
    data_packet->batt_mv             = gc_dev_batt_read();
    data_packet->panel_mv            = gc_dev_spanel_read();
    data_packet->apogee_w_m2         = gc_dev_apogee_SP212_solar_irr_read();
    data_packet->hih6131_temp_centik = gc_dev_honeywell_HIH6131_temp_centik_read();
    data_packet->hih6131_humidity_pct= gc_dev_honeywell_HIH6131_humidity_pct_read();
    data_packet->mpl115a2t1_press_pa = gc_dev_adafruit_MPL115A2_press_pa_read();

    Serial.println(F("Sample End"));
    b->sample_count = 0;

    //Disabled for Cranberry Deployment, T=30s
    //b->sample_count++;

    gc_board_tx(b);
}

static int gc_board_ready_tx(struct gc_board* b){
    //Disabled for Cranberry Deployment, T=30s
/*    const int max_samples = 20;
    if(b->sample_count > max_samples-1){
        return 1;
    }
    else{
        return 0;
    }
*/
    return 0;
}

static int gc_board_ready_sample(struct gc_board* b){
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

static int gc_board_ready_run_cmd(struct gc_board* b){
    return Serial.available();
}

static void gc_board_run_cmd(struct gc_board* b){
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
                        Serial.println(F("[2] - HIH6131 Temperature (cK)"));
                        Serial.println(F("[3] - HIH6131 Humidity (\%)"));
                        Serial.println(F("[4] - MPL115A2 Pressure (Pa)"));
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
                                            gc_dev_eeprom_naddr_post();
                                            break;
                                        case '2':
                                            gc_dev_honeywell_HIH6131_temp_centik_post();
                                            break;
                                        case '3':
                                            gc_dev_honeywell_HIH6131_humidity_pct_post();
                                            break;
                                        case '4':
                                            gc_dev_adafruit_MPL115A2_press_pa_post();
                                            break;
                                        case '5':
                                            gc_dev_apogee_SP212_solar_irr_post();
                                            break;
                                        case '6':
                                            gc_dev_batt_post();
                                            break;
                                        case '7':
                                            gc_dev_spanel_post();
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

static int gc_board_ready_heartbeat_tx(struct gc_board* b){
    const int wait_ms = 3000;
    int sample_delta = millis() - b->prev_heartbeat_ms;

    unsigned long max_heartbeat_ms = (unsigned long) 1000*69*5;

    // Heartbeats are only enabled for 5 minutes after the
    // device boots up.
    if( millis() < max_heartbeat_ms ){
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

static void gc_board_heartbeat_tx(struct gc_board* b){
    uint8_t payload[_GC_DEV_XBEE_BUFSIZE_];
    struct gc_heartbeat_packet hb_packet;

    hb_packet.schema = 0;
    hb_packet.uptime_ms = millis();
    hb_packet.batt_mv = gc_dev_batt_read();
    hb_packet.node_addr = gc_dev_eeprom_naddr_read();

    int schema_len = sizeof(hb_packet);

    Serial.println(F("TX Heartbeat Start"));

    // We need to copy our struct data over to a byte array
    // to get a consistent size for sending over xbee.
    // Raw structs have alignment bytes that are in-between the
    // data bytes.
    memset(payload, '\0', sizeof(payload));
    memcpy(payload, &(hb_packet), schema_len);
    gc_dev_xbee_write(payload, schema_len);

    Serial.println(F("TX Heartbeat End"));
}

static void gc_board_tx(struct gc_board* b){
    uint8_t payload[_GC_DEV_XBEE_BUFSIZE_];
    int schema_len = sizeof(b->data_packet);

    Serial.println(F("Sample TX Start"));

    // We need to copy our struct data over to a byte array
    // to get a consistent size for sending over xbee.
    // Raw structs have alignment bytes that are in-between the
    // data bytes.
    memset(payload, '\0', sizeof(payload));
    memcpy(payload, &(b->data_packet), schema_len);
    gc_dev_xbee_write(payload, schema_len);

    // Reset the board sample count so that
    // goes through the sample loop again.
    b->sample_count = 0;

    Serial.println(F("Sample TX End"));
}

static void gc_board_soft_rst(){
    asm volatile ("jmp 0");
}
