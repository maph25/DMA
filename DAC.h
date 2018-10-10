/*
 * DAC.h
 *
 *  Created on: Sep 24, 2018
 *      Author: Andrea Perez, Sandra Fernanda
 */

#ifndef DAC_H_
#define DAC_H_

#include "Bits.h"

#define DAC0_clockGating_mask 0x1000;
#define DAC1_clockGating_mask 0x1001;

void sine_led();
void square_led();
void triangular_led();

void DAC_clockGating(void);
void DAC_output(void);

void Wave_gen();
#endif /* DAC_H_ */
