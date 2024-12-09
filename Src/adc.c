/*
 * adc.c
 *
 *  Created on: Sep 12, 2024
 *      Author: MadaYaswanth
 */
#include"main.h"
#include<stdint.h>
#include"uart.h"
#define NUMBER_OF_SAMPLES 15
static int first_time=0;
int storing_adc_values[NUMBER_OF_SAMPLES];
extern float THRESHOLD_CURRENT;
extern void window_motor_up(void);
extern void window_motor_down(void);
void window_motor_stop(void);
void delay_ms(int ms)
{
ms*=80000;
while(ms--);
}
void adc_init(void)
{
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;//Enable clock for adc1
	RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;//Clock GPIOB
    GPIOB->CRL &= ~(GPIO_CRL_MODE0);  // Clear MODE bits for PB0 (00 - input mode)
    GPIOB->CRL &= ~(GPIO_CRL_CNF0);   // Clear CNF bits for PB0 (00 - analog mode)
    ADC1->CR1=0;
    ADC1->CR2=0;
    ADC1->SQR3=8;
    ADC1->CR2=ADC_CR2_CONT;//CONTINIOUS CONVERSION
    ADC1->CR2|=ADC_CR2_ADON;
    /*The ADC can be powered-on by setting the ADON bit in the ADC_CR2 register. When the
ADON bit is set for the first time, it wakes up the ADC from Power Down mode.
Conversion starts when ADON bit is set for a second time by software after ADC power-up
time*/
    delay_ms(10);
    ADC1->CR2|=ADC_CR2_ADON;
}
int adc_read(void)
{
	while (!(ADC1->SR & ADC_SR_EOC));
	return ADC1->DR;
}
void anti_pinch(void)
{
window_motor_stop();
for(int i=0;i<10;i++);
window_motor_down();
delay_ms(20);
window_motor_stop();
while(!(GPIOA->IDR>>5 &1));
}
int sumofaverage(int *ar)
{
      long int sum=0;
      for(int i=0;i<NUMBER_OF_SAMPLES;i++)
      {
        sum+=ar[i];
      }
      return (sum/NUMBER_OF_SAMPLES);
}
int movingarray(int n)
{
  int i;
  if(first_time==0)
  {
      first_time=1;
      return sumofaverage(storing_adc_values);
  }
  for(i=0;i<NUMBER_OF_SAMPLES;i++)
  {
	  storing_adc_values[i]=storing_adc_values[i+1];
  }
  storing_adc_values[--i]=n;
  return sumofaverage(storing_adc_values);
}
int motor_current()
{
	   int i,adc_value;
	   float adc_motor_current=0,adc_voltage=0;
	   if(first_time==0)
	   {
	       for(i=0;i<NUMBER_OF_SAMPLES;i++)
	       {
	    	   storing_adc_values[i]=adc_read();
	       }
	       adc_value=movingarray(0);//dummy value first time
	   }
	   else
	   {
		   adc_value=movingarray(adc_read());
	   }
	   adc_voltage=((float)adc_value/4095.0)*3.239;
	   uart_float(adc_voltage);
	   uart_tx(' ');
	   adc_motor_current=((adc_voltage-2.52)/0.066);
	   if(adc_motor_current<0)
	   {
		   adc_motor_current=-adc_motor_current;
	   }
	   uart_float(adc_motor_current);
	   uart_tx(' ');
		if(adc_motor_current>THRESHOLD_CURRENT)
		{
		 anti_pinch();
		 first_time=0;
		 return 0;
		}
return 1;
}

