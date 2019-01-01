#ifndef LED_H
	#define LED_H
	
	#include "pwm.h"
	
	
	#define LED_STEPS 11
	
	
	int led_set_luminance(uint8_t);
	int led_set_luminance_limit(uint8_t);
	void led_increase(void);
	void led_decrease(void);
	void led_check(void);
	
#endif