#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint32_t motorSpeed=0;
int32_t lastEncVal=0;
// initialize timer, interrupt and variable
void timer0_init(void)
{	
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (1<<CS02)|(1<<CS00);
	OCR0A = 98;
	
	TIMSK0 |= (1<<OCIE0A);
	sei();
}


ISR(TIMER0_COMPA_vect)
{	
	int32_t change;
	change = encoderValue-lastEncVal;
	
	if(change<0)
		change= -1*change;

	motorSpeed=(uint32_t)(change*0.417);
}
