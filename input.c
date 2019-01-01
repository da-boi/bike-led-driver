#include <avr/io.h>
#include <avr/interrupt.h>

#include "input.h"


void input_init(void) {
	/* Enable Pin Change interrupt for all 5 pins */
	GIMSK |= (1<<PCIE);
	
	/* Mask only the wanted pins */
	PCMSK = (1<<PCINT0) | (1<<PCINT1);
	
	/* Declare the corresponding pins as inputs */
	PORTB &= ~( (1<<PB0) | (1<<PB1) );
}

ISR(PCINT0_vect) {
	
}