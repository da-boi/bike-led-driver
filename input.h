#ifndef INPUT_H
	#define INPUT_H
	
	#define UP_PIN PB0
	#define DOWN_PIN PB1
	
	#define UP (PINB & (1<<UP_PIN)) >> UP_PIN
	#define DOWN (PINB & (1<<DOWN_PIN)) >> DOWN_PIN
	
	#define RELEASED 0
	#define PRESSED 1
	
	#define DEBOUNCE_DELAY 120
	#define EMERGENCY_DELAY 7000
	
	
	enum bool {TRUE, FALSE};

#endif