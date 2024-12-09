/*
 * ac.h
 *
 *  Created on: Sep 12, 2024
 *      Author: MadaYaswanth
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include<stdint.h>
void delay_ms(int );
void adc_init(void);
int adc_read(void);
float avg_adc(void);
int motor_current(void);
int movingarray(uint16_t n);
int sumofaverage(uint16_t *ar);
#endif /* INC_ADC_H_ */
