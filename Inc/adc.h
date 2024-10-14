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
uint16_t adc_read(void);
float avg_adc(void);
int motor_current(void);
uint32_t movingarray(uint16_t n);
uint32_t sumofaverage(uint16_t *ar);
#endif /* INC_ADC_H_ */
