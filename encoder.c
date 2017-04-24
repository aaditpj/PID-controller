+#include "encoder.h"
#include "stdint.h"

void encoder_Init(void){
	EIMSK = (1<<INT0); // enabling external interrupt 0;
	EICRA = (1<<ISC01); //Interrupt set on falling edge
	encoderValue = 0;
	dir=0;
}

// setter method for the encoder value;
void encoder_home(void){
	encoderValue=0;
}
//getter method t get the position
int32_t get_encoderValue(void){
	return encoderValue;
}
 
// getter method for the direction
int8_t get_encoderDirection(void){
	return dir;
}

ISR(INT0_vect)
{
	if(){//pin b is high
		encoderValue++;
		dir=1;
	}
	else{
		encoderValue--;
		dir=0;
	}
}