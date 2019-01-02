#ifndef HYST_H
    #define HYST_H

    #include <stdint.h>

	
	#define BUFFER_SIZE 10

    #define NO_STATE_CHANGE -1
	
	#define STATE_OFF 0
	#define STATE_50 1
	#define STATE_70 2
	#define STATE_100 3
	
	#define TEMP_100C 490
	#define TEMP_80C 810
	#define TEMP_60C 1190
	#define TEMP_55C 1530

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