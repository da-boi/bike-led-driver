#ifndef ADC_H
	#define ADC_H
	
	
	#define ADC_REFERENCE_VOLTAGE 5
	#define ADC_VOLTAGE ADC1D
	#define ADC_TEMPERATURE ADC3D
	#define ADC_MUX_VOLTAGE 0b0001
	#define ADC_MUX_TEMPERATURE 0b0011
	#define ADC_MUX_MASK 0b11110000
	
	
	/* Initializes the ADC */
	void adc_init(void);
	
	/* Returns the voltage on PB2 in mV */
	uint16_t adc_get_voltage(void);
	
	/* Returns the voltage on PB3 in mV */
	uint16_t adc_get_temperature(void);
	
#endif