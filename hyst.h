#ifndef HYST_H
    #define HYST_H

    #include <stdint.h>

	
	#define BUFFER_SIZE 10

    #define NO_STATE_CHANGE -1
	
	#define VOLT_STATE_OFF 0
	#define VOLT_STATE_50 1
	#define VOLT_STATE_70 2
	#define VOLT_STATE_100 3
	
	#define TEMP_STATE_OFF 0
	#define TEMP_STATE_50 1
	#define TEMP_STATE_70 2
	#define TEMP_STATE_100 3
	

    struct Threshold {
		uint16_t lower;
        uint16_t upper;
        uint8_t lstate;
        uint8_t ustate;
    };
	
	extern struct Threshold g_voltageHystTable[];
	extern struct Threshold g_temperatureHystTable[];

    int hyst_check(uint16_t, uint16_t, struct Threshold [], uint8_t);

#endif