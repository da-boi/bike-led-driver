/*
 * led_driver.c
 *
 * Created: 07.11.2018 22:10:10
 * Author : Nick
 */ 
#define F_CPU 8000000

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "input.h"
#include "time.h"
#include "pwm.h"
#include "adc.h"
#include "led.h"
#include "hyst.h"
#include "log.h"



#if 1
int main(void) {
	char logstr[100];
	
	int16_t temp[BUFFER_SIZE];
	int16_t volt[BUFFER_SIZE];
	int32_t tempAvg = -1;
	int32_t voltAvg = -1;
	int bufferCounter = 0;
	int i;

	int state = STATE_100;
	int tempState = STATE_100;
	int voltState = STATE_100;
	int16_t tempPrev = 0;
	int16_t voltPrev = 32767;
	int newState = NO_STATE_CHANGE;
	
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
			
			/* 
			 * ################################
			 * ****** Input Averaging *********
			 * ################################
			 */

			/* Store the adc readings in a buffer */
			temp[bufferCounter] = adc_get_temperature();
			volt[bufferCounter] = adc_get_voltage();
			bufferCounter++;

			/*
			* Check if buffer is full
			* If so: computer the average
			*   and reset the buffer counter
			*/
			if (bufferCounter == BUFFER_SIZE) {
				tempAvg = 0;
				voltAvg = 0;
				for (i=0; i < BUFFER_SIZE; i++) {
					tempAvg += temp[i];
					voltAvg += volt[i];
				}
				tempAvg /= BUFFER_SIZE;
				voltAvg /= BUFFER_SIZE;

				bufferCounter = 0;

				/* 
				 * ################################
				 * *********** Hysteris ***********
				 * ################################
				 */

				/*
				 * Check if a threshold is reached
				 * If so: switch to the corresponding state
				 */
				newState = hyst_check((int16_t)tempAvg, tempPrev, g_temperatureHystTable, 3);
				if (newState != NO_STATE_CHANGE) {
					tempState = newState;
				}

				newState = hyst_check((int16_t)voltAvg, voltPrev, g_voltageHystTable, 3);
				if (newState != NO_STATE_CHANGE) {
					voltState = newState;
				}

				/*
				 * Compare the two states
				 * Ignore the less critical state
				 */
				if (tempState < voltState) {
					state = tempState;
				} else {
					state = voltState;
				}

				/* Define corresponding actions */
				switch (state) {
					case STATE_OFF:
						led_set_luminance_limit(0);
						break;
					case STATE_50:
						led_set_luminance_limit(5);
						break;
					case STATE_70:
						led_set_luminance_limit(7);
						break;
					case STATE_100:
						led_set_luminance_limit(10);
						break;
				}
			
				sprintf(logstr, "t:%d v:%d\n", (int)tempAvg, (int)voltAvg);
				log_push(logstr);
				
			} // end: if (bufferCounter == BUFFER_SIZE)
			
		} else {
			led_set_luminance_limit(10);
		} // end: if ( emergency == FALSE )
		led_update();
    } // end: while (1)
} // end: main()
#endif
