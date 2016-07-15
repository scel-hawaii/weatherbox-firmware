/*******************************************
 *
 *    File: transmit.cpp
 *    REIS Weatherbox Firmware
 *
 *    File containing Transmission functions
 *
 ******************************************/

/* Program Libraries */
#include "transmit.h"


/******************************************
 *
 *   Name:        Packet_ClearBIN
 *   Returns:     Nothing
 *   Parameter:   schema Global packet
 *   Description: Clears/Initializes the packet schema
 *                    information.
 *
 *****************************************/
void Packet_ClearBIN(void){

    /* Variables used for indexes */
    int i, j, k;

    /* Initialize values contained in the packet */
    G_BINpacket.address = EEPROM.read(2) | (EEPROM.read(3) << 8); //Addr from EEPROM
    G_BINpacket.uptime_ms = 0;
    G_BINpacket.n = 0;
    G_BINpacket.bmp085_press_pa = 0;
    G_BINpacket.bmp085_temp_decic = 0;
    G_BINpacket.humidity_centi_pct = 0;
    
    /* Use a for loop to clear the info for the data with multiple points */
    for(i = 0; i < 60; i++){

        /* Variables for indices */
        j = i/10;
        k = i/3;

        /* Polled every 10 seconds */
        G_BINpacket.batt_mv[j] = 0;
        G_BINpacket.panel_mv[j] = 0;
#ifdef APPLE
	G_BINpacket.dallas_roof_c = 0;
#endif
 
        /* Polled every 3 seconds */
        G_BINpacket.apogee_w_m2[k] = 0;
    }
}


/******************************************
 *
 *   Name:        Packet_ConBIN
 *   Returns:     Nothing
 *   Parameter:   schema Global packet
 *   Description: Constructs a packet with data polled
 *                    from the sensors.
 *
 *****************************************/
void Packet_ConBIN(void){

    /* Note: Address and Schema data already put in during Init */
    
    /* Index Variable */
    int n = G_BINpacket.n;

    /* Variables to hold Sensor Readings */
    long BatterymV = 0;
    long SolarIrrmV = 0;
    long SolarIrr_w_m2 = 0;
    long Humiditypct = 0;
    long PanelmV = 0;
    long Pressurepa = 0;
    long Tempdecic = 0;
    long Dallas_RoofTemp_c = 0;
    unsigned long uptime;

#ifndef TEST
    /* Sample Sensors */
    BatterymV = Sensors_sampleBatterymV();
    SolarIrrmV = Sensors_sampleSolarIrrmV();
    Humiditypct = Sensors_sampleHumiditypct();
    PanelmV = Sensors_samplePanelmV();
    Pressurepa = Sensors_samplePressurepa();
    Tempdecic = Sensors_sampleTempdecic();
#ifdef APPLE
    Dallas_RoofTemp_c = a_Sensors_sampleRoofTempdecic();
#endif
#endif

    /* Uptime data */
    uptime = millis();
    
    /* Check overflow before putting in uptime */
    G_BINpacket.overflow_num += chk_overflow(uptime, G_BINpacket.uptime_ms);

    /* Save new uptime */
    G_BINpacket.uptime_ms = uptime;

    /* Pack sensor data */
    G_BINpacket.batt_mv[n/10] = BatterymV;
    G_BINpacket.panel_mv[n/10] = PanelmV;
    G_BINpacket.bmp085_press_pa = Pressurepa;
    G_BINpacket.bmp085_temp_decic = Tempdecic;
    G_BINpacket.humidity_centi_pct = Humiditypct;
    G_BINpacket.apogee_w_m2[n/3] = SolarIrrmV;
#ifdef APPLE
    G_BINpacket.dallas_roof_c = Dallas_RoofTemp_c;
#endif
    /* Increment index */
    G_BINpacket.n += 1;
}


/******************************************
 *
 *   Name:        Packet_TransmitBIN
 *   Returns:     Nothing
 *   Parameter:   schema Global packet
 *   Description: Transmits using Arduino Xbee functions,
 *                    the packet is transfered as a
 *                    binary packet.
 *
 *****************************************/
void Packet_TransmitBIN(void){
    
    /* Enable XBee */
#ifndef APPLE    
    digitalWrite(_PIN_XBEE_EN, HIGH);
#endif

    /* Variable to contain length */
    int len = 0;

    /* Obtain address of receiving end */
    XBeeAddress64 addr64 = XBeeAddress64(0,0);
    
    /* Packet to be transmitted */
    uint8_t payload[MAX_SIZE];

    /* Clear the payload */
    memset(payload, '\0', sizeof(payload));

    /* Obtain length of the packet */
    len = sizeof(G_BINpacket);

    /* Transfer information into payload */
    memcpy(payload, &G_BINpacket, len);

#ifdef DEBUG_S
    /* Debug */
    Serial.println(F("BIN Length is: "));
    Serial.print(len);
    Serial.print("\n");

    /* The payload */
    int i;
    for(i = 0; i < MAX_SIZE; i++)
    {
        Serial.println(payload[i]);
        Serial.print("\n");
    }
#endif

   /* Transfer the payload */
    ZBTxRequest zbTx = ZBTxRequest(addr64, payload, len);
    G_xbee.send(zbTx); //!!Prints packet to serial monitor
}


/******************************************
 *
 *   Name:        Test_Packet_GenBIN
 *   Returns:     Nothing
 *   Parameter:   schema Global packet
 *   Description: Constructs a packet with hard-coded
 *                    information.  Used for the initial
 *                    test of the Transmission functions.
 *                    To be replaced by Packet Construction.
 *
 *****************************************/
void Test_Packet_GenBIN(void){

    /* Hard-coded data to put into packet */
    long batt_mv_raw = 1;
    long panel_mv_raw = 2;
    long apogee_raw = 3;
    long pressure_raw = 4;
    long temperature_raw = 5;
    long humidity_raw = 6;
    long dallas_rooftemp_decic = 7;
    int n = 10;
    unsigned long uptime = 1000;

#ifdef DEBUG_S
    /* Debug */
    Serial.println(F("Generating - BIN"));
#endif

    /* Store values into packet */
    G_BINpacket.batt_mv[n/10] = batt_mv_raw;
    G_BINpacket.panel_mv[n/10] = panel_mv_raw;
    G_BINpacket.apogee_w_m2[n/3] = apogee_raw;
    G_BINpacket.bmp085_press_pa = pressure_raw;
    G_BINpacket.bmp085_temp_decic = temperature_raw;
    G_BINpacket.humidity_centi_pct = humidity_raw;
    G_BINpacket.n = n;
    G_BINpacket.uptime_ms = uptime;
#ifdef APPLE
    G_BINpacket.dallas_roof_c = dallas_rooftemp_decic;
#endif
}
