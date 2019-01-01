#include <avr/io.h>
#include <avr/interrupt.h>

#include "time.h"


/* Declare a global variable for overflow counting */
volatile uint64_t timer0OvfCounter = 0;

void time_init(void) {
	/*
	 * Leave waveform generation on "Normal" - WGM0x = 0
	 * Leave port operation disconnected - COM0xx = 0
	 * Set prescaler to	F_CPU/8 (assuming F_CPU == 8MHz) - CS02 = 0, CS01 = 1, CS00 = 1
	 *  - 1,000,000 ticks/second -> 3,906.25 overflows/second
	*/
	TCCR0A = 0x00;
	TCCR0B = (1 << CS01) | (0 << CS00);
	
	/* Enable overflow interrupt */
	TIMSK |= (1 << TOIE0);
	
	/* Global interrupt must still be enabled */
}

/* Increment the overflow counter on every overflow of TCNT0 */
ISR(TIM0_OVF_vect) {
	timer0OvfCounter++;
}


uint64_t time_get_micros(void) {
	/*
	 * Multiply the overflow counter by 256 to get microseconds
	 * Add the microseconds from TCNT0 to it
	 * Return the result
	*/
	return ( (timer0OvfCounter << 8) + TCNT0 );
}

uint32_t time_get_millis(void) {
	/*
	 * Map the overflow counter from 256us to 1000us
	 * Return the result
	*/
	return (uint32_t)( (timer0OvfCounter << 8) / 1000 );
}

void time_delay_micros(uint64_t delay) {
	uint32_t start = time_get_micros();
	while ( time_get_micros() - start < delay);
}

void time_delay_millis(uint32_t delay) {
	uint32_t start = time_get_millis();
	while ( time_get_millis() - start < delay);
}