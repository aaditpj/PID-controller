#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int dutycycle=0;

int main(void)
{
    DDRB |= (1 << DDB1)|(1 << DDB2)|(1<<PINB3);
    // PB1 and PB2 is now an output

    OCR1A = 100;
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

    

    while (1)
    {
	  dutycycle+=10;
	  if(dutycycle>=1020)
		dutycycle=0;
	
	for(int i=0; i<10000; i++);
	PORTB |= (1<<PINB3);
		
	  OCR1A=dutycycle;	
	_delay_ms(10);
    }
}

ISR(TIMER1_COMPA_vect)
{
  
}
