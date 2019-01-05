#include <avr/io.h>

#include "led.h"


const uint8_t c_lightness[11] = {
	214, 190, 188, 185, 180, 170, 154, 127, 93, 51, 0
};

volatile uint8_t g_luminance = 0;
volatile uint8_t g_luminanceLimit = 10;


int led_set_luminance(uint8_t index) {
	if ( index >= LED_STEPS ) {
		return -1;
	}
	
	g_luminance = index;
	pwm_set_duty(c_lightness[index]);
	return 0;
}

int led_set_luminance_limit(uint8_t index) {
	if ( index >= LED_STEPS ) {
		return -1;
	}
	g_luminanceLimit = index;
	return 0;
}

void led_increase(void) {
	int index = g_luminance;
	
	if ( ++index > g_luminanceLimit ) {
		index = g_luminanceLimit;
	}
	
	led_set_luminance(index);
}

void led_decrease(void) {
	int index = g_luminance;
	
	if ( --index < 0 ) {
		index = 0;
	}
	
	led_set_luminance(index);
}

void led_update(void) {
	if ( g_luminance > g_luminanceLimit ) {
		led_set_luminance(g_luminanceLimit);
	}
}