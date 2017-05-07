#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "stdint.h"

volatile int32_t encoderValue;

void encoder_Init(void);

int main(void){
	DDRB |= (1 << DDB1);
    // PB1 and PB2 is now an output

   /* OCR1A = 0;


    TCCR1A |= (1 << COM1A1);
    // set none-inverting mode

    TCCR1A |= (1 << WGM10)|(1<<WGM11);
    TCCR1B |= (1 << WGM12);
    // set Fast PWM mode using ICR1 as TOP
	TIMSK1 |= (1 << OCIE1A);
	sei();
    TCCR1B |= (1 << CS10);
    // START the timer with no prescaler*/
	DDRB = 0xff;
	encoder_Init();
	PORTD = 0xff;
	sei(); //enabling global interrupts
	PORTB=0xff;
	
	while(1){
	if(encoderValue<=20)
	{
	PORTB |= 1<<PINB1; // for testing only
	}
	else
	PORTB = 0x00;
	
}
}


void encoder_Init(void){
	EIMSK |= (1<<INT0); // enabling external interrupt 0;
	EICRA |= (1<<ISC01)|(1<<ISC00); //Interrupt set on rising edge
	encoderValue = 0;
}

//interrupt service routine
ISR(INT0_vect)
{
	encoderValue++; //encrement encoder value on every pulse
	
	
}