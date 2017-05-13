#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int32_t encoderValue;
volatile int8_t dir;
volatile int32_t maxSpeed=150;


void encoder_Init(void){
	EIMSK |= (1<<INT0)|(1<<INT1); // enabling external interrupt 0
	EICRA |= (1<<ISC10)|(1<<ISC00); //Interrupt set on logic change
	encoderValue = 0;
	
}



  
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
	
	DDRB |= (1<<PINB1);
}

void set_speed(float RPM){


if(RPM<=0.1)
{
	TCCR1A &= ~(1 << COM1A0);
}
else if( RPM >=maxSpeed )
{
	TCCR1A |= (1 << COM1A0);
	OCR1A=(uint16_t)((1171.876/maxSpeed)-1);;
}
else
{
	TCCR1A |= (1 << COM1A0);
	OCR1A= (uint16_t)((1171.876/RPM)-1);
}


}

uint8_t set_position(int32_t curPos, int32_t rqPos){
	
	int32_t error, currentSpeed;
	
	float kp=5.0f;
	
 	error=curPos-rqPos;
	currentSpeed= error*kp;
	
	if(currentSpeed<0)
	{
		PORTB &= ~(1<<PINB2);
		currentSpeed=-1*currentSpeed;
	}
	else
		PORTB |= 1<<PINB2; //change direction
	
	set_speed(currentSpeed);
	
	if(error==0)
		return 1;
	else
		return 0;
}

int main(void){

	encoder_Init();
	timer1_init();
	sei();
	DDRB |=1<<PINB2 | 1<<PINB0;

	while(1){
		
		set_position(encoderValue,1440);
	}

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


