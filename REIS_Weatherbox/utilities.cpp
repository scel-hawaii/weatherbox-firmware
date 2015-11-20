/*******************************
 *
 *    File: utilities.cpp
 *    REIS Weatherbox Firmware
 *
 *    File containing utility functions
 *
 ******************************/


#include "schema.h"
#include "utilities.h"


/*******************************
 *
 *    Name: chk_overflow
 *    Returns: 0 or 1
 *    Parameter: Current and previous millis() times
 *    Description: Compares the previous runtime with the current runtime to
 *                 check if an overflow occured
 *
 ******************************/

int chk_overflow(unsigned long current_value, unsigned long previous_value)
{
	if(current_value < previous_value)
		return TRUE;
	else
		return FALSE;
}


/*******************************
 *
 *    Name: sampleBatteryVoltage
 *    Returns: An averaged battery voltage
 *    Parameter: None.
 *    Description: Samples the battery ADC voltage a number of times and
 *                 averages it
 *
 *******************************/

long sampleBatteryVoltage(void)
{
  double temp;
  
  for(i=0; i < ADC_SAMPLE_NUM; i++)
  {
	temp += analogRead(_PIN_BATT_V);
  }

	temp = temp/ADC_SAMPLE_NUM;
	return ((temp*5000.0/1023));
}


/**********************************
 *
 *    Name: chk_health
 *    Returns: GOOD_SOLAR, NORMAL, POOR
 *    Parameter: LowPassFilter* f
 *    Description: 
 *
 *********************************/

int chkHealth(void)
{
	int apogee_voltage = 0, panel_voltage = 0;

	apogee_voltage = LPF_get_current_output(&solar_filter);
	panel_voltage = 2*analogRead(_PIN_SOLAR_V);

	if(LPF_get_current_output(&battery_filter) >= THRESH_GOOD_BATT_V)
		return NORMAL;
#ifdef HEALTH_GOOD_APOGEE
	else if(apogee_voltage >= THRESH_GOOD_APOGEE_V)
#else //HEALTH_GOOD_PANEL
	else if(panel_voltage >= THRESH_GOOD_PANEL_V)
#endif
		return GOOD_SOLAR;
	else
		return POOR;
}

void sendHealth(void)
{
	long transmit_health = 600000;
	
	if(millis() - health_transmit_timer >= transmit_health)
	{
		//Power on system to transmit health data
		pstate_system(_ACTIVE);
		
		//Wait for the system to fully turn on
		transmit_timer = millis();
		int wake_time = 3000;
		while(millis() - transmit_timer) <= wake_time);

		//Transmit health data
		health_data_transmit();

		//Power off system again until next health data transmission
		pstate_system(_POWER_SAVE);

		//Update time since last health transmission
		health_transmit_timer = millis();
	}
}

void health_data_transmit(void)
{
	getPacketHealth();
	transmitPacketHealth();
}

void transmitPacketHealth(void)
{
	memset(rf_payload, '\0', sizeof(rf_payload));
	memcpy(rf_payload, &health, sizeof(health));
	ZBTxRequest zbtx = ZBTxRequest(addr64, rf_payload, sizeof(health));
	xbee.send(zbtx);
}

void getPacketHealth(void)
{
	health.schema = 5;
	health.address = address;
	health.uptime_ms = millis();
	health.batt_mv = 1000*(analogRead(_PIN_BATT_V)*5/1023);
}


/****************** Power Management Functions *******************/

void pstate_system(int state)
{
	if(state == _ACTIVE)
	{
	    pstate_xbee(_ON);
	    pstate_sensor_array(_ON);
	}
	
	else if(state == _POWER_SAVE)
	{
	    pstate_xbee(_OFF);
	    pstate_sensors_array(_OFF);
	}
}

void pstate_xbee(int state)
{
	power_state.xbee = state;
	sync_pstate();
}

void pstate_sensors_array(int state)
{
	power_state.sensors_array = state;
	sync_pstate();
}

void sync_pstate(void)
{
	digitalWrite(_PIN_XBEE_SLEEP, !power_state.xbee);
	digitalWrite(_Pin_PSWITCH, power_state.sensor_array);
	
}

/******************************************************************/



