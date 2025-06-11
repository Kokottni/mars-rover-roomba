/**
 * adc.h
 * Created by Nicholas Kokott and Alexander Hardcopf
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include <math.h>

//initialize the ADC
void adc_init(void);

//read the value the adc is currently seeing
uint16_t adc_read(void);

uint16_t adc_translate(uint16_t translate);

#endif
