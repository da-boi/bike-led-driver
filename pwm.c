#include <avr/io.h>

#include "pwm.h"


volatile uint8_t g_TCCR1;
volatile uint8_t g_OCR1C;
volatile uint8_t g_OCR1B;


void pwm_init(void) {
	/* For the OC1B to work the corresponding I/O pin (PB4) must be set as a output. */
	DDRB |= (1 << PB4);
	
	/* Setting up the Timer to use OC1B */
	GTCCR =
		(0b10 << COM1B0)		// OC1B cleared on compare match. Set when TCNT1 = $00
		| (1 << PWM1B)			// Pulse Width Modulator B Enable
	;
	
	/* Set pwm frequency to ~300Hz */
	TCCR1 = (0b1000 << CS10);	// Set prescaler to	F_CPU/128
	OCR1C = _PWM_TOP;			// Set value on which to clear the counter
	
	/* Set duty cycle to 0% */
	OCR1B = 214;
}

uint8_t pwm_set_duty(uint8_t duty) {
	OCR1B = duty;
}

void pwm_strobe_on(void) {
	/* Save previous Timer1 settings to later restore them */
	g_TCCR1 = TCCR1;
	g_OCR1C = OCR1C;
	g_OCR1B = OCR1B;
	
	TCCR1 = (TCCR1 & 0b11110000) + 0b1110;	// Set prescaler to	F_CPU/8192
	
	OCR1C = 125;							// Set value on which to clear the counter, to alter the frequency
	
	OCR1B = 62;								// Set duty cycle to ~50%
}

void pwm_strobe_off(void) {
	/* Reset Timer1 settings saved in pwm_strobe_on() */
	TCCR1 = g_TCCR1;
	OCR1C = g_OCR1C;
	OCR1B = g_OCR1B;
}