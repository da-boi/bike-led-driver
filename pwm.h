#ifndef PWM_H
	#define PWM_H
	
	
	#define FALSE 0
	#define TRUE 1

	#define _PWM_TOP 214
	#define _PWM_BOTTOM 0

	/*
	 * Initializes the Timer1 in pwm mode
	 * with fixed frequency of 300 Hz assuming F_CPU == 8MHz
	 * a non inverted signal is created on PB4
	 * initializes with a 0% duty cycle
	 * use pwm_set_duty() to change the duty
	*/
	void pwm_init(void);
	
	uint8_t pwm_set_duty(uint8_t duty);
	
	void pwm_strobe_on(void);
	
	void pwm_strobe_off(void);
	
#endif