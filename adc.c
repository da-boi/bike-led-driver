#include <avr/io.h>

#include "adc.h"


void adc_init(void) {
	ADMUX =
		  (0 << REFS1)		// Set VCC as reference voltage together with REFS0
		| (0 << REFS0)		// (see above)
		| (1 << ADLAR)		// Set data register to left adjusted
		| (0b00 << MUX0)	// Enable ADC0 as single ended input
	;
	
	ADCSRA =
		  (1 << ADEN)		// Enable the ADC
		| (0 << ADSC)		// Halt the start of ADC
		| (0 << ADATE)		// Disable auto trigger
		| (0 << ADIE)		// Disable adc conversion complete interrupt
		| (0b111 << ADPS0)	// Set prescaler division factor to 128
	;
	
	ADCSRB =
		  (0 << BIN)			// Disable bipolar input mode
		| (0 << IPR)			// Disable input polarity reversal
	;
	
	DIDR0 =				// Disable input buffer for all ADC
		  (1 << ADC0D)
		| (1 << ADC1D)
		| (1 << ADC2D)
		| (1 << ADC3D)
	;
}

uint16_t adc_get_voltage(void) {
	uint32_t adcValue = 0;
	
	/* Enable ADC1 as single ended input */
	ADMUX = (ADMUX & ADC_MUX_MASK) | ADC_MUX_VOLTAGE;
	
	/* Enable input buffer for ADC1 */
	DIDR0 &= ~(1 << ADC_VOLTAGE);
	
	/* Start the ADC */
	ADCSRA |= (1 << ADSC);
	
	/* Wait for completion of conversion */
	while ( ADCSRA & (1 << ADSC) );
	
	/* Read and store ADC */
	adcValue = ADCH;
	
	/* Disable input buffer for ADC1 */
	DIDR0 |= (1 << ADC_VOLTAGE);
	
	/* Calculate and return voltage */
	return (uint16_t)(adcValue * 5000 / 256);
}

uint16_t adc_get_temperature(void) {
	uint32_t adcValue = 0;
	
	/* Enable ADC3 as single ended input */
	ADMUX = (ADMUX & ADC_MUX_MASK) | ADC_MUX_TEMPERATURE;
	
	/* Enable input buffer for ADC3 */
	DIDR0 &= ~(1 << ADC_TEMPERATURE);
	
	/* Start the ADC */
	ADCSRA |= (1 << ADSC);
	
	/* Wait for completion of conversion */
	while ( ADCSRA & (1 << ADSC) );
	
	/* Read and store ADC */
	adcValue = ADCH;
	
	/* Disable input buffer for ADC3 */
	DIDR0 |= (1 << ADC_TEMPERATURE);
	
	/* Calculate and return temperature */
	return (uint16_t)(adcValue * 5000 / 256);
}