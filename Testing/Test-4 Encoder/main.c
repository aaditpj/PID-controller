#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "stdint.h"

volatile int32_t encoderValue;
volatile int8_t dir;

void encoder_Init(void);

int main(void){
	DDRB |= (1 << DDB1);
    // PB1 and PB2 is now an output
	DDRB = 0xff;
	encoder_Init();
	PORTD = 0x00;
	sei(); //enabling global interrupts
	PORTB=0xff;
	
	while(1){
	if(encoderValue>=1440)
	{
	PORTB |= 1<<PINB1;                    // for testing only
	}
	else
	PORTB &= ~(1<<PINB1);
	
}
}


void encoder_Init(void){
	EIMSK |= (1<<INT0)|(1<<INT1); // enabling external interrupt 0
	EICRA |= (1<<ISC10)|(1<<ISC00); //Interrupt set on logic change
	encoderValue = 0;
}

//interrupt service routine for quadrature encoding

ISR(INT0_vect)
{
	if (bit_is_set(PIND,3))
    {
        if (bit_is_set(PIND,2))
        {
            dir=1;
            encoderValue++;
        }
        else
        {
            dir=0;
            encoderValue--;
        }
    }
    else
    {
        if (bit_is_set(PIND,2) )
        {
            dir=0;
            encoderValue--;
        }
        else
        {
            dir=1;
            encoderValue++;
        }
    }
}	
	

ISR(INT1_vect){

	if (bit_is_set(PIND,3))
    {
        if (bit_is_set(PIND,2))
        {
            dir=0;
            encoderValue--;
        }
        else
        {
            dir=1;
            encoderValue++;
        }
    }
    else
    {
        if (bit_is_set(PIND,2))
        {
            dir=1;
            encoderValue++;
        }
        else
        {
            dir=0;
            encoderValue--;
        }
    }
	
}