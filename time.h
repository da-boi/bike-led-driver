#ifndef TIME_H
	#define TIME_H
	
	#include <avr/interrupt.h>
	
	
	/*
	 * Initializes the Timer0 as a microsecond counter
	 * Global interrupt must still be enabled afterwards
	*/
	void time_init(void);
	
	/*
	 * Returns the elapsed microseconds since global interrupt enable
	 * The time_init() must be called before at startup,
	 *  otherwise the function's return is unusable
	 * Precision: 1us (assuming the clock speed is exactly 8MHz)
	*/
	uint64_t time_get_micros(void);
	
	/*
	 * Returns the elapsed milliseconds since global interrupt enable
	 * The time_init() must be called before at startup,
	 *  otherwise the function's return is unusable
	 * Precision: 1ms (assuming the clock speed is exactly 8MHz)
	*/
	uint32_t time_get_millis(void);
	
	void time_delay_micros(uint64_t);
	
	void time_delay_millis(uint32_t);
	
#endif