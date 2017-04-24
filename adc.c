#include "adc.h"
#include "stdint.h"

void adc_Init(void){
	ADMUX = (1<<REFS0); //Aref = Vcc and using adc0 pin
	ADCSRA = (1<<ADEN)|(1<<ADATE)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2)|(1<<ADSC); // Enabling ADC, interrupts and setting pre-scaler to 128, Start conversion
} 
int8_t get_adcValue(void){
	//getter method
	
	return adcValue;
	// will return 8 bit adc value (0-255)
}

ISR(ADC_vect){
	adcValue = ADC;
}

