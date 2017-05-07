#include <avr/io.h>
#include <avr/interrupt.h>

  
// initialize timer, interrupt and variable
void timer1_init(void)
{
    // set up timer with prescaler = 8 and CTC mode
    TCCR1B |= (1 << WGM12)|(1 << CS11);
  
    // set up timer OC1A pin in toggle mode
    TCCR1A |= (1 << COM1A0);
	
  
    // initialize counter
    TCNT1 = 0;
    // initialize compare value
    OCR1A = 0;
}

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

void set_speed(float RPM){

if(RPM<=0.1)
{
	TCCR1A &= ~(1 << COM1A0);
}
else if( RPM >=585)
{
	TCCR1A |= (1 << COM1A0);
	OCR1A=1;
}
else
{
	TCCR1A |= (1 << COM1A0);
	OCR1A= (uint16_t)((1171.876/RPM)-1);
}


}
  
int main(void)
{
    DDRB |= (1<<PINB1);
    // initialize timer
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); 
	
	
    timer1_init();
	
	set_speed((float)(ReadADC()*0.57)); 
  
    // loop forever
    while(1)
    {
		set_speed((float)(ReadADC()*0.57)); 
    }
}