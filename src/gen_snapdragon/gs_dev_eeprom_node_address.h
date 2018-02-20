/*******************************
 *
 * File: gs_dev_eeprom_node_address.h
 *
 * Contains prototypes for reading the node ID
 * from the EEPROM
 *
 ******************************/

#include <Arduino.h>
#include <EEPROM.h>

#ifndef GS_DEV_EEPROM_NODE_ADDRESS_H
#define GS_DEV_EEPROM_NODE_ADDRESS_H
void gs_dev_eeprom_node_address_open(void);
uint16_t gs_dev_eeprom_node_address_read(void);
void gs_dev_eeprom_node_address_test(void);
#endif
