/*
 * timer1.c
 *
 *  Created on: Oct 14, 2024
 *      Author: MadaYaswanth
 */
#include"main.h"
#include<stdint.h>
#include"automatic_updown.h"
#include"uart.h"
#include<string.h>
#define SELECT_LEFT_RIGHT_SWITCH 2

#define SIZE 10
char uart_receive_data[SIZE];
int can_switch_status;
char *Switch[3]={"not ","up ","down "};
void timer1_init(void)
{
	 RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	 RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	 GPIOA->CRH &= ~(GPIO_CRH_CNF11 | GPIO_CRH_MODE11);
	 GPIOA->CRH |= GPIO_CRH_MODE11_0; // Set PA11
	 TIM1->PSC = 7999;
	 TIM1->ARR = 749;
	 TIM1->DIER |= TIM_DIER_UIE;
     TIM1->CR1 |= TIM_CR1_CEN;
	 NVIC_EnableIRQ(TIM1_UP_IRQn);
}
void TIM1_UP_IRQHandler(void)
{
    if (TIM1->SR & TIM_SR_UIF)
    {
        TIM1->SR &= ~TIM_SR_UIF;   // Clear the update interrupt flag
#if SELECT_LEFT_RIGHT_SWITCH ==2
  int k=check_left_side_switch_status();
  if(k==NORMAL_STATE)
  {
	 uart_string("not ");
  }
  else if(k==UP_STATE)
  {
	  uart_string("up ");
  }
  else if(k==DOWN_STATE)
  {
	  uart_string("down ");
  }
#elif SELECT_LEFT_RIGHT_SWITCH ==1
  uart_receive_data[SIZE]=0;
  uint8_t c,cnt=0;
 while(1)
 {
 c=uart_rx();
 if((c=='\0')||(cnt>=SIZE))
 {
     break;
 }
 uart_receive_data[cnt]=c;
 cnt++;
 }
uart_receive_data[cnt]='\0';
if(strcmp(uart_receive_data,Switch[0])==0)
{
can_switch_status=NORMAL_STATE;
uart_string("0 ");
}
else if(strcmp(uart_receive_data,Switch[1])==0)
{
can_switch_status=UP_STATE;
uart_string("1 ");
}
else if(strcmp(uart_receive_data,Switch[2])==0)
{
can_switch_status=DOWN_STATE;
uart_string("2 ");
}
#endif
    }
}
