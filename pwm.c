#include "pwm.h"
#include "stdint.h"

void pwm_Init(){
	// Using timer 1
	TCCR1A = (1<<COM1A1)|(1<<WGM10)|(1<<WGM11)|(1<<WGM12); //Setting up 10 bit fast PWM
	TIMSK1 = (1<<TOIE1); // for interrupt
	OCR1A  = 0;
	sei();
	TCCR1B = (1<<CS10);//pre-scaler is set to 1
	
}

void set_pwm(int16_t pwm){
	// setter method to dynamically set pwm during program execution
	
	DutyCycle = pwm; //10 bit PWM values (0-1024)
}

ISR(TIMER1_OVF_vect){
	OCR1A = DutyCycle;
}
//done this way to avoid heisenbugs