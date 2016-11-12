#include <Arduino.h>
//#include <XBee.h>
#include <SoftwareSerial.h>

#define _GA_DEV_XBEE_BUFSIZE_ 150


#define START_BYTE 0x7e
#define ESCAPE 0x7d
#define XON 0x11
#define XOFF 0x13

#define ZB_TX_API_LENGTH 12
#define ZB_BROADCAST_RADIUS_MAX_HOPS 0
#define ZB_TX_REQUEST 0x10
#define DEFAULT_FRAME_ID 1
#define ZB_TX_UNICAST 0
#define ZB_BROADCAST_PACKET 0x02

struct XBee {
  Stream* serial;
  uint8_t api_id;
  uint8_t frame_id;
  uint8_t broadcast_range;
  uint8_t option;
  uint16_t addr16;
  uint32_t msb;
  uint32_t lsb;
};



#ifndef GA_DEV_XBEE
#define GA_DEV_XBEE
void ga_dev_xbee_open(void);
int ga_dev_xbee_avail(void);
int ga_dev_xbee_read(void);
void ga_dev_xbee_write(uint8_t val);

uint8_t ga_dev_xbee_frame_data(uint8_t* data, int pos);
uint8_t ga_dev_xbee_frame_length(int data_len);
void ga_dev_xbee_send_byte(uint8_t b, bool escape);
void ga_dev_xbee_send(uint8_t* data, int data_len);

const byte rx_pin = 2;
const byte tx_pin = 9;
static SoftwareSerial soft_serial(rx_pin, tx_pin);
#endif
