#ifndef pwm_H
#define pwm_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "stdint.h"

int16_t DutyCycle;

void pwm_Init(void);
void set_pwm(int16_t pwm);

#endif