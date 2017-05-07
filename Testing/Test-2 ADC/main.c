 #include <avr/io.h>
 #include <avr/interrupt.h>
 
 uint16_t ReadADC(void)
{
   //Start Single conversion
   ADCSRA|=(1<<ADSC);

   //Wait for conversion to complete
   while(!(ADCSRA & (1<<ADIF)));

   //Clear ADIF by writing one to it
   //Note you may be wondering why we have write one to clear it
   //This is standard way of clearing bits in io as said in datasheets.
   //The code writes '1' but it result in setting bit to '0' !!!

   ADCSRA|=(1<<ADIF);

   return(ADC);
}
 
 int main(void){
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); 
	
	DDRB |= (1 << DDB1)|(1 << DDB2)|(1<<PINB3);
    // PB1 and PB2 is now an output

    OCR1A = 0;
    // set PWM for 25% duty cycle @ 8bit

    OCR1B = 125;
    // set PWM for 75% duty cycle @ 8bit

    TCCR1A |= (1 << COM1A1)|(1 << COM1B1);
    // set none-inverting mode

    TCCR1A |= (1 << WGM10)|(1<<WGM11);
    TCCR1B |= (1 << WGM12);
    // set Fast PWM mode using ICR1 as TOP
	TIMSK1 |= (1 << OCIE1A);
	sei();
    TCCR1B |= (1 << CS10);
    // START the timer with no prescaler
	
	while(1){
		
	}
	
}

ISR(TIMER1_COMPA_vect){

	OCR1A= ReadADC();
}