#include "hyst.h"
#include "adc.h"

struct Threshold g_voltageHystTable[3] =  {
	{   // Threshold 0
		2277,
		2415,
		STATE_OFF,
		STATE_50
	},
	{   // Threshold 1
		2414,
		2484,
		STATE_50,
		STATE_70
	},
	{   // Threshold 2
		2483,
		2588,
		STATE_70,
		STATE_100
	}
};

struct Threshold g_temperatureHystTable[3] =  {
	{   // Threshold 0
		TEMP_100C,
		TEMP_80C,
		STATE_OFF,
		STATE_50
	},
	{   // Threshold 1
		TEMP_80C,
		TEMP_60C,
		STATE_50,
		STATE_70
	},
	{   // Threshold 2
		TEMP_60C,
		TEMP_55C,
		STATE_70,
		STATE_100
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