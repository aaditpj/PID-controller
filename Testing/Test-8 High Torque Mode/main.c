#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

volatile int32_t encoderValue;
volatile int8_t dir;
volatile int32_t maxSpeed=150;
volatile int rpm=0;
volatile int stallcl=0;
volatile uint32_t motorSpeed=0;
volatile int32_t lastEncVal=0;


void encoder_Init(void){
	EIMSK |= (1<<INT0)|(1<<INT1); // enabling external interrupt 0
	EICRA |= (1<<ISC10)|(1<<ISC00); //Interrupt set on logic change
	encoderValue = 0;
	
}



// initialize timer, interrupt and variable
void timer0_init(void)
{	
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (1<<CS02)|(1<<CS00);
	OCR0A = 98;
	
	TIMSK0 |= (1<<OCIE0A);
	sei();
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

rpm=RPM;

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
	timer0_init();
	timer1_init();
	sei();
	DDRB |=1<<PINB2 | 1<<PINB0;

	while(1){
		
			while(set_position(encoderValue,1440)==0);
		
		_delay_ms(10);
		
		while(set_position(encoderValue,0)==0);  
		
		_delay_ms(10);
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

ISR(TIMER0_COMPA_vect)
{
	int32_t change;
	change = abs(encoderValue-lastEncVal);
	
	motorSpeed=(uint32_t)abs(change*0.417); 
		
	if(motorSpeed<=maxSpeed-5)
		stallcl++;
	else
		stallcl=0;
	
	if(stallcl>=2)
		maxSpeed--;
	else
		maxSpeed=150;
	
	if(maxSpeed<=5)
		maxSpeed=5;//also sound alaram
		
	

}
