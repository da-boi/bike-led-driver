/*
 * led_driver.c
 *
 * Created: 07.11.2018 22:10:10
 * Author : Nick
 */ 
#define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h>

#include "input.h"
#include "time.h"
#include "pwm.h"
#include "adc.h"
#include "led.h"
#include "hyst.h"



#if 1
int main(void) {
	volatile enum {NORMAL, STROBE} mode;
	volatile enum bool emergency;
	
	mode = NORMAL;
	emergency = FALSE;
	time_init();
	pwm_init();
	adc_init();
	sei();
	
    while (1) {
		
		/*
		 * NORMAL MODE
		*/
		if ( mode == NORMAL ) {
			
			/* ======================== */
			if ( UP == PRESSED || DOWN == PRESSED ) {
				uint32_t start = time_get_millis();
				
				time_delay_millis(DEBOUNCE_DELAY);
				
				if ( UP == PRESSED && DOWN == PRESSED ) {
					mode = STROBE;
					pwm_strobe_on();
				}
					
				else if ( UP == PRESSED ) {
					enum bool emergencyPressed = FALSE;
					while ( UP == PRESSED ) {
						if ( time_get_millis() - start > EMERGENCY_DELAY ) {
							emergencyPressed = TRUE;
							emergency = TRUE;
							break;
						}
					}
					if ( !emergencyPressed ) {
						led_increase();
					}
				}
					
				else if ( DOWN == PRESSED ) {
					while ( DOWN == PRESSED  );
					led_decrease();
				}
			}
		}
			
			
		/*
		 * STROBE MODE
		*/	
		else if ( mode == STROBE ) {
			if ( UP == RELEASED && DOWN == RELEASED ) {
				pwm_strobe_off();
				mode = NORMAL;
			}
		}
		
		
		
		/* 
			* ===============================
			* ======== ADC Readings =========
			* ===============================
		*/
		
		
		if ( emergency == FALSE ) {
			
			uint16_t temp[BUFFER_SIZE];
			uint16_t volt[BUFFER_SIZE];
			uint16_t tempAverage;
			uint16_t voltAverage;
			int c = 0;
			enum bool flag = FALSE;
			int i;
		
			for (i=0; i < (BUFFER_SIZE-1); i++) {
				temp[i] = temp[i+1];
				volt[i] = volt[i+1];
			}
			temp[BUFFER_SIZE-1] = adc_get_temperature();
			volt[BUFFER_SIZE-1] = adc_get_voltage();
			c++;
			if ( c == 10 ) {
				flag = TRUE;
			}
		
			if ( flag == TRUE ) {
				tempAverage = 0;
				voltAverage = 0;
				for (i=0; i < BUFFER_SIZE; i++) {
					tempAverage += temp[i];
					voltAverage += volt[i];
				}
				tempAverage = tempAverage / BUFFER_SIZE;
				voltAverage = voltAverage / BUFFER_SIZE;
			}
		
			
			uint16_t vCurrent = 0;
			uint16_t vPrevious = 0;
			int vState = VOLT_STATE_100;
			int vStateCheck = NO_STATE_CHANGE;
			uint16_t tCurrent = 0;
			uint16_t tPrevious = 0;
			int tState = TEMP_STATE_100;
			int tStateCheck = NO_STATE_CHANGE;
		
			vPrevious = vCurrent;
			vCurrent = voltAverage;
			vStateCheck = hyst_check(vCurrent, vPrevious, g_voltageHystTable, 3);
			if ( vStateCheck != NO_STATE_CHANGE ) {
				vState = vStateCheck;
			}
			
			tPrevious = tCurrent;
			tCurrent = tempAverage;
			tStateCheck = hyst_check(tCurrent, tPrevious, g_temperatureHystTable, 3);
			if ( tStateCheck != NO_STATE_CHANGE ) {
				tState = tStateCheck;
			}
			
			int state = VOLT_STATE_100;
			
			if ( vState < tState) {
				state = vState;
			} else {
				state = tState;
			}
		
			switch (state) {
				case VOLT_STATE_OFF:
					led_set_luminance_limit(0);
					break;
				case VOLT_STATE_50:
					led_set_luminance_limit(5);
					break;
				case VOLT_STATE_70:
					led_set_luminance_limit(7);
					break;
				case VOLT_STATE_100:
					led_set_luminance_limit(10);
					break;
			}
			
		} else {
			led_set_luminance_limit(10);
		} // end: if ( luminance == FALSE )
		led_check();
    } // end: while (1)
} // end: main()
#endif
