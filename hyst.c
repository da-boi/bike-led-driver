#include "hyst.h"
#include "adc.h"

struct Threshold g_voltageHystTable[3] =  {
	{   // Threshold 0
		2970,
		3150,
		VOLT_STATE_OFF,
		VOLT_STATE_50
	},
	{   // Threshold 1
		3149,
		3240,
		VOLT_STATE_50,
		VOLT_STATE_70
	},
	{   // Threshold 2
		3239,
		3390,
		VOLT_STATE_70,
		VOLT_STATE_100
	}
};

struct Threshold g_temperatureHystTable[3] =  {
	{   // Threshold 0
		TEMP_100C,
		TEMP_80C,
		TEMP_STATE_OFF,
		TEMP_STATE_50
	},
	{   // Threshold 1
		TEMP_80C,
		TEMP_60C,
		TEMP_STATE_50,
		TEMP_STATE_70
	},
	{   // Threshold 2
		TEMP_60C,
		TEMP_55C,
		TEMP_STATE_70,
		TEMP_STATE_100
	},
};


int hyst_check(uint16_t value,
               uint16_t previous,
               struct Threshold hystTable[],
               uint8_t size) {
    int i;
    int state = NO_STATE_CHANGE;

    if ( value >= previous ) {
        for (i=0; i < size; i++) {
            if ( value >= hystTable[i].upper ) {
                state = hystTable[i].ustate;
            }
        }
    } else {
        for (i=(size-1); i >= 0; i--) {
            if ( value <= hystTable[i].lower ) {
                state = hystTable[i].lstate;
            }
        }
    }

    return state;
}