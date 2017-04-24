#ifndef adc_H
#define adc_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "stdint.h"

int8_t adcValue;

void adc_Init(void);
int8_t get_adcValue(void);

#endif