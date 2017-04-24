#ifndef encoder_H
#define encoder_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "stdint.h"

int8_t dir;
int32_t encoderValue;

void encoder_Init(void);
void ecoder_home(void);
int32_t get_encoderValue(void);
int8_t get_encoderDirection(void);


#endif