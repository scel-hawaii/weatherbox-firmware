/*********************************************
 *
 *    File: firmware.cpp
 *    SCEL Weatherbox Firmware
 *
 *    The main driver file for the SCEL Weatherbox
 *
 ********************************************/

#include <EEPROM.h>

/* Program Libraries */
#include "log.h"

#ifdef GA
#include "gen_apple/ga_board.h"
#elif defined(GC)
#include "gen_cranberry/gc_board.h"
#elif defined(GD)
#include "gen_dragonfruit/gd_board.h"
#endif

/* Arudino Libraries */
#include <Wire.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

/* External Libraries */
#include <SHT1x.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_INA219.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_MPL115A2.h>
#include <Adafruit_ADS1015.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <HIH613x.h>
#include <XBee.h>

#ifdef GA
struct ga_board board;
#endif

#ifdef GC
struct gc_board board;
#endif

#ifdef GD
struct gd_board board;
#endif

volatile int f_timer=0;
/*********************************************
 *
 *    Name:        setup
 *    Returns:     Nothing
 *    Parameter:   Nothing
 *    Description: Runs once at the start of the sequence.
 *                     Used for initialization. Place all
 *                     init scripts here.
 *
 ********************************************/
void setup(){
    #ifdef GA
    ga_board_init(&board);
    #endif

    #ifdef GC
    gc_board_init(&board);
    #endif

    #ifdef GD
    gd_board_init(&board);
    #endif

    board.print_build_opts();
    board.setup(&board);
    board.post();

    #ifdef _BCFG_ONLY_POST
    // Stop execution if the ONLY_POST build configuration
    // flag is defined.
    while(1);
    #endif

    /* Dragonfruit: clock frequency of 16MHz
       Set the timer control register A
       Bits 6-7: Disconnect output compare pin from timer
       Bits 2-5: Unused
       Bits 0-1: Disable PWM
    */
    TCCR1A = 0x00;

    /* Load the timer counter register to account for varying clock frequencies
       of different weatherbox designs
       Dragonfruit: Prescalar=256, so each clock cycle takes (1/16MHz)*256 secs.
       So, 1 second will elapse after 16*10^6/256 clock cycles have elapsed.
       16-bit timer so clock will overflow after 2^16 clock cycles
       So, we need to preload timer with value = 2^16 - 16*10^6/256
    */
    TCNT1=0x0BDC;

    /* Set the timer control register B
       Bits 0-2, clock select bits: set the prescalar to 1:256
       Bits 3-5, unused
       Bit 6, input capture edge select: capture contents of TCNT1 on falling
       edge
       Bit 7, input capture noise canceler: disable noise canceler
    */
    TCCR1B = 0x04;

    // Set timer interrupt mask register
    // Enable timer 1 to interrupt on overflow
    TIMSK1=0x01;
}

/*********************************************
 *
 *    Name:        loop
 *    Returns:     Nothing
 *    Parameter:   Nothing
 *    Description: Main program function, runs constantly.
 *                     Executed after setup(), and will
 *                     continue running indefinitely,
 *                     unless conditions stop it.
 *
 ********************************************/
void loop(){
    /*if(board.ready_sample(&board))  board.sample(&board);
    if(board.ready_tx(&board))      board.tx(&board);
    if(board.ready_run_cmd(&board))      board.run_cmd(&board);
    if(board.ready_heartbeat_tx(&board))      board.heartbeat_tx(&board);*/

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Power down microchip on sleep mode
    cli();                                // Disable global interrupts
    if (f_timer)
    {
      // reset the timer flag
      f_timer=0;
      board.cycle_count++;
      if (board.cycle_count%3==0){
          board.sample(&board);
          board.heartbeat_tx(&board);
      }
      if (board.cycle_count%30==0){
          // board.sample(&board);
          board.tx(&board);
      }
      sleep_enable();                     // Set the sleep enable bit
      power_adc_disable();                // Disable peripherals
      power_spi_disable();
      power_timer0_disable();
      power_timer2_disable();
      power_twi_disable();
      sleep_bod_disable();                // Disable brown out detector
      sei();                              // Enable global interrupts
      sleep_cpu();                        // Put device into sleep mode
      sleep_disable();                    // Clear the sleep enable bit
    }
    sei();                                // Enable global interrupts
}

// Use 16-bit timer interrupt, timer 1
ISR(TIMER1_OVF_vect){
  // set the flag
  // if timer has overflowed, set the timer flag to 1
   if(f_timer == 0)
   {
     f_timer = 1;
   }
}
